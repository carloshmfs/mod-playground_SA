#include "plugin.h"
#include "Events.h"
#include "CTheScripts.h"

#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <cstdint>

class ReverseFunction {
public:
	static inline bool MY_IsPlayerOnAMission(void) 
	{
		int _OnAMissionFlag = CTheScripts::OnAMissionFlag;
		if (_OnAMissionFlag) {
			if (reinterpret_cast<uint8_t*>(0xA49960)[_OnAMissionFlag]) {
                std::cout << "[ReverseFuntion] MISSION: " << std::hex << reinterpret_cast<uint8_t*>(0xA49960)[_OnAMissionFlag] << std::endl;
				return true;
			}
		}

		return false;
	};

    ReverseFunction() 
	{
		OpenConsole();
		InstallHooks();
	};

private:
	static inline void OnGameProcess()
	{
		std::cout << "[ReverseFunction] OnMissionFlag: " << CTheScripts::OnAMissionFlag << std::endl;
	};

	void InstallHooks() const
	{
		std::cout << "[ReverseFunction] Installing Hooks" << std::endl;

		plugin::Events::gameProcessEvent += OnGameProcess;

		plugin::patch::ReplaceFunction(0x464D50, MY_IsPlayerOnAMission, false);
	};

	void OpenConsole() const
	{
		AllocConsole();
        FILE* fs{};
        freopen_s(&fs, "CONIN$", "r", stdin);
        freopen_s(&fs, "CONOUT$", "w", stdout);
        freopen_s(&fs, "CONOUT$", "w", stderr);
	};
} ReverseFunctionPlugin;
