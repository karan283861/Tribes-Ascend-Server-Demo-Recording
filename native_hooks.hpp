#pragma once

#include <SdkHeaders.h>

inline constexpr size_t kUGameEngineTickAddress{0x00790530};
inline constexpr size_t kFMallocFree{0x00d1e480};

using GameEngineTickPrototype = void(__fastcall *)(UGameEngine *engine, void *unused, float delta_seconds);
extern GameEngineTickPrototype original_game_engine_tick;
void __fastcall GameEngineTickHook(UGameEngine *engine, void *unused, float delta_seconds);

using FMallocFreePrototype = void(__fastcall *)(void *this_, void *unused, void *ptr);
extern FMallocFreePrototype original_fmalloc_free;
void __fastcall FMallocFreeHook(void *this_, void *unused, void *ptr);
