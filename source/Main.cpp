#include "plugin.h"
#include "Events.h"
#include "CPlaneTrails.h"


#include <iostream>
#include <stdio.h>

using namespace plugin;


class Test {
public:
    Test() {
        openConsole();

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

        onComputePathFind += [](CPathFind* pathFind, int nodeType, const CVector* ref_origin, const CVector* ref_dest, CNodeAddress* ref_firstNode, CNodeAddress** ref_route) {
            std::cout << "onComputePathFind" << std::endl;
        };
    }
        

private:

    void openConsole() {
        AllocConsole();
        FILE* fs{};
        freopen_s(&fs, "CONIN$", "r", stdin);
        freopen_s(&fs, "CONOUT$", "w", stdout);
        freopen_s(&fs, "CONOUT$", "w", stderr);
    }
} TestPlugin;
