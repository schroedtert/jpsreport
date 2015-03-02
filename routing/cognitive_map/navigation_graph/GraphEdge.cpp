/**
 * \file        GraphEdge.cpp
 * \date        Jan 1, 2014
 * \version     v0.6
 * \copyright   <2009-2014> Forschungszentrum Jülich GmbH. All rights reserved.
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


#include "GraphEdge.h"

#include<vector>

#include "GraphVertex.h"
#include "../../../geometry/SubRoom.h"
#include "../../../geometry/Crossing.h"
#include "../../../geometry/Transition.h"

using namespace std;



/**
 * Constructors & Destructors
 */

GraphEdge::~GraphEdge()
{
     return;
}

GraphEdge::GraphEdge(const GraphVertex * const s, const GraphVertex  * const d, const Crossing * const crossing)
     : _src(s), _dest(d), _crossing(crossing)
{
     CalcApproximateDistance();
     SetUpFireMesh();
}

GraphEdge::GraphEdge(GraphEdge const &ge)
     : _src(ge._src), _dest(ge._dest), _crossing(ge._crossing), _approximate_distance(ge._approximate_distance), _fireMesh(ge._fireMesh)
{
}

void GraphEdge::CalcApproximateDistance()
{
     double distance = 0.0;
     int count = 0;
     for(std::vector<Crossing*>::const_iterator it = _src->GetSubRoom()->GetAllCrossings().begin(); it != _src->GetSubRoom()->GetAllCrossings().end(); ++it) {
          if(_crossing->GetUniqueID() == (*it)->GetUniqueID()) continue;
          if(GetDest() != NULL && ((*it)->GetSubRoom1() == GetDest()->GetSubRoom() || (*it)->GetSubRoom2() == GetDest()->GetSubRoom())) continue;
          count++;
          distance = distance + (((*it)->GetCentre() - _crossing->GetCentre()).Norm());
     }

     for(std::vector<Transition*>::const_iterator it = _src->GetSubRoom()->GetAllTransitions().begin(); it != _src->GetSubRoom()->GetAllTransitions().end(); ++it) {
          if(_crossing->GetUniqueID() == (*it)->GetUniqueID()) continue;
          if(GetDest() != NULL && ((*it)->GetSubRoom1() == GetDest()->GetSubRoom() || (*it)->GetSubRoom2() == GetDest()->GetSubRoom())) continue;
          count++;
          distance = distance + (((*it)->GetCentre() - _crossing->GetCentre()).Norm());
     }
     if(count == 0) _approximate_distance = 0;
     else _approximate_distance = distance/count;
}


double GraphEdge::GetWeight(const Point & position) const
{
    if(factors.empty()) {
        return GetApproximateDistance(position);
    }
    //double weight = GetFactorWithDistance(GetApproximateDistance(position));
    double weight = GetApproximateDistance(position) * GetFactor();
    return weight;
}

double GraphEdge::GetFactor() const
{
    double factor = 1.0;
    for(FactorContainer::const_iterator it = factors.begin(); it != factors.end(); ++it) {
        factor = factor * it->second.first;
    }
    return factor;
}

double GraphEdge::GetFactorWithDistance(double distance) const
{
    double factor = distance;
    for(FactorContainer::const_iterator it = factors.begin(); it != factors.end(); ++it) {
        factor = factor + distance * it->second.first;
    }
    return factor;
}

void GraphEdge::SetFactor(double factor, std::string name)
{
     //TODO: set global time as second double
     factors[name] = std::make_pair(factor, 0.0);
}



/**
 * GETTER AND SETTER
 */

double GraphEdge::GetRoomToFloorFactor() const
{
     if(GetDest() == NULL || GetDest()->GetSubRoom()->GetType() == GetSrc()->GetSubRoom()->GetType()) return 1.0;
     if(GetDest()->GetSubRoom()->GetType() == "floor") return 1.0;
     else return 5.0;
}

double GraphEdge::GetApproximateDistance(const Point & position) const
{
     return (_crossing->GetCentre()-position).Norm();
}

double GraphEdge::GetApproximateDistance() const
{
     return _approximate_distance;
}

const GraphVertex * GraphEdge::GetDest() const
{
     return _dest;
}
const GraphVertex * GraphEdge::GetSrc() const
{
     return _src;
}

const Crossing * GraphEdge::GetCrossing() const
{
    return _crossing;
}

const FireMesh &GraphEdge::GetFireMesh() const
{
    return _fireMesh;
}

void GraphEdge::SetUpFireMesh()
{

     //TODO: read this prefix from the ini file
     // std::string filename = "D:/workspace/JuPedSim/jpscore/inputfiles/cognitive_map";
     std::string prefix = "D:/workspace/JPS/JPScore/inputfiles/cognitive_map";


     std::string filename = prefix + "/Door_X_" + std::to_string(_crossing->GetCentre().GetX())
               + "_Y_" + std::to_string(_crossing->GetCentre().GetY()) + ".csv";
     _fireMesh.SetKnotValuesFromFile(filename);
}

double GraphEdge::GetSmokeFactor(const Point &pointPed) const
{   //std::cout << pointPed.GetX() << " " << pointPed.GetY() << std::endl;
    if (_fireMesh.statusMesh()==true)
        return _fireMesh.GetKnotValue(pointPed.GetX(),pointPed.GetY());
    else
        return 1.0;

}

bool GraphEdge::IsExit() const
{
     return _crossing->IsExit();
}
