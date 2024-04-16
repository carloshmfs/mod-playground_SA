#include "plugin.h"
#include "Events.h"
#include "CSprite.h"
#include "CFont.h"
#include "CTheScripts.h"


#include <iostream>
#include <stdio.h>
#include <string>

#define MAX_PATH 260

int SetSaveDirectoryHook(char* name)
{
    (void)name;

    std::cout << "HELLO WORLD FROM HookTest()" << std::endl;
    const char* NewDir = "C:\\Users\\ROBOTT\\Downloads\\test";

    sprintf_s(*(char(*)[MAX_PATH])0xC16F18, "%s\\%s", NewDir, "GTASAsf");

    return 0;
}

void JetPackCheatHook()
{
    std::cout << "JetPackCheatHook" << std::endl;
}

using namespace plugin;


class Test {
public:
    Test()
    {
        OpenConsole();
        HookInstallEvents();
    }
        
private:
    const CVector* pathnode_from_pos;
    const CVector* pathnode_to_pos;

    void HookInstallEvents()
    {
        Events::drawHudEvent += [&]() {
            if (pathnode_from_pos) {
                PrintText2ScreenFromWorldPos("ORIGEM", *pathnode_from_pos, { 0, 0, 255, 255 });
            }

            if (pathnode_to_pos) {
                PrintText2ScreenFromWorldPos("DESTINO", *pathnode_to_pos, { 0, 255, 255, 255 });
            }
        };

        static StdcallEvent <AddressList<0x4E9EF3, H_CALL>, PRIORITY_BEFORE, ArgPickN<signed char, 0>, void(signed char)> onGetRadioStationName;
        onGetRadioStationName += [](signed char station) {
            switch (station)
            {
            case eRadioID::RADIO_K_DST:
                std::cout << "RADIO_K_DST" << std::endl;
                break;

            case eRadioID::RADIO_PLAYBACK_FM:
                std::cout << "RADIO_PLAYBACK_FM" << std::endl;
                break;

            case eRadioID::RADIO_K_ROSE:
                std::cout << "RADIO_K_ROSE" << std::endl;
                break;

            case eRadioID::RADIO_BOUNCE_FM:
                std::cout << "RADIO_BOUNCE_FM" << std::endl;
                break;

            case eRadioID::RADIO_SF_UR:
                std::cout << "RADIO_SF_UR" << std::endl;
                break;

            case eRadioID::RADIO_LOS_SANTOS:
                std::cout << "RADIO_LOS_SANTOS" << std::endl;
                break;

            case eRadioID::RADIO_RADIO_X:
                std::cout << "RADIO_RADIO_X" << std::endl;
                break;

            case eRadioID::RADIO_CSR_103_9:
                std::cout << "RADIO_CSR_103_9" << std::endl;
                break;

            case eRadioID::RADIO_K_JAH_WEST:
                std::cout << "RADIO_K_JAH_WEST" << std::endl;
                break;

            case eRadioID::RADIO_MASTER_SOUNDS_98_3:
                std::cout << "RADIO_MASTER_SOUNDS_98_3" << std::endl;
                break;

            case eRadioID::RADIO_WCTR:
                std::cout << "RADIO_WCTR" << std::endl;
                break;

            case eRadioID::RADIO_USER_TRACKS:
                std::cout << "RADIO_USER_TRACKS" << std::endl;
                break;

            case eRadioID::RADIO_NONE:
                std::cout << "RADIO_NONE" << std::endl;
                break;

            default:
                break;
            }
            };


        static ThiscallEvent <
            AddressList<0x66F049, H_CALL>,
            PRIORITY_BEFORE,
            ArgPick6N<CPathFind*, 0, int, 1, const CVector*, 2, const CVector*, 3, CNodeAddress*, 4, CNodeAddress**, 5>,
            void(CPathFind*, int, const CVector*, const CVector*, CNodeAddress*, CNodeAddress**)
        > onComputePathFind;

        onComputePathFind += [&](CPathFind* pathFind, int nodeType, const CVector* ref_origin, const CVector* ref_dest, CNodeAddress* ref_firstNode, CNodeAddress** ref_route) mutable {
            std::cout << "onComputePathFind" << std::endl;

            pathnode_from_pos = ref_origin;
            pathnode_to_pos = ref_dest;
        };

        patch::RedirectCall({ 0x747476 }, SetSaveDirectoryHook);
        patch::RedirectJump({ 0x439600 }, JetPackCheatHook);
    }

    void PrintText2ScreenFromWorldPos(const std::string text, const CVector& posn, CRGBA color) const
    {
        RwV3d rwp = { posn.x, posn.y, posn.z + 1.0f };
        RwV3d screenCoors; 
        float w, h;

        if (CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true)) {
            CFont::SetOrientation(ALIGN_CENTER);
            CFont::SetColor(color);
            CFont::SetDropShadowPosition(1);
            CFont::SetBackground(false, false);
            CFont::SetWrapx(500.0);
            CFont::SetScale(0.5, 1.0);
            CFont::SetFontStyle(FONT_SUBTITLES);
            CFont::SetProportional(true);
            CFont::PrintString(screenCoors.x, screenCoors.y, text.c_str());
        }
    }

    void OpenConsole() const
    {
        AllocConsole();
        FILE* fs{};
        freopen_s(&fs, "CONIN$", "r", stdin);
        freopen_s(&fs, "CONOUT$", "w", stdout);
        freopen_s(&fs, "CONOUT$", "w", stderr);
    }
} TestPlugin;
