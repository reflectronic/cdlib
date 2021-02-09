#pragma once

#include "AudioCDPlayer.h"
#include <wmp.h>

struct WMPAudioCDPlayerEventHandler;

struct WMPAudioCDPlayer : winrt::implements<WMPAudioCDPlayer, winrt::CDLib::IAudioCDPlayer, winrt::non_agile>
{
	friend WMPAudioCDPlayerEventHandler;

	WMPAudioCDPlayer(winrt::com_ptr<IWMPPlayer> const& player);

	wfc::IVectorView<winrt::CDLib::IAudioCDDrive> GetDrives();

	void PlayTrack(const winrt::CDLib::IAudioCDTrack& track);
	void Pause();
	void Resume();

	double Volume();
	void Volume(double value);

	wf::TimeSpan CurrentPosition();
	void Seek(wf::TimeSpan position);

	winrt::event_token FinishedPlayingTrack(winrt::CDLib::FinishedPlayingTrackHandler const& handler);
	void FinishedPlayingTrack(winrt::event_token const& token) noexcept;

private:
	winrt::com_ptr<IWMPCdromCollection> cdromCollection;
	winrt::com_ptr<IWMPPlayer> player;

	winrt::com_ptr<WMPAudioCDPlayerEventHandler> eventHandler;
	winrt::com_ptr<IConnectionPoint> connectionPoint;
	DWORD adviseCookie;

	wfc::IVector<winrt::CDLib::IAudioCDDrive> drives;
	winrt::CDLib::IAudioCDTrack playingTrack;

	winrt::event<winrt::CDLib::FinishedPlayingTrackHandler> finishedPlayingTrackEvent;

	inline winrt::com_ptr<IWMPControls> WMPAudioCDPlayer::get_controls()
	{
		winrt::com_ptr<IWMPControls> controls;
		winrt::check_hresult(player->get_controls(controls.put()));
		return controls;
	}

	inline winrt::com_ptr<IWMPSettings> WMPAudioCDPlayer::get_settings()
	{
		winrt::com_ptr<IWMPSettings> settings;
		winrt::check_hresult(player->get_settings(settings.put()));
		return settings;
	}
};

struct WMPAudioCDDrive : winrt::implements<WMPAudioCDDrive, winrt::CDLib::IAudioCDDrive, winrt::non_agile>
{
	friend WMPAudioCDPlayerEventHandler;

	WMPAudioCDDrive(winrt::com_ptr<IWMPCdrom> const& cdrom);

    wf::IReference<char16_t> DriveLetter();
	winrt::CDLib::IAudioCD InsertedMedia();

	winrt::event_token MediaChanged(winrt::CDLib::MediaChangedEventHandler const& handler);
	void MediaChanged(winrt::event_token const& token) noexcept;

	void Eject();

private:
	winrt::com_ptr<IWMPCdrom> cd;

	winrt::event<winrt::CDLib::MediaChangedEventHandler> mediaChangedEvent;
};

struct WMPAudioCD : winrt::implements<WMPAudioCD, winrt::CDLib::IAudioCD, winrt::non_agile>
{
	friend WMPAudioCDPlayer;

	WMPAudioCD(winrt::com_ptr<IWMPPlaylist> const& tracks);

	winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> Tracks();
	winrt::hstring Title();

private:
	winrt::com_ptr<IWMPPlaylist> wmpTrackList;
	wfc::IVector<winrt::CDLib::IAudioCDTrack> tracks;

};

struct WMPAudioCDTrack : winrt::implements<WMPAudioCDTrack, winrt::CDLib::IAudioCDTrack, winrt::non_agile>
{
	friend WMPAudioCDPlayer;

	WMPAudioCDTrack(winrt::com_ptr<IWMPMedia> const& media);

	winrt::hstring Title();
	winrt::hstring AlbumTitle();
	winrt::hstring Artist();
	winrt::hstring AlbumCoverUrl();
	uint32_t TrackNumber();

	winrt::Windows::Foundation::TimeSpan Duration();

private:
	winrt::com_ptr<IWMPMedia> wmpMedia;
};