#include "pch.h"
#include "AudioCDPlayer.h"
#include "AudioCDPlayer.g.cpp"

namespace winrt::CDLib::implementation
{
    CDLib::IAudioCDPlayer AudioCDPlayer::GetPlayer()
    {
        if (false)
        {
            // TODO: Windows media player backend
            throw_hresult(0);
        }

        return make<MCIAudioCDPlayer>();
    }
}
