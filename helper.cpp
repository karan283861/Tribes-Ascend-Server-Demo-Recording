#include <plog/Log.h>
#include "helper.hpp"

UGameEngine *g_game_engine{};
std::wstring g_demo_command{};
const UClass *kDemoRecControllerClass{DemoRecController::StaticClass()};
