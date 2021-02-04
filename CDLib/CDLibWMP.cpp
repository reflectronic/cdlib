#include "pch.h"
#include "CDLibWMP.h"

namespace cdlib = winrt::CDLib;

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
}

wfc::IVectorView<cdlib::IAudioCDDrive> WMPAudioCDPlayer::GetDrives()
{
	return drives.GetView();
}

void WMPAudioCDPlayer::PlayTrack(const cdlib::IAudioCDTrack& track)
{
	auto wmpTrack = track.as<WMPAudioCDTrack>();
	winrt::check_hresult(player->put_currentPlaylist(wmpTrack->parentCd.as<WMPAudioCD>()->wmpTrackList.get()));
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
		tracks.Append(winrt::make<WMPAudioCDTrack>(track, i + 1, *this));
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

WMPAudioCDTrack::WMPAudioCDTrack(winrt::com_ptr<IWMPMedia> const& media, uint32_t track, cdlib::IAudioCD parentCd)
	: wmpMedia(media),
	  trackNumber(track),
	parentCd(parentCd)
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
	return std::chrono::duration_cast<wf::TimeSpan>(std::chrono::duration<double>(seconds));
}

uint32_t WMPAudioCDTrack::TrackNumber()
{
	return trackNumber;
}
