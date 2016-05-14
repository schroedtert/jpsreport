/**
 * \file        PedDistributor.cpp
 * \date        Oct 12, 2010
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


#include <cmath>
#include "PedDistributor.h"
#include "../tinyxml/tinyxml.h"
#include "../geometry/Obstacle.h"
#include "../routing/RoutingEngine.h"
#include "../pedestrian/Pedestrian.h"
#include "../geometry/SubRoom.h"
#include "../IO/OutputHandler.h"
#include "../IO/PedDistributionParser.h"
#include "PedDistributionLoader.h"
#include "../hybrid/PedDistributionFromProtobufLoader.h"

#include "../JPSfire/B_walking_speed/WalkingSpeed.h"


using namespace std;

/************************************************************
 PedDistributor
 ************************************************************/

PedDistributor::PedDistributor(const Configuration *configuration) : _configuration(configuration) {
    _start_dis = vector<std::shared_ptr<StartDistribution> >();
    _start_dis_sub = vector<std::shared_ptr<StartDistribution> >();
    _start_dis_sources = vector<std::shared_ptr<AgentsSource> >();


    PedDistributionLoader * parser;

#ifdef _JPS_AS_A_SERVICE
    if(_configuration->GetRunAsService()) {
        parser = new PedDistributionFromProtobufLoader(_configuration);

    } else
#endif
    {
        parser = new PedDistributionParser(_configuration);
    }

    parser->LoadPedDistribution(_start_dis,_start_dis_sub,_start_dis_sources);

}


PedDistributor::~PedDistributor() {
//     for (unsigned int i = 0; i < _start_dis.size(); i++)
//     {
//          delete _start_dis[i];
//     }
//     for (unsigned int i = 0; i < _start_dis_sub.size(); i++)
//     {
//          delete _start_dis_sub[i];
//     }

//     _start_dis_sub.clear();
//     _start_dis.clear();
}

const vector<std::shared_ptr<AgentsSource> > &PedDistributor::GetAgentsSources() const {
    return _start_dis_sources;
}


bool PedDistributor::Distribute(Building *building) const {
    Log->Write("INFO: \tInit Distribute");
    int nPeds_is = 0;
    int nPeds_expected = 0;

    // store the position in a map since we are not computing for all rooms/subrooms.
    std::map<int, std::map<int, vector<Point> > > allFreePos;

    //collect the available positions for that subroom
    for (const auto &dist: _start_dis_sub) {
        int roomID = dist->GetRoomId();
        Room *r = building->GetRoom(roomID);
        if (!r) return false;

        int subroomID = dist->GetSubroomID();
        SubRoom *sr = r->GetSubRoom(subroomID);
        if (!sr) return false;

        auto &allFreePosRoom = allFreePos[roomID];
        // the positions were already computed
        if (allFreePosRoom.count(subroomID) > 0)
            continue;

        if(dist->GetAgentsNumber()!=0){
            nPeds_expected += dist->GetAgentsNumber(); // classical number of agents
        }
        else if(dist->GetAgentsDensity()!=0){
            nPeds_expected += dist->GetAgentsDensity() * sr->GetArea() ; // number of agents = density * area of subroom
        }
        else{
        Log->Write("ERROR: \t Specify either number or density!");
        return false;}

        allFreePosRoom[subroomID] = PedDistributor::PossiblePositions(*sr);
    }

    //collect the available positions for that room
    for (const auto &dist: _start_dis) {
        int roomID = dist->GetRoomId();
        Room *r = building->GetRoom(roomID);
        if (!r) return false;

        nPeds_expected += dist->GetAgentsNumber();
        //compute all subrooms since no specific one is given
        for (const auto &it_sr: r->GetAllSubRooms()) {
            int subroomID = it_sr.second->GetSubRoomID();
            auto &allFreePosRoom = allFreePos[roomID];
            // the positions were already computed
            if (allFreePosRoom.count(subroomID) > 0)
                continue;
            allFreePosRoom[subroomID] = PedDistributor::PossiblePositions(*it_sr.second);
        }
    }


    // now proceed to the distribution
    int pid = 1; // the pedID is being increased throughout...

    for (const auto &dist: _start_dis_sub) {
        int room_id = dist->GetRoomId();
        Room *r = building->GetRoom(room_id);
        if (!r) continue;
        int roomID = r->GetID();
        int subroomID = dist->GetSubroomID();
        SubRoom *sr = r->GetSubRoom(subroomID);
        if (!sr) continue;

        int N;

        if(dist->GetAgentsNumber()!=0){
            N = dist->GetAgentsNumber();    // classical number of agents
        }
        else if(dist->GetAgentsDensity()!=0){
            N = dist->GetAgentsDensity() * sr->GetArea(); // number of agents = density * area of subroom
        }

        if (N < 0) {
            Log->Write("ERROR: \t negative  number of pedestrians!");
            return false;
        }

        auto &allpos = allFreePos[roomID][subroomID];

        int max_pos = allpos.size();
        if (max_pos < N) {
            Log->Write("ERROR: \tCannot distribute %d agents in Room %d . Maximum allowed: %d\n",
                       N, roomID, allpos.size());
            return false;
        }

        // Distributing
        Log->Write("INFO: \tDistributing %d Agents in Room/Subrom [%d/%d]! Maximum allowed: %d", N, roomID, subroomID,
                   max_pos);
        DistributeInSubRoom(sr, N, allpos, &pid, dist.get(), building);
        Log->Write("\t...Done");
        nPeds_is += N;
    }

    // then continue the distribution according to the rooms
    for (const auto &dist: _start_dis) {
        int room_id = dist->GetRoomId();
        Room *r = building->GetRoom(room_id);
        if (!r) continue;
        int N = dist->GetAgentsNumber();
        if (N < 0) {
            Log->Write("ERROR: \t negative or null number of pedestrians! Ignoring");
            continue;
        }

        double sum_area = 0;
        int max_pos = 0;
        double ppm; // pedestrians per square meter
        int ges_anz = 0;
        vector<int> max_anz = vector<int>();
        vector<int> akt_anz = vector<int>();

        auto &allFreePosInRoom = allFreePos[room_id];
        //FIXME: wont work if the subrooms ids are not continous, consider using map
        for (int is = 0; is < r->GetNumberOfSubRooms(); is++) {
            SubRoom *sr = r->GetSubRoom(is);
            double area = sr->GetArea();
            sum_area += area;
            int anz = allFreePosInRoom[is].size();
            max_anz.push_back(anz);
            max_pos += anz;
        }
        if (max_pos < N) {
            Log->Write("ERROR: \t Distribution of %d pedestrians in Room %d not possible! Maximum allowed: %d\n",
                       N, r->GetID(), max_pos);
            return false;
        }
        ppm = N / sum_area;
        // Anzahl der Personen pro SubRoom bestimmen
        //FIXME: wont work if the subrooms ids are not continous, consider using map
        for (int is = 0; is < r->GetNumberOfSubRooms(); is++) {
            SubRoom *sr = r->GetSubRoom(is);
            int anz = sr->GetArea() * ppm + 0.5; // wird absichtlich gerundet
            while (anz > max_anz[is]) {
                anz--;
            }
            akt_anz.push_back(anz);
            ges_anz += anz;
        }
        // Falls N noch nicht ganz erreicht, von vorne jeweils eine Person dazu
        int j = 0;
        while (ges_anz < N) {
            if (akt_anz[j] < max_anz[j]) {
                akt_anz[j] = akt_anz[j] + 1;
                ges_anz++;
            }
            j = (j + 1) % max_anz.size();
        }
        j = 0;
        while (ges_anz > N) {
            if (akt_anz[j] > 0) {
                akt_anz[j] = akt_anz[j] - 1;
                ges_anz--;
            }
            j = (j + 1) % max_anz.size();
        }
        // distributing
        for (unsigned int is = 0; is < akt_anz.size(); is++) {
            SubRoom *sr = r->GetSubRoom(is);
            //workaround, the subroom will be changing at each run.
            //so the last subroom ID is not necessarily the 'real' one
            // might conflicts with sources
            dist->SetSubroomID(sr->GetSubRoomID());
            //dist->SetSubroomUID(sr->GetSubRoomUID())
            if (akt_anz[is] > 0) {
                DistributeInSubRoom(sr, akt_anz[is], allFreePosInRoom[is], &pid, dist.get(), building);
            }
        }
        nPeds_is += N;
    }

    //now populate the sources
    vector<Point> emptyPositions;
    for (const auto &source: _start_dis_sources) {
        for (const auto &dist: _start_dis_sub) {
            if (source->GetGroupId() == dist->GetGroupId()) {
                source->SetStartDistribution(dist);
                for (int i = 0; i < source->GetMaxAgents(); i++) {
                    //source->AddToPool(dist->GenerateAgent(building, &pid,emptyPositions));
                    //nPeds_is++;
                }
            }
        }

        for (const auto &dist: _start_dis) {
            if (source->GetGroupId() == dist->GetGroupId()) {
                source->SetStartDistribution(dist);
                for (int i = 0; i < source->GetMaxAgents(); i++) {
                    //source->AddToPool(dist->GenerateAgent(building, &pid,emptyPositions));
                    //nPeds_is++;
                }
            }
        }
    }

    if (nPeds_is != nPeds_expected) {
        Log->Write("ERROR:\t only [%d] agents could be distributed out of [%d] requested", nPeds_is, nPeds_expected);
    }

    return (nPeds_is == nPeds_expected);
}


vector<Point> PedDistributor::PositionsOnFixX(double min_x, double max_x, double min_y, double max_y,
                                              const SubRoom &r, double bufx, double bufy, double dy) {
    vector<Point> positions;
    double x = (max_x + min_x) * 0.5;
    double y = min_y;

    while (y < max_y) {
        Point pos = Point(x, y);
        // Abstand zu allen Wänden prüfen
        bool ok = true;
        for (auto &&w: r.GetAllWalls()) {
            if (w.DistTo(pos) < max(bufx, bufy) || !r.IsInSubRoom(pos)) {
                ok = false;
                break; // Punkt ist zu nah an einer Wand oder nicht im Raum => ungültig
            }
        }

        if (ok) {
            //check all transitions
            bool tooNear = false;
            for (unsigned int t = 0; t < r.GetAllTransitions().size(); t++) {
                if (r.GetTransition(t)->DistTo(pos) < J_EPS_GOAL) {
                    //too close
                    tooNear = true;
                    break;
                }
            }

            for (unsigned int c = 0; c < r.GetAllCrossings().size(); c++) {
                if (r.GetCrossing(c)->DistTo(pos) < J_EPS_GOAL) {
                    //too close
                    tooNear = true;
                    break;
                }
            }
            if (tooNear == false) positions.push_back(pos);
        }
        y += dy;
    }
    return positions;

}

vector<Point>PedDistributor::PositionsOnFixY(double min_x, double max_x, double min_y, double max_y,
                                             const SubRoom &r, double bufx, double bufy, double dx) {
    vector<Point> positions;
    double y = (max_y + min_y) * 0.5;
    double x = min_x;

    while (x < max_x) {
        Point pos = Point(x, y);
        // check distance to wall
        bool ok = true;
        for (auto &&w: r.GetAllWalls()) {
            if (w.DistTo(pos) < max(bufx, bufy) || !r.IsInSubRoom(pos)) {
                ok = false;
                break; // Punkt ist zu nah an einer Wand oder nicht im Raum => ungültig
            }
        }

        if (ok) {

            //check all transitions
            bool tooNear = false;
            for (unsigned int t = 0; t < r.GetAllTransitions().size(); t++) {
                if (r.GetTransition(t)->DistTo(pos) < J_EPS_GOAL) {
                    //too close
                    tooNear = true;
                    break;
                }
            }

            for (unsigned int c = 0; c < r.GetAllCrossings().size(); c++) {
                if (r.GetCrossing(c)->DistTo(pos) < J_EPS_GOAL) {
                    //too close
                    tooNear = true;
                    break;
                }
            }
            if (tooNear == false) positions.push_back(pos);
        }
        x += dx;
    }
    return positions;
}

vector<Point>  PedDistributor::PossiblePositions(const SubRoom &r) {
    double uni = 0.7; // wenn ein Raum in x oder y -Richtung schmaler ist als 0.7 wird in der Mitte verteilt
    double bufx = 0.12;
    double bufy = 0.12;

    double amin = 0.18; // = GetAmin()->GetMean();
    double bmax = 0.25; // = GetBmax()->GetMean();

    double dx = amin + bufx;
    double dy = bmax + bufy;

    double max_buf = max(bufx, bufy);

    vector<double>::iterator min_x, max_x, min_y, max_y;
    const vector<Point> &poly = r.GetPolygon();
    vector<Point> all_positions;
    vector<double> xs;
    vector<double> ys;

    for (int p = 0; p < (int) poly.size(); ++p) {
        xs.push_back(poly[p]._x);
        ys.push_back(poly[p]._y);
    }

    min_x = min_element(xs.begin(), xs.end());
    max_x = max_element(xs.begin(), xs.end());
    min_y = min_element(ys.begin(), ys.end());
    max_y = max_element(ys.begin(), ys.end());

    if (*max_y - *min_y < uni) {
        all_positions = PositionsOnFixY(*min_x, *max_x, *min_y, *max_y, r, bufx, bufy, dx);
    } else if (*max_x - *min_x < uni) {
        all_positions = PositionsOnFixX(*min_x, *max_x, *min_y, *max_y, r, bufx, bufy, dy);
    } else {
        // create the grid
        double x = (*min_x);
        while (x < *max_x) {
            double y = (*min_y);
            while (y < *max_y) {
                y += dy;
                Point pos = Point(x, y);
                bool tooNear = false;

                //skip if the position is not in the polygon
                if (!r.IsInSubRoom(pos)) continue;

                // check the distance to all Walls
                for (const auto &w : r.GetAllWalls()) {
                    if (w.DistTo(pos) < max_buf) {
                        tooNear = true;
                        break; // too close
                    }
                }

                //check the distance to all transitions
                if (tooNear == true) continue;
                for (const auto &t : r.GetAllTransitions()) {
                    if (t->DistTo(pos) < max_buf) {
                        //too close
                        tooNear = true;
                        break;
                    }
                }

                //  and check the distance to all crossings
                if (tooNear == true) continue;
                for (const auto &c : r.GetAllCrossings()) {
                    if (c->DistTo(pos) < max_buf) {
                        //too close
                        tooNear = true;
                        break;
                    }
                }

                // and finally the distance to all opened obstacles
                if (tooNear == true) continue;

                for (const auto &obst : r.GetAllObstacles()) {
                    for (const auto &wall : obst->GetAllWalls()) {
                        if (wall.DistTo(pos) < max_buf) {
                            tooNear = true;
                            break; // too close
                        }
                    }

                    //only continue if...
                    if (tooNear == true) continue;

                    if (obst->Contains(pos)) {
                        tooNear = true;
                        break; // too close
                    }
                }

                if (tooNear == false) all_positions.push_back(pos);
            }
            x += dx;
        }
    }

    std::random_shuffle(all_positions.begin(), all_positions.end());
    return all_positions;
}

/* Verteilt N Fußgänger in SubRoom r
 * Algorithms:
 *   - Lege Gitter von min_x bis max_x mit Abstand dx und entsprechend min_y bis max_y mit
 *     Abstand dy
 *   - Prüfe alle so enstandenen Gitterpunkte, ob ihr Abstand zu Wänden und Übergängen aus
 *     reicht
 *   - Wenn nicht verwerfe Punkt, wenn ja merke Punkt in Vector positions
 *   - Wähle zufällig einen Punkt aus dem Vektor aus Position der einzelnen Fußgänger
 * Parameter:
 *   - r:       SubRoom in den verteilt wird
 *   - N:       Anzahl der Fußgänger für SubRoom r
 *   - pid:     fortlaufender Index der Fußgänger (wird auch wieder zurückgegeben, für den
 *              nächsten Aufruf)
 *   - routing: wird benötigt um die Zielline der Fußgänger zu initialisieren
 * */
void PedDistributor::DistributeInSubRoom(SubRoom *r, int nAgents, vector<Point> &positions, int *pid,
                                         StartDistribution *para, Building *building) const {
    // set the pedestrians
    for (int i = 0; i < nAgents; ++i) {
        Pedestrian *ped = para->GenerateAgent(building, pid, positions);
        building->AddPedestrian(ped);
    }
}




