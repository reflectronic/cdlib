#pragma once

#include "AudioCDPlayer.h"
#include <wmp.h>

struct WMPAudioCDPlayerPrivate;
struct WMPAudioCDPlayer : winrt::implements<WMPAudioCDPlayer, winrt::CDLib::IAudioCDPlayer, winrt::non_agile>
{
	WMPAudioCDPlayer(winrt::com_ptr<IWMPPlayer> const& player);

	wfc::IVectorView<winrt::CDLib::IAudioCDDrive> GetDrives();

	void PlayTrack(const winrt::CDLib::IAudioCDTrack& track);
	void Pause();
	void Resume();

private:
	winrt::com_ptr<IWMPCdromCollection> cdromCollection;
	winrt::com_ptr<IWMPPlayer> player;
	wfc::IVector<winrt::CDLib::IAudioCDDrive> drives;
	winrt::CDLib::IAudioCDTrack playingTrack;

	inline winrt::com_ptr<IWMPControls> WMPAudioCDPlayer::get_controls()
	{
		winrt::com_ptr<IWMPControls> controls;
		winrt::check_hresult(player->get_controls(controls.put()));
		return controls;
	}
};

struct WMPAudioCDDrive : winrt::implements<WMPAudioCDDrive, winrt::CDLib::IAudioCDDrive, winrt::non_agile>
{
	WMPAudioCDDrive(winrt::com_ptr<IWMPCdrom> const& cdrom);

    wf::IReference<char16_t> DriveLetter();
	winrt::CDLib::IAudioCD InsertedMedia();

	void Eject();

private:
	winrt::com_ptr<IWMPCdrom> cd;
};

struct WMPAudioCD : winrt::implements<WMPAudioCD, winrt::CDLib::IAudioCD, winrt::non_agile>
{
	friend struct WMPAudioCDPlayer;

	WMPAudioCD(winrt::com_ptr<IWMPPlaylist> const& tracks);

	winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> Tracks();
	winrt::hstring Name();

private:
	winrt::com_ptr<IWMPPlaylist> wmpTrackList;
	wfc::IVector<winrt::CDLib::IAudioCDTrack> tracks;

};

struct WMPAudioCDTrack : winrt::implements<WMPAudioCDTrack, winrt::CDLib::IAudioCDTrack, winrt::non_agile>
{
	friend struct WMPAudioCDPlayer;

	WMPAudioCDTrack(winrt::com_ptr<IWMPMedia> const& media, uint32_t track, winrt::CDLib::IAudioCD parentCd);

	winrt::hstring Name();
	winrt::Windows::Foundation::TimeSpan Duration();
	uint32_t TrackNumber();

private:
	winrt::CDLib::IAudioCD parentCd;
	winrt::com_ptr<IWMPMedia> wmpMedia;
	uint32_t trackNumber;
};