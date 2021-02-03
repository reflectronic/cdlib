#include "pch.h"
#include "CDLibWMP.h"

namespace cdlib = winrt::CDLib;

WMPAudioCDPlayer::WMPAudioCDPlayer()
	: cdromCollection(winrt::create_instance<IWMPCdromCollection>(winrt::guid_of<WindowsMediaPlayer>())),
	  player(winrt::create_instance<IWMPPlayer>(winrt::guid_of<WindowsMediaPlayer>())),
	  drives(winrt::single_threaded_vector<cdlib::IAudioCDDrive>())
{
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
	get_controls()->playItem(wmpTrack->wmpMedia.get());
}

void WMPAudioCDPlayer::Pause()
{
	get_controls()->pause();
}

void WMPAudioCDPlayer::Resume()
{
	get_controls()->play();
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
	cd->get_playlist(playlist.put());
	return winrt::make<WMPAudioCD>(playlist);
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
		playlist->get_item(i, track.put());
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
	return std::chrono::duration_cast<wf::TimeSpan>(std::chrono::duration<double>(seconds));
}

uint32_t WMPAudioCDTrack::TrackNumber()
{
	return trackNumber;
}
