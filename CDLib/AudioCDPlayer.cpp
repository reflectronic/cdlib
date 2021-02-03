#include "pch.h"
#include "AudioCDPlayer.h"
#include "AudioCDPlayer.g.cpp"

//#include "MCI.h"
#include "CDLibWMP.h"

namespace winrt::CDLib::implementation
{
    CDLib::IAudioCDPlayer AudioCDPlayer::GetPlayer()
    {
        if (true)
        {
            return make<WMPAudioCDPlayer>();
        }

        //return make<MCIAudioCDPlayer>();
    }
}
