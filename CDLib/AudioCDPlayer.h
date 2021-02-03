#pragma once
#include "pch.h"
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