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

PROCESSINTERNAL_HOOK(UTGameMatchInProgressBeginState)
{
	auto now{std::chrono::system_clock::now()};
	auto date_string{std::format("{:%d-%m-%Y_%H-%M}", now)};
	auto date_string_wide{std::wstring(date_string.begin(), date_string.end())};
	std::wstring current_map_name{g_game_engine->GetCurrentWorldInfo()->GetURLMap().Data};
	g_demo_command = std::wstring(L"demorec ").append(date_string_wide).append(L"_").append(current_map_name);
	g_game_engine->DeferredCommands.Add(FString(const_cast<wchar_t *>(g_demo_command.c_str())));
}

PROCESSINTERNAL_HOOK(ActorSetInitialState)
{
	if (calling_uobject->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

PROCESSINTERNAL_HOOK(TrPlayerControllerReceiveLocalizedMessage)
{
	if (calling_uobject->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

PROCESSINTERNAL_HOOK(TrPlayerControllerClientShowAccoladeText)
{
	if (calling_uobject->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

PROCESSINTERNAL_HOOK(TrPlayerControllerClientSetHUD)
{
	if (calling_uobject->Class == kDemoRecControllerClass)
	{
		return;
	}
	original_processinternal(calling_uobject, unused, stack, result);
}

PROCESSINTERNAL_HOOK(TrPawnClientUpdateHUDHealth)
{
}

PROCESSINTERNAL_HOOK(WeaponClientGivenTo)
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

PROCESSINTERNAL_HOOK(TrDevice_AutoFireSwitchToPostFireDevice)
{
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