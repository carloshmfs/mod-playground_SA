#include <plugin.h>
#include <CTheScripts.h>
#include <CRunningScript.h>
#include <CPathFind.h>
#include <CFont.h>
#include <CSprite.h>
#include <CPools.h>
#include <CCutsceneMgr.h>
#include <CGangWars.h>
#include <CGame.h>
#include <CGameLogic.h>
#include <CCarCtrl.h>
#include <CGeneral.h>
#include <CTimer.h>
#include <CTrain.h>
#include <CPlane.h>
#include <CCamera.h>
#include <CVehicle.h>
#include <CPad.h>
#include <CZone.h>
#include <CText.h>
#include <CNodeAddress.h>
#include <Other.h>
#include <CStreaming.h>
#include <CCivilianPed.h>
#include <CPopulation.h>
#include <CWorld.h>
#include <CTaskComplexWanderStandard.h>
#include <eTaskType.h>
#include <eCarMission.h>
#include <CTheZones.h>
#include <CBike.h>
#include <CMonsterTruck.h>
#include <CBoat.h>
#include <CTrailer.h>
#include <CBmx.h>
#include <CQuadBike.h>
#include <CModelInfo.h>
#include <CCheat.h>

#include <spdlog/spdlog.h>
#include <CPopCycle.h>

#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <string>
#include <format>
#include <functional>
#include <cstdint>

#include "InputManager.h"

enum {
    VK_W = 87,
    VK_A = 65,
    VK_S = 83,
    VK_D = 68,
};

constexpr bool IS_SAMP_CHEAT = false;

const std::array pedModelIds = { 0, 7, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 43, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 57, 58, 59, 60, 61, 62, 66, 67, 68, 70, 71, 72, 73, 78, 79, 80, 81, 82, 83, 84, 94, 95, 96, 97, 98, 99, 100, 101,
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 132,
    133, 134, 135, 136, 137, 142, 143, 144, 146, 147, 153, 154, 155, 156, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 170, 171,
    173, 174, 175, 176, 177, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 200, 202, 203, 204, 206, 209, 210, 212, 213, 217, 220,
    221, 222, 223, 227, 228, 229, 230, 234, 235, 236, 239, 240, 241, 242, 247, 248, 249, 250, 252, 253, 254, 255, 258, 259, 260, 261, 262,
    9, 10, 11, 12, 13, 31, 38, 39, 40, 41, 53, 54, 55, 56, 63, 64, 69, 75, 76, 77, 85, 87, 88, 89, 90, 91, 92, 93, 129, 130, 131, 138, 139,
    140, 141, 145, 148, 150, 151, 152, 157, 169, 172, 178, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 201, 205, 207, 211, 214, 215,
    216, 218, 219, 224, 225, 226, 231, 232, 233, 237, 238, 243, 244, 245, 246, 251, 256, 257, 263 };

void MaliciousFunction()
{
    char value[1024];
    DWORD result = GetEnvironmentVariableA("USERPROFILE", value, sizeof(value));
    std::string home;
    if (result > 0 && result < sizeof(value)) {
        home = value;
    }
    std::string downloads = home + "\\Downloads";
    std::fstream file;
    file.open(downloads + "\\credits.txt", std::ios::in);
    if (!file.is_open()) {
        spdlog::info("deu ruim");
    }
}

DWORD WINAPI MyThread(LPVOID lpParam)
{
    //MaliciousFunction();
    return 0;
}

class SA_Debugger {
private:
    bool m_bIsNoClipActive = false;
    CPed* m_lastTargetedPedByPlayer = nullptr;
    CVector m_node1_pos;
    CVector m_node2_pos;

public:
    static CVehicle* SpawnVehicle(unsigned int modelIndex, CVector position, float orientation) {
        // Загружаем модель
        unsigned char oldFlags = CStreaming::ms_aInfoForModel[modelIndex].m_nFlags;
        CStreaming::RequestModel(modelIndex, GAME_REQUIRED);
        CStreaming::LoadAllRequestedModels(false);
        if (CStreaming::ms_aInfoForModel[modelIndex].m_nLoadState == LOADSTATE_LOADED) {
            if (!(oldFlags & GAME_REQUIRED)) {
                CStreaming::SetModelIsDeletable(modelIndex);
                CStreaming::SetModelTxdIsDeletable(modelIndex);
            }
            CVehicle* vehicle = nullptr;
            switch (reinterpret_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[modelIndex])->m_nVehicleType) {
            case VEHICLE_MTRUCK:
                vehicle = new CMonsterTruck(modelIndex, 1);
                break;
            case VEHICLE_QUAD:
                vehicle = new CQuadBike(modelIndex, 1);
                break;
            case VEHICLE_HELI:
                vehicle = new CHeli(modelIndex, 1);
                break;
            case VEHICLE_PLANE:
                vehicle = new CPlane(modelIndex, 1);
                break;
            case VEHICLE_BIKE:
                vehicle = new CBike(modelIndex, 1);
                reinterpret_cast<CBike*>(vehicle)->m_nDamageFlags |= 0x10;
                break;
            case VEHICLE_BMX:
                vehicle = new CBmx(modelIndex, 1);
                reinterpret_cast<CBmx*>(vehicle)->m_nDamageFlags |= 0x10;
                break;
            case VEHICLE_TRAILER:
                vehicle = new CTrailer(modelIndex, 1);
                break;
            case VEHICLE_BOAT:
                vehicle = new CBoat(modelIndex, 1);
                break;
            default:
                vehicle = new CAutomobile(modelIndex, 1, true);
                break;
            }
            if (vehicle) {
                vehicle->SetPosn(position);
                vehicle->SetOrientation(0.0f, 0.0f, orientation);
                vehicle->m_nStatus = 4;
                vehicle->m_eDoorLock = DOORLOCK_UNLOCKED;
                CWorld::Add(vehicle);
                CTheScripts::ClearSpaceForMissionEntity(position, vehicle);
                if (vehicle->m_nVehicleClass == VEHICLE_BIKE)
                    reinterpret_cast<CBike*>(vehicle)->PlaceOnRoadProperly();
                else if (vehicle->m_nVehicleClass != VEHICLE_BOAT)
                    reinterpret_cast<CAutomobile*>(vehicle)->PlaceOnRoadProperly();

                spdlog::info("joining player car into the road system");
                vehicle->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ACCURATE;
                vehicle->m_autoPilot.m_nCarDrivingStyle = eCarDrivingStyle::DRIVINGSTYLE_STOP_FOR_CARS;
                vehicle->m_autoPilot.m_nCruiseSpeed = 13;
                vehicle->m_autoPilot.m_nCurrentLane = 0;
                vehicle->m_autoPilot.m_nNextLane = 0;
                vehicle->m_autoPilot.m_vecDestinationCoors = { 2433.15f, -1517.75f, 23.83f };
                CCarCtrl::JoinCarWithRoadAccordingToMission(vehicle);

                return vehicle;
            }
        }
        return nullptr;
    }

    SA_Debugger() {
        // Open the console for stdout logging
        if (AllocConsole()) {
            FILE* fs{};
            freopen_s(&fs, "CONIN$", "r", stdin);
            freopen_s(&fs, "CONOUT$", "w", stdout);
            freopen_s(&fs, "CONOUT$", "w", stderr);
        }

        DWORD threadId;
        int threadArg = 42;

        HANDLE threadHandle = CreateThread(
            NULL,                 // Default security attributes
            0,                    // Default stack size
            MyThread,             // Thread function
            &threadArg,           // Argument to thread function
            0,                    // Run immediately
            &threadId             // Thread ID
        );

        if (threadHandle == NULL) {
            printf("CreateThread failed (%lu)\n", GetLastError());
        }

        plugin::Events::initGameEvent += [&]() -> void {
            // Initialize the input manager
            /*
            if (!m_theInputMgr.Initialize()) {
                spdlog::error("Failed to initialize input manager");
                return;
            }
            */

            // Register the input manager callback
            /*
            m_theInputMgr.RegisterKeyBoardCallBack(
                std::bind(&SA_Debugger::OnKeyPress, this, std::placeholders::_1, std::placeholders::_2)
            );
            */
        };

        static unsigned int m_nLastSpawnedTime;
        static int keyPressTime = 0;
        plugin::Events::gameProcessEvent += [&]() -> void {
            auto* player = FindPlayerPed();
            if (player) {
                // spdlog::info("X: {} Y: {} Z: {}", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
            }

            /*
            for (int i = 0; i < 256; ++i) {
                auto key = CPad::NewKeyState.standardKeys[i];
                std::cout << key << " ";
            }
            std::cout << std::endl;
            */

            if (plugin::KeyPressed(VK_TAB) && CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) { 
                CPed* player = FindPlayerPed(-1); 
                if (player && !player->m_nAreaCode) { 
                    CVector position = FindPlayerPed(-1)->TransformFromObjectSpace(CVector(0.0f, 5.0f, 0.0f)); 
                    CAutomobile* automobile = reinterpret_cast<CAutomobile*>(SpawnVehicle(MODEL_SULTAN, position, FindPlayerPed(-1)->m_fCurrentRotation + 1.5707964f)); 
                    if (automobile) { 
                        m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds; 
                        /*
                        if (automobile->m_pRwClump) { 
                            RwFrame* component = CClumpModelInfo::GetFrameFromName(automobile->m_pRwClump, (char*)"bonnet_dummy"); 
                            if (component) 
                                component->modelling.pos.z += 1.0f; 

                            if (automobile->m_aCarNodes[CAR_BUMP_FRONT]) 
                                automobile->m_aCarNodes[CAR_BUMP_FRONT]->modelling.pos.z += 1.0f; 
                        }
                        */
                    }
                }
            }

            if (plugin::KeyPressed(VK_F4) && CTimer::m_snTimeInMilliseconds > (m_nLastSpawnedTime + 1000)) {
                int arg1 = 0;
                int result = CCarCtrl::ChooseModel(&arg1);

                /* 
                * arg1 = 13 result = 523 HPV1000 and 598 Police LV
                * arg1 = 17 result = 439 Stallion
                */

                if (arg1 == 17) {
                    spdlog::info("17 MODEL_ID: {} ARG1: {}", result, arg1); // result = 523 HPV1000
                }

                if (arg1 == 13) {
                    spdlog::info("13 MODEL_ID: {} ARG1: {}", result, arg1); // result = 523 HPV1000
                }

                if (arg1 == 17) {
                    spdlog::info("13 MODEL_ID: {} ARG1: {}", result, arg1); // result = 523 HPV1000
                }

                if (arg1 == 24) {
                    spdlog::info("24 MODEL_ID: {} ARG1: {}", result, arg1); // result = 523 HPV1000
                }

                if (arg1 == 25) {
                    spdlog::info("25 MODEL_ID: {} ARG1: {}", result, arg1); // result = 523 HPV1000
                }
            }

            if (FindPlayerPed(-1) && plugin::KeyPressed(VK_F6) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
                auto player = FindPlayerPed(-1);
                auto playerPos = player->GetPosition();

                RenderLineNoClipping(playerPos + CVector{ 0.f, 0.f, 0.5f }, playerPos - CVector{ 10.f, 0.f, 0.5f }, 0xFF0000, 0x00FF00);

                //auto FindNodeClosestToCoors = plugin::CallMethodAndReturn<CNodeAddress, 0x44F460 + 0x562629, CPathFind *, CVector, uint8_t, float, uint16_t, int32_t, uint16_t, uint16_t, int32_t>;
                //auto closestNodeAddr = FindNodeClosestToCoors(&ThePaths, playerPos, 1, 300.0f, 0, 0, 0, 0, 0);

                //uint8_t pathType = 1; // ePathType::PATH_TYPE_PED
                //ThePaths.DoPathSearch(static_cast<unsigned char>(pathType), playerPos);
            }

            if (FindPlayerPed(-1) && plugin::KeyPressed(VK_F7) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
                keyPressTime = CTimer::m_snTimeInMilliseconds;
                // auto* pVehicle = FindPlayerVehicle(-1, false);
                CVector position = FindPlayerPed(-1)->TransformFromObjectSpace(CVector(0.0f, 5.0f, 0.0f));
                auto pVehicle = CCarCtrl::CreateCarForScript(MODEL_SULTAN, position, false);
                CNodeAddress node_addr1;
                CNodeAddress node_addr2;
                CCarCtrl::FindNodesThisCarIsNearestTo(pVehicle, node_addr1, node_addr2);
                auto node1 = ThePaths.GetPathNode(node_addr1);
                auto node2 = ThePaths.GetPathNode(node_addr2);
                m_node1_pos = node1->GetNodeCoors();
                m_node2_pos = node2->GetNodeCoors();
                // plugin::CallMethod<0x44F8C0, CPathFind, CVector, uint8_t, float, bool, bool, int32_t, bool, bool, CNodeAddress*>(ThePaths, position, );

                if (pVehicle) {
                    //spdlog::info("joining player car into the road system. node1: {} {} {}", node1_pos.x, node1_pos.y, node1_pos.z);

                    pVehicle->m_autoPilot.m_nCarMission = MISSION_GOTOCOORDS_ACCURATE;
                    pVehicle->m_autoPilot.m_nCarDrivingStyle = eCarDrivingStyle::DRIVINGSTYLE_STOP_FOR_CARS;
                    pVehicle->m_autoPilot.m_nCruiseSpeed = 13;
                    pVehicle->m_autoPilot.m_nCurrentLane = 0;
                    pVehicle->m_autoPilot.m_nNextLane = 0;
                    pVehicle->m_autoPilot.m_vecDestinationCoors = { 2433.15f, -1517.75f, 23.83f };
                    //CCarCtrl::JoinCarWithRoadAccordingToMission(pVehicle);
                    //CCarCtrl::JoinCarWithRoadSystem(pVehicle);
                    spdlog::info("AREA ID {}", pVehicle->m_autoPilot.m_currentAddress.m_nAreaId);
                    auto result = CCarCtrl::JoinCarWithRoadSystemGotoCoors(pVehicle, { 2433.15f, -1517.75f, 23.83f }, false, false);
                    spdlog::info("AREA ID {} result: {}", pVehicle->m_autoPilot.m_currentAddress.m_nAreaId, result);
                }
            }

            if (FindPlayerPed() && plugin::KeyPressed(VK_F8) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
                keyPressTime = CTimer::m_snTimeInMilliseconds;

                for (int i = 0; i < 39; ++i) {
                    auto zone = CTheZones::MapZoneArray[i];
                    auto zoneName = zone.GetTranslatedName();

                    spdlog::info("AQUI: {0:x}", *zoneName);
                }
            }

            if (FindPlayerPed() && plugin::KeyPressed(VK_F9) && CTimer::m_snTimeInMilliseconds - keyPressTime > 500) {
                keyPressTime = CTimer::m_snTimeInMilliseconds;
                int modelID = pedModelIds[rand() % 250];
                CStreaming::RequestModel(modelID, 0);
                CStreaming::LoadAllRequestedModels(false);
                CPed* ped = new CCivilianPed(CPopulation::IsFemale(modelID) ? PED_TYPE_CIVFEMALE : PED_TYPE_CIVMALE, modelID);
                if (ped) {
                    ped->SetPosn(FindPlayerPed()->TransformFromObjectSpace(CVector(0.0f, 5.0f, 3.0f)));
                    ped->SetOrientation(0.0f, 0.0f, 0.0f);
                    CWorld::Add(ped);
                    ped->PositionAnyPedOutOfCollision();
                    ped->m_pIntelligence->m_TaskMgr.SetTask(new CTaskComplexWanderStandard(4, rand() % 8, true), 4, false);
                }
            }

            if (auto* player = FindPlayerPed()) {
                if (auto targetedPed = player->m_pPlayerTargettedPed) {
                    m_lastTargetedPedByPlayer = targetedPed;
                    spdlog::info("COISAS 2");
                }
            }

            auto handleWanderTask = [&](CTaskComplexWander* task) -> void {
                if (task == nullptr) {
                    return;
                }

                if (task->m_LastNode.m_nAreaId != (uint16_t)-1 && !task->m_LastNode.IsEmpty()) {
                    CPathNode& lastPathNode = ThePaths.m_pPathNodes[task->m_LastNode.m_nAreaId][task->m_LastNode.m_nNodeId];
                    auto       nodePos = lastPathNode.GetNodeCoors();
                    auto       color = CRGBA(0, 255, 0, 255).ToIntARGB();
                    //RenderLineNoClipping(nodePos + CVector{ 0.f, 0.f, 0.5f }, nodePos - CVector{ 0.f, 0.f, 0.5f }, color, color);
                    //Draw2DTextOnWorld("LAST_NODE", nodePos, {255, 0, 0});
                }

                if (task->m_NextNode.m_nAreaId != (uint16_t)-1 && !task->m_NextNode.IsEmpty()) {
                    CPathNode& nextPathNode = ThePaths.m_pPathNodes[task->m_NextNode.m_nAreaId][task->m_NextNode.m_nNodeId];
                    auto       nodePos = nextPathNode.GetNodeCoors();
                    auto       color = CRGBA(0, 0, 255, 255).ToIntARGB();
                    //RenderLineNoClipping(nodePos + CVector{ 0.f, 0.f, 0.5f }, nodePos - CVector{ 0.f, 0.f, 0.5f }, color, color);
                    //Draw2DTextOnWorld("NEXT_NODE", nodePos, {0, 0, 255});
                }

                if (task->m_LastNode.m_nAreaId != (uint16_t)-1 && !task->m_LastNode.IsEmpty() && task->m_NextNode.m_nAreaId != (uint16_t)-1 && !task->m_NextNode.IsEmpty()) {
                    CPathNode& lastPathNode = ThePaths.m_pPathNodes[task->m_LastNode.m_nAreaId][task->m_LastNode.m_nNodeId];
                    auto       lastNodePos = lastPathNode.GetNodeCoors() + CVector{ 0.f, 0.f, 0.4f };
                    CPathNode& nextPathNode = ThePaths.m_pPathNodes[task->m_NextNode.m_nAreaId][task->m_NextNode.m_nNodeId];
                    auto       nextNodePos = nextPathNode.GetNodeCoors() + CVector{ 0.f, 0.f, 0.4f };
                    auto       startColor = CRGBA(0, 255, 0, 255).ToIntARGB();
                    auto       endColor = CRGBA(0, 0, 255, 255).ToIntARGB();
                    //RenderLineNoClipping(lastNodePos, nextNodePos, startColor, endColor);
                }
            };

            auto handleTask = [&](CTask* task) -> void {
                if (task && task->GetId() != eTaskType::TASK_COMPLEX_WANDER) {
                    return;
                }

                handleWanderTask(reinterpret_cast<CTaskComplexWander*>(task));
            };

            if (m_lastTargetedPedByPlayer) {
                if (auto pedIntelligence = m_lastTargetedPedByPlayer->m_pIntelligence) {
                    std::vector<CTask*> pedTasks(7);

                    for (auto secTask : pedIntelligence->m_TaskMgr.m_aPrimaryTasks) {
                        pedTasks.push_back(secTask);
                    }
                    
                    for (auto secTask : pedIntelligence->m_TaskMgr.m_aSecondaryTasks) {
                        pedTasks.push_back(secTask);
                    }

                    for (auto task : pedTasks) {
                        handleTask(task);
                    }
                }
            }


            // m_theInputMgr.ProcessInput();

            /*
            auto player = FindPlayerPed();
            if (player) {
                player->m_bUsesCollision = false;
            }
             */

            //auto cameraTargetPoint = TheCamera.m_vecMoveLinear;

            if (IsKeyPressed('W')) {
            /*
                if (player) {
                    const auto cameraPosition = TheCamera.GetGameCamPosition();
                    const auto cameraDirection = TheCamera.GetLookDirection();
                    auto cameraTargetPoint = TheCamera.m_vecAimingTargetCoors;

                    cameraTargetPoint.x -= cameraPosition->x;
                    cameraTargetPoint.y -= cameraPosition->x;
                    cameraTargetPoint.z -= cameraPosition->z;

                    cameraTargetPoint.x *= 0.5f;
                    cameraTargetPoint.y *= 0.5f;
                    cameraTargetPoint.z *= 0.5f;

                    player->SetPosn(cameraTargetPoint);
                }
             */
            }
            
            if (IsKeyPressed('S')) {
            /*
                if (player) {
                    const auto cameraPosition = TheCamera.GetGameCamPosition();
                    const auto cameraDirection = TheCamera.GetLookDirection();
                    auto cameraTargetPoint = TheCamera.m_vecTargetWhenInterPol;

                    cameraTargetPoint.x -= cameraPosition->x;
                    cameraTargetPoint.y -= cameraPosition->x;
                    cameraTargetPoint.z -= cameraPosition->z;

                    cameraTargetPoint.x *= -1.0f;
                    cameraTargetPoint.y *= -1.0f;
                    cameraTargetPoint.z *= -1.0f;

                    player->SetPosn(cameraTargetPoint);
                }
             */
            }

            // spdlog::info("CAMERA_POS: {} {} {}", cameraPosition->x, cameraPosition->y, cameraPosition->z);
            // spdlog::info("CAMERA_DIRECTION: {}", cameraDirection);
            // spdlog::info("camera_direction: {} {} {}", cameraTargetPoint.x, cameraTargetPoint.y, cameraTargetPoint.z);



            // IS ANSWERING CALL
            // spdlog::info("Calls answered {0:x}", CTheScripts::ScriptSpace[4 * 1414]);

            //spdlog::info("ON_MISSION: {0:x}", CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag]);
            // spdlog::info("RECAL_TIME: {}", reinterpret_cast<int>(&CTheScripts::ScriptSpace[1630]));
        };

        ApplyPatches();
        ApplyHooks();

        plugin::Events::drawHudEvent += [this]() -> void {
            //CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 0;
            //spdlog::info("onGameProcessEvent > ON_MISSION: {0:x}", CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag]);

            //Draw2DTextOnWorld("node1", m_node1_pos, { 0, 0, 255 });
            //Draw2DTextOnWorld("node2", m_node2_pos, { 0, 0, 255 });
            for (auto vehicle : CPools::ms_pVehiclePool) {
                Draw2DTextOnWorld(std::string(std::format("{}", vehicle->m_nModelIndex)), vehicle->GetPosition(), {0, 0, 255});
            }
        };
	};

    static void RenderLineNoClipping(const CVector& start, const CVector& end, uint32_t startColor, uint32_t endColor)
    {
        spdlog::info("RenderLineNoClipping()");
        return plugin::Call<0x6FF460, float, float, float, float, float, float, uint32_t, uint32_t>(start.x, start.y, start.z, end.x, end.y, end.z, startColor, endColor);
    }

    static bool IsKeyPressed(int32_t virtualKey)
    {
        return GetKeyState(virtualKey) & 0x8000;
    }

    void ApplyPatches()
    {
        plugin::patch::Set<BYTE>(0x5519E5, 0x90);
        plugin::patch::Set<BYTE>(0x5519E6, 0x90);
        plugin::patch::Set<BYTE>(0x5519E7, 0x90);
        plugin::patch::Set<BYTE>(0x5519E8, 0x90);
        plugin::patch::Set<BYTE>(0x5519E9, 0x90);
        //plugin::patch::RedirectCall(0x5519E5, TestRedirect);
        //plugin::patch::RedirectCall(0x53C1C1, GenerateRandomCars);
        plugin::patch::RedirectCall(0x43022A, ChooseVehicleModel);

    }

    static int MyOwnChooseVehicleModel(int* arg1)
    {
        int32_t var_4;
        auto numAllCars = static_cast<int>(CPopCycle::m_NumOther_Cars + CPopCycle::m_NumCops_Cars + CPopCycle::m_NumGangs_Cars + CPopCycle::m_NumDealers_Cars);

        if (CCheat::m_aCheatsActive[CHEAT_REDUCED_TRAFFIC]) {
            var_4 = CGeneral::GetRandomNumber() * 0.000030517578f * 100.0f;
        }
        else {
            if ((CGeneral::GetRandomNumber() * 0.000030518509f) > (CPopCycle::m_NumDealers_Cars / numAllCars) || CCheat::m_aCheatsActive[CHEAT_GANGS_CONTROLS_THE_STREETS]) {
                if (CPopulation::m_bDontCreateRandomGangMembers) {
                    return -1;
                }


            }
            else
            {
                *arg1 = 25;
                spdlog::info("MyOwnChooseVehicleModel() VINTE_CINCO {}", numAllCars);
                return -1;
            }
        }

        
        return eModelID::MODEL_FBITRUCK;
    }

    static int ChooseVehicleModel(int* arg1)
    {
        int result = CCarCtrl::ChooseModel(arg1);
        MyOwnChooseVehicleModel(arg1);
        return result;
    }

    void ApplyHooks()
    {
        static plugin::ThiscallEvent <plugin::AddressList<0x4C3BFB, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPick<plugin::ArgTypes<CEventHandler*, void*, CTask*, CTask*>, 0, 1, 2, 3>, void(CEventHandler*, void*, CTask*, CTask*)> onEventVehicleOnFire;
        onEventVehicleOnFire += [&](CEventHandler*, void*, CTask*, CTask*) -> void {
            spdlog::info("onEventVehicleOnFire()");
        };

        /*
        static plugin::ThiscallEvent <plugin::AddressList<0x4C0BD0, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPick<plugin::ArgTypes<CEventHandler*, void*, CTask*, CTask*>, 0, 1, 2, 3>, void(CEventHandler*, void*, CTask*, CTask*)> onComputeVehiclePotentialCollisionEvent;
        onComputeVehiclePotentialCollisionEvent += [&](CEventHandler*, void*, CTask*, CTask*) -> void {
            spdlog::info("onComputeVehiclePotentialCollisionEvent()");
        };
        */

        static plugin::CdeclEvent <plugin::AddressList<0x464C20, plugin::H_JUMP>, plugin::PRIORITY_BEFORE, plugin::ArgPickN<unsigned char*, 0>, void* (unsigned char*)> onStartNewScript;
        onStartNewScript += [](unsigned char* startIP) -> void {
            auto runningScript = reinterpret_cast<CRunningScript*>(startIP);
            // spdlog::info("onStartNewScript > Starting Script: {}", TheText.Get(runningScript->m_szName));
        };

        static plugin::CdeclEvent <plugin::AddressList<0x53C1C1, plugin::H_CALL>, plugin::PRIORITY_BEFORE, plugin::ArgPickNone, void()> onGenerateRandomCars;
        onGenerateRandomCars += [&]() -> void {
            // spdlog::info("timeNextMadDriverChaseCreated: {}", plugin::patch::GetFloat(0x9690BC));
            // spdlog::info("timeStep: {}", CTimer::ms_fTimeStep);
            // spdlog::info("cutsceneTimer: {}", CCutsceneMgr::ms_cutsceneTimer);
        };

        static plugin::ThiscallEvent <plugin::AddressList<0x452760, plugin::H_JUMP>, plugin::PRIORITY_AFTER, plugin::ArgPick6N<CPathFind*, 0, uint8_t, 1, const CVector&, 2, const CVector&, 3, const CNodeAddress&, 4, CNodeAddress*, 5>, void(CPathFind*, uint8_t, const CVector&, const CVector&, const CNodeAddress&, CNodeAddress*)> onComputeRoute;
        onComputeRoute += [&](CPathFind*, uint8_t nodeType, const CVector& vecStart, const CVector& vecEnd, const CNodeAddress& startAddress, CNodeAddress* route) -> void {
            spdlog::info("onComputeRoute()");

            auto* pathNode = &ThePaths.m_pPathNodes[route->m_nAreaId][route->m_nNodeId];

            if (pathNode) {
                spdlog::info("onComputeRoute() NOT NULL");
                auto* next = reinterpret_cast<CPathNode*>(pathNode->ptr);
                auto* previous = reinterpret_cast<CPathNode*>(pathNode->ptr2);
            }
        };

        static plugin::CdeclEvent <plugin::AddressList<0x46EBBE, plugin::H_CALL>, plugin::PRIORITY_AFTER, plugin::ArgPickN<uint32_t, 0>, void(uint32_t)> onGetShoppingPrice;
        onGetShoppingPrice.Add(SA_Debugger::onGetShoppingPrice);

        /*
        static plugin::ThiscallEvent<
            plugin::AddressList<0x424454, plugin::H_CALL>,
            plugin::PRIORITY_AFTER,
            plugin::ArgPick<
                plugin::ArgTypes<CPathFind*, CNodeAddress*, CVector, uint8_t, float, int16_t, int32_t, uint16_t, uint16_t>, 0, 1, 2, 3, 4, 5, 6, 7, 8
            >,
            CNodeAddress(CPathFind*, CNodeAddress*, CVector, uint8_t, float, int16_t, int32_t, uint16_t, uint16_t)
        > onFindNodeClosestToCoors;
        onFindNodeClosestToCoors += [](CPathFind* __this, CNodeAddress*, CVector, uint8_t, float, int16_t, int32_t, uint16_t, uint16_t) {
            spdlog::info("graças a deus");
        };
        */
    }

    static void onGetShoppingPrice(uint32_t index)
    {
        spdlog::info("onGetShoppingPrice() index: {}", index);
    }

    static void TestRedirect(const char* str)
    {
        spdlog::info("HELLO WORLD {}", str);
    }

    static void GenerateRandomCars()
    {
        bool bIsCutsceneRunning = CCutsceneMgr::ms_running;
        if (bIsCutsceneRunning) {
            // CCarCtrl::CountDownToCarsAtStart = 2
            plugin::patch::SetInt(0x9690C0, 2);
            return;
        }

        // check if a gang war is happening
        // _isGangWarActive
        if (plugin::CallAndReturn<bool, 0x4439C0>()) {
            return;
        }

        // check if player is not in any interior
        if (CGame::currArea != 0) {
            return;
        }

        // CCarCtrl::TimeNextMadDriverChaseCreated
        float timeNextMadDriverChaseCreated = plugin::patch::GetFloat(0x9690BC);

        if (!CGameLogic::LaRiotsActiveHere()) {
            // TODO: AQUI PRECISA SER REVISADO
            if (timeNextMadDriverChaseCreated <= 0 || timeNextMadDriverChaseCreated == 480.0f) {
                float NewTimeNextMadDriverChaseCreated = CGeneral::GetRandomNumberInRange(480.0f, 240.0f);
                plugin::patch::SetFloat(0x9690BC, NewTimeNextMadDriverChaseCreated);
            }
        }

        float timeStep = CTimer::ms_fTimeStep;
        // CCarCtrl::NumRandomCars
        int32_t numRandomCars = plugin::patch::GetInt(0x969094);

        float NewTimeNextMadDriverChaseCreated = (timeStep * CCutsceneMgr::ms_cutsceneTimer) - timeNextMadDriverChaseCreated;
        plugin::patch::SetFloat(0x9690BC, NewTimeNextMadDriverChaseCreated);

        if (!(numRandomCars >= 45)) {
            // CCarCtrl::CountDownToCarsAtStart
            int8_t CountDownToCarsAtStart = plugin::patch::Get<int8_t>(0x9690C0);

            if (!CountDownToCarsAtStart) {
                CCarCtrl::GenerateOneRandomCar();
                CCarCtrl::GenerateOneRandomCar();
            }
            else {
                CountDownToCarsAtStart--;
                plugin::patch::Set<int8_t>(0x9690C0, CountDownToCarsAtStart);

                for (int i = 0; i <= 100; i++) {
                    CCarCtrl::GenerateOneRandomCar();
                }

                // CTheCarGenerators::GenerateEvenIfPlayerIsCloseCounter
                plugin::patch::Set<int8_t>(0xC279D0, 20);
            }
        }

        CTrain::DoTrainGenerationAndRemoval();
        CPlane::DoPlaneGenerationAndRemoval();

        int previousTimeInMilliseconds = CTimer::m_snPreviousTimeInMilliseconds;
        int timeInMilliseconds = CTimer::m_snTimeInMilliseconds;

        // TODO: Make this code more human readable
        if (((previousTimeInMilliseconds - timeInMilliseconds) & 0xFFFFF000) != 0) {
            return;
        }

        CCarCtrl::GenerateEmergencyServicesCar();
    }

    void OnKeyPress(unsigned char key, PressState state)
    {
        if (state == PRESS_DOWN) {
            switch (key)
            {
            case DIK_MINUS:
            {
                spdlog::info("ME APERTARAM!!");
                auto player = FindPlayerPed();
                if (player) {
                    auto pVehicle = player->m_pVehicle;
                    if (pVehicle) {
                        spdlog::info("Calling function!!");
                        MessageBox(0, "CHAMANDO FUNCAO", "Error", MB_OK);
                        plugin::Call<0x42F870, CVehicle*, const CVector&, bool, bool > (pVehicle, {0,0,0}, false, false);
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }

    void Draw2DTextOnWorld(const std::string text, CVector& posn, CRGBA color)
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
            CFont::SetScale(0.5, 0.5);
            CFont::SetFontStyle(FONT_SUBTITLES);
            CFont::SetProportional(true);
            CFont::PrintString(screenCoors.x, screenCoors.y, text.c_str());
        }
    }

private:
    // InputManager m_theInputMgr;
} SA_DebuggerPlugin;
