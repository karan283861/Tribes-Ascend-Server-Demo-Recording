#include <format>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <string>
#include <plog/Log.h>
#include <SdkHeaders.h>
#include "helper.hpp"
#include "processinternal_hooks.hpp"

UE3_PROCESSINTERNAL_HOOK(UTGameMatchInProgressBeginState)
{
	auto now{std::chrono::system_clock::now()};
	auto date_string{std::format("{:%d-%m-%Y_%H-%M}", now)};
	auto date_string_wide{std::wstring(date_string.begin(), date_string.end())};
	std::wstring current_map_name{g_game_engine->GetCurrentWorldInfo()->GetURLMap().Data};
	g_demo_command = std::wstring(L"demorec ").append(date_string_wide).append(L"_").append(current_map_name);
	g_game_engine->DeferredCommands.Add(FString(const_cast<wchar_t *>(g_demo_command.c_str())));
}

UE3_PROCESSINTERNAL_HOOK(ActorSetInitialState)
{
	auto controller{reinterpret_cast<AController *>(calling_uobject)};
	if (controller->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

UE3_PROCESSINTERNAL_HOOK(TrPlayerControllerReceiveLocalizedMessage)
{
	auto controller{reinterpret_cast<AController *>(calling_uobject)};
	if (controller->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

UE3_PROCESSINTERNAL_HOOK(TrPlayerControllerClientShowAccoladeText)
{
	auto controller{reinterpret_cast<AController *>(calling_uobject)};
	if (controller->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

UE3_PROCESSINTERNAL_HOOK(TrPlayerControllerClientSetHUD)
{
	auto controller{reinterpret_cast<AController *>(calling_uobject)};
	if (controller->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

UE3_PROCESSINTERNAL_HOOK(TrPawnClientUpdateHUDHealth)
{
}

UE3_PROCESSINTERNAL_HOOK(WeaponClientGivenTo)
{
	static auto lock{false};
	if (lock)
	{
		return;
	}
	auto weapon{reinterpret_cast<AWeapon *>(calling_uobject)};
	if (weapon->Instigator)
	{
		lock = true;
		// This will call ProcessEvent -> ProcessInternal and come back to this function, so we lock before calling it
		weapon->ClientGivenTo(weapon->Instigator, false);
	}
	lock = false;
}

UE3_PROCESSINTERNAL_HOOK(TrDevice_AutoFireSwitchToPostFireDevice)
{
	// Not sure how much of the code below is actually needed for the functionality
	auto device{reinterpret_cast<ATrDevice_AutoFire *>(calling_uobject)};
	auto inventory_manager{reinterpret_cast<ATrInventoryManager *>(device->InvManager)};
	auto instigator{reinterpret_cast<Player *>(inventory_manager->Instigator)};
	device->ClientWeaponThrown();
	if (device->m_PostFireDevice)
	{
		device->m_PostFireDevice->ClientGivenTo(instigator, false);
		device->m_PostFireDevice->ClientWeaponSet(true, false);
	}
}