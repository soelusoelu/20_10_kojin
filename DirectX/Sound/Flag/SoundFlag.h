#pragma once

#include <xaudio2.h>
#include <x3daudio.h>

//サウンド関連フラグ
namespace SoundFlags {
constexpr unsigned NOPITCH = XAUDIO2_VOICE_NOPITCH;
constexpr unsigned NOSRC = XAUDIO2_VOICE_NOSRC;
constexpr unsigned USE_FILTER = XAUDIO2_VOICE_USEFILTER;
}
