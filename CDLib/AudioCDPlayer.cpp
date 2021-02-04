#include "pch.h"
#include "AudioCDPlayer.h"
#include "AudioCDPlayer.g.cpp"
#include "CDLibWMP.h"

namespace winrt::CDLib::implementation
{
    CDLib::IAudioCDPlayer AudioCDPlayer::GetPlayer()
    {
        if (auto player = winrt::try_create_instance<IWMPPlayer>(winrt::guid_of<WindowsMediaPlayer>()))
        {
            return make<WMPAudioCDPlayer>(player);
        }
        else
        {
            // WMP isn't supported.
            // In the future, we may want to fall back to a dumber backend like MCI.
            return nullptr;
        }

    }
}
