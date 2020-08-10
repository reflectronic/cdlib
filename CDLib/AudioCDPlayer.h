#pragma once

#include "AudioCDPlayer.g.h"

namespace winrt::CDLib::implementation
{
    struct AudioCDPlayer
    {
        AudioCDPlayer() = default;

        static CDLib::IAudioCDPlayer GetPlayer();
    };
}

namespace winrt::CDLib::factory_implementation
{
    struct AudioCDPlayer : AudioCDPlayerT<AudioCDPlayer, implementation::AudioCDPlayer>
    {
    };
}
