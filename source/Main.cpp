#include "plugin.h"
#include "Events.h"
#include "CSprite.h"
#include "CFont.h"
#include "CTheScripts.h"
#include "CPathFind.h"
#include "CPlayerInfo.h"
#include "CNodeAddress.h"


#include <iostream>
#include <stdio.h>
#include <string>
#include <stdint.h>

namespace PluginPlayground {

    using namespace plugin;

    typedef uint8_t uint8;
    typedef uint16_t  uint16;
    typedef int32_t   int32;

    // CPathFind.h
    enum class ePathType : uint8 {
        PATH_TYPE_VEH, /// Cars, boats, race tracks
        PATH_TYPE_PED, /// Peds

        PATH_TYPE_ALL /// @notsa
    };

    int SetSaveDirectoryHook(char* name)
    {
        std::cout << "HELLO WORLD FROM HookTest()" << std::endl;
        std::cout << "OLD PATH: " << name << std::endl;
        const char* NewDir = "C:\\Users\\ROBOTT\\Downloads\\test";
        std::cout << "NEW PATH: " << NewDir << std::endl;

        //sprintf_s(*(char(*)[MAX_PATH])0xC16F18, "%s\\%s", NewDir, "GTASAsf");
        sprintf_s(*reinterpret_cast<char(*)[MAX_PATH]>(0xC16F18), "%s\\%s", NewDir, "GTASAsf");

        return 0;
    }

    void JetPackCheatHook()
    {
        std::cout << "JetPackCheatHook" << std::endl;
    }

    CNodeAddress FindNodeClosestToCoorsHook(CVector* pos, unsigned char pathType, float, bool, bool, bool, bool, bool)
    {
        std::cout << "OnFindNodeClosestToCoors() " << pos->x << " " << pos->y << " " << pos->z << " " << pathType << std::endl;
        return {};
    }

    class Test {
    public:
        Test()
        {
            OpenConsole();
            HookInstallEvents();
        }

        static void OnFindNodeClosestToCoors(CVector* pos, unsigned char pathType, float, bool, bool, bool, bool, bool)
        {
            std::cout << "OnFindNodeClosestToCoors() " << pos->x << " " << pos->y << " " << pos->z << " " << pathType << std::endl;
        }

    private:
        const CVector* pathnode_from_pos;
        const CVector* pathnode_to_pos;
        CVector* pTargetedPedPos;
        float* fTargetedPedRotation;

        bool hasCallIt = false;

        void HookInstallEvents()
        {

            Events::drawHudEvent += [&]() {
                if (pathnode_from_pos) {
                    PrintText2ScreenFromWorldPos("ORIGEM", *pathnode_from_pos, { 0, 0, 255, 255 });
                }

                if (pathnode_to_pos) {
                    PrintText2ScreenFromWorldPos("DESTINO", *pathnode_to_pos, { 0, 255, 255, 255 });
                }

                if (pTargetedPedPos) {
                    std::cout << pTargetedPedPos->x << " " << pTargetedPedPos->y << " " << pTargetedPedPos->z << " " << *fTargetedPedRotation << std::endl;
                }
                };

            Events::gameProcessEvent += [&]() mutable {

                //static uint OnMissionFlag = patch::Get<bool>(0xA476AC, true);
                //std::cout << reinterpret_cast<int*>(CTheScripts::OnAMissionFlag) << std::endl;

                CPlayerPed* pPlayer = FindPlayerPed();
                if (pPlayer) {
                    CPed* pTargetedPed = pPlayer->m_pPlayerTargettedPed;
                    if (pTargetedPed && !hasCallIt) {
                        hasCallIt = true;

                        pTargetedPedPos = &pTargetedPed->GetPosition();
                        fTargetedPedRotation = &pTargetedPed->m_fCurrentRotation;

                        // FIXME: crash hen attempt to call
                        //CNodeAddress node = reinterpret_cast<CNodeAddress(*)(CVector, unsigned char, float, float)>(0x44FCE0)(*pTargetedPedPos, (unsigned char) 1, 1.0f, 2.0f);
                        //CNodeAddress* node = plugin::CallAndReturn<CNodeAddress*, 0x44F460, CVector*, ePathType, float, uint16, int32, uint16, uint16, int32>(
                        //    pTargetedPedPos, ePathType::PATH_TYPE_PED, 500.0f, 1, 2, 3, 0, 4);

                        //CNodeAddress* node = plugin::Call<CNodeAddress*, 0x44F460, CVector*, ePathType, float, uint16, int32, uint16, uint16, int32>(
                        //    pTargetedPedPos, ePathType::PATH_TYPE_PED, 500.0f, 1, 2, 3, 0, 4);


                        //plugin::CallMethod<0x44F460, CPathFind*, CVector, unsigned char, float, bool, bool, bool, bool, bool>(
                        //    &ThePaths, *pTargetedPedPos, ePathType::PATH_TYPE_PED, 100.0f, true, true, true, true, true);

                        //plugin::patch::SetFloat({ 0x858624 }, 0.0f);
                        //std::cout << &ThePaths << std::endl;

                    }
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
                ArgPick6N<CPathFind*, 0, int, 1, const CVector*, 2, const CVector*, 3, const CNodeAddress&, 4, const CNodeAddress&, 5>,
                void(CPathFind*, int, const CVector*, const CVector*, const CNodeAddress&, const CNodeAddress&)
            > onComputePathFind;

            onComputePathFind += [&](
                CPathFind* pathFind,
                int nodeType,
                const CVector* ref_origin,
                const CVector* ref_dest,
                const CNodeAddress& address,
                const CNodeAddress& nodeRoute
            ) mutable {
                    std::cout << "onComputePathFind" << std::endl;

                    pathnode_from_pos = ref_origin;
                    pathnode_to_pos = ref_dest;
            };

            //static ThiscallEvent <
            //    AddressList<0x424454, H_CALL>,
            //    PRIORITY_BEFORE,
            //    ArgPick<ArgTypes<CVector*, unsigned char, float, bool, bool, bool, bool, bool>, 0, 1, 2, 3, 4, 5, 6, 7>,
            //    void(CVector*, unsigned char, float, bool, bool, bool, bool, bool)
            //> onFindNodeClosestToCoorsEvent;
            //onFindNodeClosestToCoorsEvent += OnFindNodeClosestToCoors;

            //static CdeclEvent <
            //    AddressList<0x6717E9, H_CALL>,
            //    PRIORITY_BEFORE,
            //    ArgPickN<void*, 0>,
            //    void(void*)
            //> onPedTaskPathSearch;
            //onPedTaskPathSearch += [&](void* ped) {
            //    std::cout << "onPedTaskPathSearch" << std::endl;
            //};

            patch::RedirectCall({ 0x747476 }, SetSaveDirectoryHook);
            //patch::RedirectCall({ 0x424454 }, FindNodeClosestToCoorsHook, true);
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

}
