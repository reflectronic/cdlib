#include "pch.h"
#include "AudioCDPlayer.h"
#include "AudioCDPlayer.g.cpp"

#include "MCI.h"

namespace winrt::CDLib::implementation
{
    CDLib::IAudioCDPlayer AudioCDPlayer::GetPlayer()
    {
        if (false)
        {
            // TODO: Windows media player backend
        }

        return make<MCIAudioCDPlayer>();
    }
}
