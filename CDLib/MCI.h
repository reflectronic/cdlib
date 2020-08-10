#pragma once

#include "AudioCDPlayer.h"
#include <mciapi.h>

struct MCIAudioCDPlayer : winrt::implements<MCIAudioCDPlayer, winrt::CDLib::IAudioCDPlayer>
{
    MCIAudioCDPlayer();
    bool IsMetadataSupported();
    bool IsMediaInserted();
    winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCD> GetInsertedMedia();
    void Close();

private:
    MCIDEVICEID deviceHandle;
};

struct MCIAudioCD : winrt::implements<MCIAudioCD, winrt::CDLib::IAudioCD>
{
    MCIAudioCD(MCIDEVICEID deviceHandle);
    winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> Tracks();
    winrt::Windows::Foundation::IReference<char16_t> DriveLetter();

private:
    MCIDEVICEID deviceHandle;
};

struct MCIAudioCDTrack : winrt::implements<MCIAudioCDTrack, winrt::CDLib::IAudioCDTrack>
{
    MCIAudioCDTrack(winrt::Windows::Foundation::TimeSpan& duration, uint32_t trackNumber);

    winrt::hstring Name();
    winrt::Windows::Foundation::TimeSpan Duration();
    uint32_t TrackNumber();
    
    void Play();
    void Pause();
    void Stop();

private:
    winrt::Windows::Foundation::TimeSpan duration;
    uint32_t trackNumber;
};