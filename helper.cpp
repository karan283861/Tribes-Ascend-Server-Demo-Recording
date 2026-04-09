#include <plog/Log.h>
#include "helper.hpp"

UGameEngine *g_game_engine{};
std::wstring g_demo_command{};

bool IsPlayerValid(Player *player)
{
	if (player && player->PlayerReplicationInfo && player->Health && !player->bDeleteMe)
	{
		return true;
	}
	return false;
}

const UClass *kDemoRecControllerClass{DemoRecController::StaticClass()};
