#pragma once

#include "AudioCDPlayer.h"

struct MCIAudioCDPlayer : winrt::implements<MCIAudioCDPlayer, winrt::CDLib::IAudioCDPlayer>
{
    bool IsMetadataSupported();
    bool IsMediaInserted();
    winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCD> GetInsertedMedia();
    void Close();
};

struct MCIAudioCD : winrt::implements<MCIAudioCD, winrt::CDLib::IAudioCD>
{
    winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> Tracks();
    winrt::Windows::Foundation::IReference<char16_t> DriveLetter();
};

struct MCIAudioCDTrack : winrt::implements<MCIAudioCDTrack, winrt::CDLib::IAudioCDTrack>
{
    winrt::hstring Name();
    winrt::Windows::Foundation::TimeSpan Duration();
    uint32_t TrackNumber();
    
    void Play();
    void Pause();
    void Stop();

protected:
    friend MCIAudioCD;
    MCIAudioCDTrack(uint32_t trackNumber);
};