#include "pch.h"
#include "CDLibWMP.h"

#include <wmpids.h>

namespace cdlib = winrt::CDLib;

struct WMPAudioCDPlayerEventHandler : winrt::implements<WMPAudioCDPlayerEventHandler, _WMPOCXEvents, winrt::non_agile>
{
	WMPAudioCDPlayerEventHandler(winrt::com_ptr<WMPAudioCDPlayer> const& cdPlayer)
		: cdPlayer(cdPlayer)
	{
	}

	HRESULT __stdcall Invoke(
		DISPID dispIdMember,
		[[maybe_unused]] REFIID riid,
		[[maybe_unused]] LCID lcid,
		[[maybe_unused]] WORD wFlags,
		DISPPARAMS* pDispParams,
		[[maybe_unused]] VARIANT* pVarResult,
		[[maybe_unused]] EXCEPINFO* pExcepInfo,
		[[maybe_unused]] UINT* puArgErr) noexcept final
	{
		if (!pDispParams)
			return E_POINTER;

		if (pDispParams->cNamedArgs != 0)
			return DISP_E_NONAMEDARGS;

		HRESULT hr = S_OK;

		try 
		{
			switch (dispIdMember)
			{
				case DISPID_WMPCOREEVENT_PLAYSTATECHANGE:
				{
					if (pDispParams->rgvarg[0].lVal == wmppsMediaEnded)
					{
						cdPlayer->finishedPlayingTrackEvent();
					}

					break;
				}

				case DISPID_WMPCOREEVENT_CDROMMEDIACHANGE:
				{
					long index = pDispParams->rgvarg[0].lVal;
					auto drive = cdPlayer->drives.GetAt(index);
					drive.as<WMPAudioCDDrive>()->mediaChangedEvent(drive);	
					break;
				}

				default:
				{
					hr = DISP_E_MEMBERNOTFOUND;
					break;
				}
			}

			return hr;
		}
		catch (...)
		{
			return winrt::to_hresult();
		}
	}

	HRESULT __stdcall GetTypeInfoCount([[maybe_unused]] UINT* pctinfo) noexcept final
	{
		return E_NOTIMPL;
	}

	HRESULT __stdcall GetTypeInfo(
		[[maybe_unused]] UINT iTInfo,
		[[maybe_unused]] LCID lcid,
		[[maybe_unused]] ITypeInfo** ppTInfo) noexcept final
	{
		return E_NOTIMPL;
	}

	HRESULT __stdcall GetIDsOfNames(
		[[maybe_unused]] REFIID riid,
		[[maybe_unused]] LPOLESTR* rgszNames,
		[[maybe_unused]] UINT cNames,
		[[maybe_unused]] LCID lcid,
		[[maybe_unused]] DISPID* rgDispId) noexcept final
	{
		return E_NOTIMPL;
	}

private:
	winrt::com_ptr<WMPAudioCDPlayer> cdPlayer;
};

WMPAudioCDPlayer::WMPAudioCDPlayer(winrt::com_ptr<IWMPPlayer> const& wmpPlayer)
	 : player(wmpPlayer),
	   drives(winrt::single_threaded_vector<cdlib::IAudioCDDrive>())
{
	player->get_cdromCollection(cdromCollection.put());

	long count;
	winrt::check_hresult(cdromCollection->get_count(&count));

	for (int i = 0; i < count; i++) 
	{
		winrt::com_ptr<IWMPCdrom> cdrom;
		winrt::check_hresult(cdromCollection->item(i, cdrom.put()));
		drives.Append(winrt::make<WMPAudioCDDrive>(cdrom));
	}

	// TODO: Consider requesting IWMPEvents in addition to _WMPOCXEvents, 
	// and implementing it on WMPAudioCDPlayerEventHandler.
	auto container = player.as<IConnectionPointContainer>();
	if (SUCCEEDED(container->FindConnectionPoint(winrt::guid_of<_WMPOCXEvents>(), connectionPoint.put())))
	{
		winrt::com_ptr<WMPAudioCDPlayer> thisPtr;
		thisPtr.copy_from(this);

		eventHandler = winrt::make_self<WMPAudioCDPlayerEventHandler>(thisPtr);
		connectionPoint->Advise(eventHandler.get(), &adviseCookie);
	}
}

wfc::IVectorView<cdlib::IAudioCDDrive> WMPAudioCDPlayer::GetDrives()
{
	return drives.GetView();
}

void WMPAudioCDPlayer::PlayTrack(const cdlib::IAudioCDTrack& track)
{
	auto wmp3 = player.as<IWMPCore3>();
	auto wmpTrack = track.as<WMPAudioCDTrack>();

	wil::unique_bstr name;
	winrt::check_hresult(wmpTrack->wmpMedia->get_name(name.put()));

	winrt::com_ptr<IWMPPlaylist> playlist;
	winrt::check_hresult(wmp3->newPlaylist(name.get(), nullptr, playlist.put()));
	winrt::check_hresult(playlist->appendItem(wmpTrack->wmpMedia.get()));

	winrt::check_hresult(player->put_currentPlaylist(playlist.get()));
	winrt::check_hresult(get_controls()->playItem(wmpTrack->wmpMedia.get()));
}

void WMPAudioCDPlayer::Pause()
{
	winrt::check_hresult(get_controls()->pause());
}

void WMPAudioCDPlayer::Resume()
{
	winrt::check_hresult(get_controls()->play());
}

double WMPAudioCDPlayer::Volume()
{
	long volume;
	winrt::check_hresult(get_settings()->get_volume(&volume));
	return static_cast<double>(volume) / 100.0;
}

void WMPAudioCDPlayer::Volume(double value)
{
	long volume = static_cast<long>(value * 100);
	winrt::check_hresult(get_settings()->put_volume(volume));
}

wf::TimeSpan to_timespan(double seconds)
{
	return std::chrono::duration_cast<wf::TimeSpan>(std::chrono::duration<double>(seconds));
}

wf::TimeSpan WMPAudioCDPlayer::CurrentPosition()
{
	double seconds;
	winrt::check_hresult(get_controls()->get_currentPosition(&seconds));
	return to_timespan(seconds);
}

void WMPAudioCDPlayer::Seek(wf::TimeSpan position)
{
	double doublePosition{ std::chrono::duration_cast<std::chrono::duration<double>>(position).count() };
	winrt::check_hresult(get_controls()->put_currentPosition(doublePosition));
}

winrt::event_token WMPAudioCDPlayer::FinishedPlayingTrack(winrt::CDLib::FinishedPlayingTrackHandler const& handler)
{
	return finishedPlayingTrackEvent.add(handler);
}

void WMPAudioCDPlayer::FinishedPlayingTrack(winrt::event_token const& token) noexcept
{
	finishedPlayingTrackEvent.remove(token);
}

WMPAudioCDDrive::WMPAudioCDDrive(winrt::com_ptr<IWMPCdrom> const& cdrom)
	: cd(cdrom)
{
}

winrt::Windows::Foundation::IReference<char16_t> WMPAudioCDDrive::DriveLetter()
{
	wil::unique_bstr name;
	cd->get_driveSpecifier(name.put());
	
	if (SysStringLen(name.get()) < 1)
	{
		return nullptr;
	}

	return name.get()[0];
}

cdlib::IAudioCD WMPAudioCDDrive::InsertedMedia()
{
	winrt::com_ptr<IWMPPlaylist> playlist;
	winrt::check_hresult(cd->get_playlist(playlist.put()));
	return winrt::make<WMPAudioCD>(playlist);
}

winrt::event_token WMPAudioCDDrive::MediaChanged(winrt::CDLib::MediaChangedEventHandler const& handler)
{
	return mediaChangedEvent.add(handler);
}

void WMPAudioCDDrive::MediaChanged(winrt::event_token const& token) noexcept
{
	mediaChangedEvent.remove(token);
}

void WMPAudioCDDrive::Eject()
{
	cd->eject();
}

WMPAudioCD::WMPAudioCD(winrt::com_ptr<IWMPPlaylist> const& playlist) 
	: wmpTrackList(playlist),
	  tracks(winrt::single_threaded_vector<cdlib::IAudioCDTrack>())
{
	long count;
	winrt::check_hresult(playlist->get_count(&count));
	
	for (int i = 0; i < count; i++)
	{
		winrt::com_ptr<IWMPMedia> track;
		winrt::check_hresult(playlist->get_item(i, track.put()));

		winrt::com_ptr<WMPAudioCD> thisPtr;
		thisPtr.copy_from(this);

		tracks.Append(winrt::make<WMPAudioCDTrack>(track, i + 1));
	}
}

winrt::Windows::Foundation::Collections::IVectorView<cdlib::IAudioCDTrack> WMPAudioCD::Tracks()
{
	return tracks.GetView();
}

template<typename T>
winrt::hstring get_name(winrt::com_ptr<T> const& namedObj)
{
	wil::unique_bstr name;
	winrt::check_hresult(namedObj->get_name(name.put()));

	return winrt::to_hstring(name.get());
}

winrt::hstring WMPAudioCD::Name()
{
	return get_name(wmpTrackList);
}

WMPAudioCDTrack::WMPAudioCDTrack(winrt::com_ptr<IWMPMedia> const& media, uint32_t track)
	: wmpMedia(media),
	  trackNumber(track)
{
}

winrt::hstring WMPAudioCDTrack::Name()
{
	return get_name(wmpMedia);
}

wf::TimeSpan WMPAudioCDTrack::Duration()
{
	double seconds;
	winrt::check_hresult(wmpMedia->get_duration(&seconds));
	return to_timespan(seconds);
}

uint32_t WMPAudioCDTrack::TrackNumber()
{
	return trackNumber;
}
