#pragma once

#include <string>
#include <vector>
#include <SdkHeaders.h>

using Player = ATrPlayerPawn;
using Controller = ATrPlayerController;
using DemoRecController = ATrDemoRecSpectator;

extern UGameEngine *g_game_engine;
extern std::wstring g_demo_command;

// Get all instances of a specific UObject type in the GObjects buffer.
template <class T>
std::vector<T *> GetInstancesUObjects(void)
{
	std::vector<T *> found_uobjects;

	for (int i = 0; i < UObject::GObjObjects()->Count; ++i)
	{
		UObject *object = UObject::GObjObjects()->Data[i];
		if (!object || !object->IsA(T::StaticClass()))
			continue;

		found_uobjects.push_back(reinterpret_cast<T *>(object));
	}
	return found_uobjects;
}

extern const UClass *kDemoRecControllerClass;