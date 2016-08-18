/**
 * \file        CognitiveMapRouter.cpp
 * \date        Feb 1, 2014
 * \version     v0.7
 * \copyright   <2009-2015> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 *
 *
 **/


#include "CognitiveMapRouter.h"
#include "Router.h"

#include "cognitive_map/BrainStorage.h"
#include "cognitive_map/cognitiveMap/cognitivemap.h"
#include "cognitive_map/NavigationGraph.h"
#include "cognitive_map/sensor/SensorManager.h"

#include "../geometry/SubRoom.h"
#include "../geometry/NavLine.h"
#include "../geometry/Building.h"

#include "../pedestrian/Pedestrian.h"
#include "../tinyxml/tinyxml.h"

CognitiveMapRouter::CognitiveMapRouter()
{
    building=nullptr;
//    cm_storage=nullptr;
//    sensor_manager=nullptr;

}

CognitiveMapRouter::CognitiveMapRouter(int id, RoutingStrategy s) : Router(id, s)
{
    building=nullptr;
//    cm_storage=nullptr;
//    sensor_manager=nullptr;
}

CognitiveMapRouter::~CognitiveMapRouter()
{
     delete brain_storage;
     delete sensor_manager;

}

int CognitiveMapRouter::FindExit(Pedestrian * p)
{
    //check for former goal.
    if((*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->HadNoDestination()) {
        sensor_manager->execute(p, SensorManager::INIT);
    }

    //Check if the Pedestrian already has a Dest. or changed subroom and needs a new one.
    if((*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->ChangedSubRoom()) {
        //execute periodical sensors
        sensor_manager->execute(p, SensorManager::CHANGED_ROOM);

        int status = FindDestination(p);

        (*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->UpdateSubRoom();

        return status;
    }

    //std::cout << p->GetGlobalTime() << std::endl;
    if (std::fmod(p->GetGlobalTime(),sensor_manager->GetIntVPeriodicUpdate())==0.0 && p->GetGlobalTime()>0)
    {
        //Log->Write(std::to_string(p->GetGlobalTime()));
        sensor_manager->execute(p, SensorManager::PERIODIC);

        int status = FindDestination(p);

        //(*cm_storage)[p]->UpdateSubRoom();

        return status;

    }
    return 1;
}

int CognitiveMapRouter::FindDestination(Pedestrian * p)
{
        // Discover doors
        sensor_manager->execute(p, SensorManager::NO_WAY);
        //check if there is a way to the outside the pedestrian knows (in the cognitive map)
        const GraphEdge * destination = nullptr;
        //Cognitive Map /Associations/ Waypoints/ landmarks

//        (*cm_storage)[p]->UpdateMap();

        //--------------------COGMAP----------------------------
        //See if Landmarks are visible

        (*brain_storage)[p]->GetCognitiveMap().UpdateMap();
        //Find next appropriate landmark
        (*brain_storage)[p]->GetCognitiveMap().FindNextTarget();
        //Find appropriate door to reach next app. landmark
        (*brain_storage)[p]->GetCognitiveMap().AssessDoors();
        //------------------------------------------------------

        //Log->Write(std::to_string((*cm_storage)[p]->GetOwnPos().GetX())+" "+std::to_string((*cm_storage)[p]->GetOwnPos().GetY()));

        destination = (*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->GetLocalDestination();
        if(destination == nullptr) {
            //no destination was found, now we could start the discovery!
            //1. run the no_way sensors for room discovery.
            sensor_manager->execute(p, SensorManager::NO_WAY);

            //check if this was enough for finding a global path to the exit

            destination = (*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->GetDestination();

            if(destination == nullptr) {
                //we still do not have a way. lets take the "best" local edge
                //for this we don't calculate the cost to exit but calculate the cost for the edges at the actual vertex.
                destination = (*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->GetLocalDestination();
            }
        }

        //if we still could not found any destination we are lost! Pedestrian will be deleted
        //no destination should just appear in bug case or closed rooms.
        if(destination == nullptr) {
            Log->Write("ERROR: \t One Pedestrian (ID: %i) was not able to find any destination", p->GetID());
            return -1;
        }

        (*brain_storage)[p]->GetCognitiveMap().GetGraphNetwork()->AddDestination(destination);
        sensor_manager->execute(p, SensorManager::NEW_DESTINATION);

        //setting crossing to ped
        p->SetExitLine(destination->GetCrossing());
        p->SetExitIndex(destination->GetCrossing()->GetUniqueID());

        return 1;
}



bool CognitiveMapRouter::Init(Building * b)
{
     Log->Write("INFO:\tInit the Cognitive Map Router Engine");
     building = b;

     //Init Cognitive Map Storage, second parameter: decides whether cognitive Map is empty or complete
     if (getOptions().find("CognitiveMapFiles")==getOptions().end())
        brain_storage = new BrainStorage(building,getOptions().at("CognitiveMap")[0]);
     else
        brain_storage = new BrainStorage(building,getOptions().at("CognitiveMap")[0],getOptions().at("CognitiveMapFiles")[0]);
     Log->Write("INFO:\tCognitiveMapStorage initialized");
     //cm_storage->ParseCogMap();

     //Init Sensor Manager
     //sensor_manager = SensorManager::InitWithAllSensors(b, cm_storage);
     sensor_manager = SensorManager::InitWithCertainSensors(b, brain_storage, getOptions());
     Log->Write("INFO:\tSensorManager initialized");
     return true;
}


const optStorage &CognitiveMapRouter::getOptions() const
{
    return options;
}

void CognitiveMapRouter::addOption(const std::string &key, const std::vector<std::string> &value)
{
    options.insert(std::make_pair(key, value));
}

