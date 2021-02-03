#pragma once

#include "AudioCDPlayer.h"
#include <wmp.h>

struct WMPAudioCDPlayerPrivate;
struct WMPAudioCDPlayer : winrt::implements<WMPAudioCDPlayer, winrt::CDLib::IAudioCDPlayer>
{
	WMPAudioCDPlayer();
	~WMPAudioCDPlayer();

	winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDDrive> GetDrives();

	winrt::CDLib::IAudioCDTrack PlayingTrack();

	void PlayTrack(const winrt::CDLib::IAudioCDTrack& track);

	void Play();
	void Pause();
	void Stop();

private:
	WMPAudioCDPlayerPrivate* d;
};

struct WMPAudioCDDrivePrivate;
struct WMPAudioCDDrive : winrt::implements<WMPAudioCDDrive, winrt::CDLib::IAudioCDDrive>
{
	WMPAudioCDDrive(winrt::com_ptr<IWMPCdrom> cdrom);
	~WMPAudioCDDrive();

    winrt::Windows::Foundation::IReference<char16_t> DriveLetter();
	winrt::CDLib::IAudioCD InsertedMedia();

private:
	WMPAudioCDDrivePrivate* d;
};

struct WMPAudioCDPrivate;
struct WMPAudioCD : winrt::implements<WMPAudioCD, winrt::CDLib::IAudioCD>
{
	WMPAudioCD();
	~WMPAudioCD();

	winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> Tracks();
	winrt::hstring Name();

private:
	WMPAudioCDPrivate* d;
};

struct WMPAudioCDTrackPrivate;
struct WMPAudioCDTrack : winrt::implements<WMPAudioCDTrack, winrt::CDLib::IAudioCDTrack>
{
	WMPAudioCDTrack();
	~WMPAudioCDTrack();

	winrt::hstring Name();
	winrt::Windows::Foundation::TimeSpan Duration();
	uint32_t TrackNumber();

private:
	WMPAudioCDTrackPrivate* d;
};