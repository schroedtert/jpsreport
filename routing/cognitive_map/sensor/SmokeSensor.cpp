/**
 * @file SmokeSensor.cpp
 * @author David H. <d.haensel@fz-juelich.de>
 * @date 05, 2014
 * @brief
 */

#include "SmokeSensor.h"
#include "../NavigationGraph.h"
#include "../../../geometry/Building.h"
#include "../CognitiveMap.h"
#include "../../../pedestrian/Pedestrian.h"
#include "../../../geometry/SubRoom.h"
#include "../fire_mesh/FireMesh.h"
#include "../fire_mesh/FireMeshStorage.h"
#include <set>

SmokeSensor::SmokeSensor(const Building *b, const &filepath, const &updateintervall, const &finalTime) : AbstractSensor(b)
{

    _FMStorage= new FireMeshStorage(b,filepath,updateintervall,finalTime);
}

SmokeSensor::~SmokeSensor()
{
}

std::string SmokeSensor::GetName() const
{
    return "SmokeSensor";
}

void SmokeSensor::execute(const Pedestrian * pedestrian, CognitiveMap * cognitive_map) const
{
    SubRoom * sub_room = building->GetRoom(pedestrian->GetRoomID())->GetSubRoom(pedestrian->GetSubRoomID());
    GraphVertex * vertex = (*cognitive_map->GetNavigationGraph())[sub_room];
    const GraphVertex::EdgesContainer * edges = vertex->GetAllEdges();

    for (auto &item : *edges)
    {
        double smokeFactor = item->GetSmokeFactor(pedestrian->GetPos());
        //std::cout << smokeFactor << std::endl;
        item->SetFactor(smokeFactor,GetName());
        //std::cout << item->GetFactor() << std::endl;
    }

//    const GraphVertex * smoked_room = NULL;

//    for(GraphVertex::EdgesContainer::const_iterator it = edges->begi           bn(); it != edges->end(); ++it) {
//        if(
//            (*it)->GetDest()  != NULL
//            && (
//                (*it)->GetDest()->GetSubRoom()->GetRoomID() == 14 //insert room id here
//                && (*it)->GetDest()->GetSubRoom()->GetSubRoomID() == 7 //insert sub room id here
//                )
//            ) {
//            smoked_room =(*it)->GetDest();
//        }
//    }

//    if(smoked_room != nullptr) {
//        NavigationGraph::VerticesContainer * vertices = cognitive_map->GetAllVertices();

//        //iterating over all Vertices
//        for(NavigationGraph::VerticesContainer::iterator it_vertices = vertices->begin(); it_vertices != vertices->end(); ++it_vertices) {
//            GraphVertex::EdgesContainer * edges = (it_vertices->second)->GetAllEdges();
//            //iterating over all edges
//            for(GraphVertex::EdgesContainer::iterator it_edges = edges->begin(); it_edges != edges->end(); ++it_edges) {
//                if((*it_edges)->GetDest() == smoked_room) {
//                    (*it_edges)->SetFactor(10000.0, GetName());
//                }
//            }
//        }
//    }

}

void SmokeSensor::set_FMStorage(const&fmStorage)
{
    _FMStorage=fmStorage;

}

const FireMeshStorage *SmokeSensor::get_FMStorage()
{
    return _FMStorage;

}


