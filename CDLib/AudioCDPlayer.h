#pragma once
<<<<<<< HEAD

=======
#include "pch.h"
>>>>>>> 45b7514... Use WMP for CD audio
#include "AudioCDPlayer.g.h"

namespace winrt::CDLib::implementation
{
    struct AudioCDPlayer
    {
        AudioCDPlayer() = default;

        static CDLib::IAudioCDPlayer GetPlayer();
    };
}

FACTORY(winrt::CDLib, AudioCDPlayer);