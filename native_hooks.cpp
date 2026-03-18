#include <format>
#include <plog/Log.h>
#include "helper.hpp"
#include "native_hooks.hpp"

GameEngineTickPrototype original_game_engine_tick = reinterpret_cast<GameEngineTickPrototype>(kUGameEngineTickAddress);
void __fastcall GameEngineTickHook(UGameEngine *engine, void *unused, float delta_seconds)
{
	g_game_engine = engine;
	return original_game_engine_tick(engine, unused, delta_seconds);
}

FMallocFreePrototype original_fmalloc_free = reinterpret_cast<FMallocFreePrototype>(kFMallocFree);
void __fastcall FMallocFreeHook(void *this_, void *unused, void *ptr)
{
	if (ptr == g_demo_command.c_str())
	{
		g_game_engine->DeferredCommands.Clear();
		return;
	}

	original_fmalloc_free(this_, unused, ptr);
}