#include "pch.h"
#include "CDLibWMP.h"

using namespace winrt;
using namespace CDLib;

struct WMPAudioCDPlayerPrivate {
	com_ptr<IWMPCdromCollection> cdromCollection;
	Windows::Foundation::Collections::IVector<IAudioCDDrive> drives;
	IAudioCDTrack playingTrack;


};

WMPAudioCDPlayer::WMPAudioCDPlayer()
	: d(new WMPAudioCDPlayerPrivate)
{
	d->cdromCollection = create_instance<IWMPCdromCollection>(winrt::guid_of<WindowsMediaPlayer>());
	d->drives = single_threaded_vector<IAudioCDDrive>();

	long count;
	check_hresult(d->cdromCollection->get_count(&count));

	for (int i = 0; i < count; i++) 
	{
		com_ptr<IWMPCdrom> cdrom;
		check_hresult(d->cdromCollection->item(i, cdrom.put()));
		d->drives.Append(WMPAudioCDDrive(cdrom));
	}
}

WMPAudioCDPlayer::~WMPAudioCDPlayer()
{
	delete d;
}

Windows::Foundation::Collections::IVectorView<IAudioCDDrive> WMPAudioCDPlayer::GetDrives()
{
	return d->drives.GetView();
}

IAudioCDTrack WMPAudioCDPlayer::PlayingTrack()
{
	return d->playingTrack;
}

void WMPAudioCDPlayer::PlayTrack(const IAudioCDTrack& track)
{
	d->playingTrack = track;
	Play();
}

void WMPAudioCDPlayer::Play()
{
	
}

void WMPAudioCDPlayer::Pause()
{
	
}

struct WMPAudioCDDrivePrivate {
	com_ptr<IWMPCdrom> cd;
};

WMPAudioCDDrive::WMPAudioCDDrive(winrt::com_ptr<IWMPCdrom> cdrom)
	: d(new WMPAudioCDDrivePrivate)
{
	d->cd = cdrom;
}

WMPAudioCDDrive::~WMPAudioCDDrive()
{
	delete d;
}

Windows::Foundation::IReference<char16_t> WMPAudioCDDrive::DriveLetter()
{
	return Windows::Foundation::IReference<char16_t>();
}

IAudioCD WMPAudioCDDrive::InsertedMedia()
{
	return IAudioCD();
}

struct WMPAudioCDPrivate {

};

WMPAudioCD::WMPAudioCD()
	: d(new WMPAudioCDPrivate())
{
}

WMPAudioCD::~WMPAudioCD()
{
	delete d;
}

Windows::Foundation::Collections::IVectorView<IAudioCDTrack> WMPAudioCD::Tracks()
{
	return Windows::Foundation::Collections::IVectorView<IAudioCDTrack>();
}

hstring WMPAudioCD::Name()
{
	return hstring();
}

struct WMPAudioCDTrackPrivate {

};

WMPAudioCDTrack::WMPAudioCDTrack()
	: d(new WMPAudioCDTrackPrivate())
{
}

WMPAudioCDTrack::~WMPAudioCDTrack()
{
	delete d;
}

hstring WMPAudioCDTrack::Name()
{
	return hstring();
}

Windows::Foundation::TimeSpan WMPAudioCDTrack::Duration()
{
	return Windows::Foundation::TimeSpan();
}

uint32_t WMPAudioCDTrack::TrackNumber()
{
	return uint32_t();
}
