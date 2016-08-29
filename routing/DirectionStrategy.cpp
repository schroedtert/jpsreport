/**
 * \file        DirectionStrategy.cpp
 * \date        Dec 13, 2010
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


#include "../geometry/Line.h"
#include "../geometry/NavLine.h"
#include "../geometry/Building.h"
//#include "../geometry/Room.h"
#include "../pedestrian/Pedestrian.h"
#include "../geometry/SubRoom.h"
//#include "../geometry/Wall.h"
#include "../routing/FloorfieldViaFM.h"
#include "../routing/LocalFloorfieldViaFM.h"
//#include "DirectionStrategy.h"
//#include <fstream>
#include <chrono>
//#include <ctime>

#define UNUSED(x) [&x]{}()  // c++11 silence warnings

DirectionStrategy::DirectionStrategy()
{
}


DirectionStrategy::~DirectionStrategy()
{
}
/// 1
Point DirectionMiddlePoint::GetTarget(Room* room, Pedestrian* ped) const
{
    UNUSED(room); // suppress the unused warning
    return (ped->GetExitLine()->GetPoint1() + ped->GetExitLine()->GetPoint2())*0.5;
}
/// 2
Point DirectionMinSeperationShorterLine::GetTarget(Room* room, Pedestrian* ped) const
{
    UNUSED(room); // suppress the unused warning

     double d = ped->GetEllipse().GetBmin() + 0.1 ; // shoulder//0.5;
     const Point& p1 = ped->GetExitLine()->GetPoint1();
     const Point& p2 = ped->GetExitLine()->GetPoint2();
     if(p1 == p2) return p1;

     double length = (p1 - p2).Norm();
     if(d >= 0.5*length) return (p1 + p2)*0.5; // return the middle point, since line is anyway too short
     double u = d/length; // d is supposed to be smaller than length, then u is in [0, 1]
     //Point diff = (p1 - p2).Normalized() * d;
     Line e_neu = Line(p1 + (p2-p1)*u, p1 + (p2-p1)*(1-u), 0);
     Point target = e_neu.ShortestPoint(ped->GetPos());
     // if(ped->GetID() == 81)
     // {
      // printf("=======\nX=[%.2f], Y=[%.2f]\n", ped->GetPos().GetX(), ped->GetPos().GetY());
      //     printf("p1=[%.2f, %.2f], p2=[%.2f, %.2f]\n", p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY());
      //     printf("u= %f length=%f\n", u, length);
      //     printf("e_neu=[%.2f, %.2f]===[%.2f, %.2f]\n", e_neu.GetPoint1().GetX(), e_neu.GetPoint1().GetY(), e_neu.GetPoint2().GetX(), e_neu.GetPoint2().GetY() );

     // printf("target=[%.2f, %.2f]\n====\n\n", target.GetX(), target.GetY());
     //getc(stdin);
     //}
     // kürzester Punkt auf der Linie

     return  target;

}
/// 3
Point DirectionInRangeBottleneck::GetTarget(Room* room, Pedestrian* ped) const
{
   UNUSED(room); // suppress the unused warning

    const Point& p1 = ped->GetExitLine()->GetPoint1();
    const Point& p2 = ped->GetExitLine()->GetPoint2();
    Line ExitLine = Line(p1, p2, 0);
    Point Lot = ExitLine.LotPoint( ped->GetPos() );
    Point ExitMiddle = (p1+p2)*0.5;
    double d = 0.2;
    // fix when using triangulation to avoid steering too near to the walls
    // double lenSq = ExitLine.LengthSquare();
    // if (lenSq >= 19 && lenSq < 50)
    //   d = 1;
    // else if(lenSq >= 50 &&lenSq < 100)
    //   d = 2;
    // else if(lenSq >= 100)
    //   d = 3;


    Point diff = (p1 - p2).Normalized() * d;
    Line e_neu = Line(p1 - diff, p2 + diff, 0);

    // if(ped->GetID() == )
    // {
    //     printf("=======\nX=[%.2f], Y=[%.2f]\n", ped->GetPos().GetX(), ped->GetPos().GetY());
    //     printf("p1=[%.2f, %.2f], p2=[%.2f, %.2f]\n", p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY());
    //     printf("e_neu=[%.2f, %.2f]===[%.2f, %.2f]\n", e_neu.GetPoint1().GetX(), e_neu.GetPoint1().GetY(), e_neu.GetPoint2().GetX(), e_neu.GetPoint2().GetY() );
    // }

    if ( e_neu.IsInLineSegment(Lot) ) {
        // if(ped->GetID() == -10){
        //     printf("Return Lot=[%.2f, %.2f]\n", Lot.GetX(), Lot.GetY() );
        //     if(0 && ped->GetPos().GetX() > 56)
        //         getc(stdin);}
        return Lot;
    } else {
        // if(ped->GetID() == -10){
        //     printf("Return Middle=[%.2f, %.2f]\n", ExitMiddle.GetX(), ExitMiddle.GetY() );
        //     if(0 && ped->GetPos().GetX() > 56)
        //         getc(stdin);}
        return ExitMiddle;
    }

}



/// 4
Point DirectionGeneral::GetTarget(Room* room, Pedestrian* ped) const
{
#define DEBUG 0
      using namespace std;
      const Point& p1 = ped->GetExitLine()->GetPoint1();
      const Point& p2 = ped->GetExitLine()->GetPoint2();
      Line ExitLine = Line(p1, p2, 0);
      //Point Lot = ExitLine.LotPoint( ped->GetPos() );
      double d = 0.2; //shorten the line by  20 cm
      Point diff = (p1 - p2).Normalized() * d;
      Line e_neu = Line(p1 - diff, p2 + diff, 0);
      Point NextPointOnLine =  e_neu.ShortestPoint(ped->GetPos());

      Line tmpDirection = Line(ped->GetPos(), NextPointOnLine, 0);//This direction will be rotated if
      // it intersects a wall || obstacle.
      // check for intersection with walls
      // @todo: make a FUNCTION of this

#if DEBUG
      printf("\n----------\nEnter GetTarget() with PED=%d\n----------\n",ped->GetID());
      printf("nextPointOn Line: %f %f\n", NextPointOnLine.GetX(), NextPointOnLine.GetY());
#endif
      double dist;
      int inear = -1;
      int iObs = -1;
      double minDist = 20001;
      int subroomId = ped->GetSubRoomID();
      SubRoom * subroom = room->GetSubRoom(subroomId);

      //============================ WALLS ===========================
      const vector<Wall>& walls = subroom->GetAllWalls();
      for (unsigned int i = 0; i < walls.size(); i++) {
            dist = tmpDirection.GetDistanceToIntersectionPoint(walls[i]);
            if (dist < minDist) {
                  inear = i;
                  minDist = dist;

#if DEBUG
                  printf("Check wall number %d. Dist = %f (%f)\n", i, dist, minDist);
                  printf("%f    %f --- %f    %f\n===========\n",walls[i].GetPoint1().GetX(),walls[i].GetPoint1().GetY(), walls[i].GetPoint2().GetX(),walls[i].GetPoint2().GetY());
#endif

            }
      }//walls

      //============================ WALLS ===========================


      //============================ OBST ===========================
      const vector<Obstacle*>& obstacles = subroom->GetAllObstacles();
      for(unsigned int obs=0; obs<obstacles.size(); ++obs) {
            const vector<Wall>& owalls = obstacles[obs]->GetAllWalls();
            for (unsigned int i = 0; i < owalls.size(); i++) {
                  dist = tmpDirection.GetDistanceToIntersectionPoint(owalls[i]);
                  if (dist < minDist) {
                        inear = i;
                        minDist = dist;
                        iObs = obs;
#if DEBUG
                        printf("Check OBS:obs=%d, i=%d Dist = %f (%f)\n", obs, i, dist, minDist);
                        printf("%f    %f --- %f    %f\n===========\n",owalls[i].GetPoint1().GetX(),owalls[i].GetPoint1().GetY(), owalls[i].GetPoint2().GetX(),owalls[i].GetPoint2().GetY());
#endif
                  }
            }//walls of obstacle
      }// obstacles
      //============================ OBST ===========================


      double angle = 0;
      if (inear >= 0) {
            ped->SetNewOrientationFlag(true); //Mark this pedestrian for next target calculation
            ped->SetDistToBlockade(minDist);
            if(iObs >= 0){ // obstacle is nearest
                  const vector<Wall>& owalls = obstacles[iObs]->GetAllWalls();
                  angle = tmpDirection.GetObstacleDeviationAngle(owalls, walls);

                  // angle =  tmpDirection.GetDeviationAngle(owalls[inear].enlarge(2*ped->GetLargerAxis()));

#if DEBUG
                  printf("COLLISION WITH OBSTACLE %f    %f --- %f    %f\n===========\n",owalls[inear].GetPoint1().GetX(),owalls[inear].GetPoint1().GetY(), owalls[inear].GetPoint2().GetX(),owalls[inear].GetPoint2().GetY());

#endif
            } //iObs
            else{ // wall is nearest
                  angle =  tmpDirection.GetDeviationAngle(walls[inear].Enlarge(2*ped->GetLargerAxis()));

#if DEBUG
                  printf("COLLISION WITH WALL %f    %f --- %f    %f\n===========\n",walls[inear].GetPoint1().GetX(),walls[inear].GetPoint1().GetY(), walls[inear].GetPoint2().GetX(),walls[inear].GetPoint2().GetY());
#endif
            } //else
      }//inear
      else{

            if(ped->GetNewOrientationFlag()){ //this pedestrian could not see the target and now he can see it clearly.
                  // printf("ped->GetNewOrientationFlag()=%d\n",ped->GetNewOrientationFlag());getc(stdin);
                  ped->SetSmoothTurning(); // so the turning should be adapted accordingly.
                  ped->SetNewOrientationFlag(false);
            }
      }
////////////////////////////////////////////////////////////


      Point  G;
      if (fabs(angle) > J_EPS)
            //G  =  tmpDirection.GetPoint2().Rotate(cos(angle), sin(angle)) ;
            G  = (NextPointOnLine-ped->GetPos()).Rotate(cos(angle), sin(angle))+ped->GetPos() ;
      else {
            if(ped->GetNewOrientationFlag()) //this pedestrian could not see the target and now he can see it clearly.
                  ped->SetSmoothTurning(); // so the turning should be adapted accordingly.

            G  =  NextPointOnLine;
      }

#if DEBUG
      printf("inear=%d, iObs=%d, minDist=%f\n", inear, iObs, minDist);
      printf("PED=%d\n",  ped->GetID());
      printf ("MC Posx = %.2f, Posy=%.2f, Lot=[%.2f, %.2f]\n", ped->GetPos().GetX(), ped->GetPos().GetY(), NextPointOnLine.GetX(), NextPointOnLine.GetY());
      printf("MC p1=[%.2f, %.2f] p2=[%.2f, %.2f]\n", p1.GetX(), p1.GetY(),  p2.GetX(), p2.GetY());
      printf("angle=%f, G=[%.2f, %.2f]\n", angle, G.GetX(), G.GetY());
      printf("\n----------\nLEAVE function with PED=%d\n----------\n",ped->GetID());
      // getc(stdin);


#endif

      // if( ped->GetID() == 21)
      //       fprintf(stderr, "%.2f %.2f %.2f %.2f %f %f %d %.2f %.2f %.2f\n", NextPointOnLine.GetX(), NextPointOnLine.GetY(),
      //               ped->GetPos().GetX(), ped->GetPos().GetY(), G.GetX(), G.GetY(), ped->GetID(), ped->GetV0().GetX(), ped->GetV0().GetY(), ped->GetGlobalTime());
// this stderr output can be used with plot_desired_velocity.py



      // if( ped->GetID() == 1)
            // fprintf(stderr, "%.2f %.2f %.2f %.2f %f %f %d %.2f %.2f %.2f\n", NextPointOnLine.GetX(), NextPointOnLine.GetY(),
                    // ped->GetPos().GetX(), ped->GetPos().GetY(), G.GetX(), G.GetY(), ped->GetID(), ped->GetV0().GetX(), ped->GetV0().GetY(), ped->GetGlobalTime());
// this stderr output can be used with scripts/plot_desired_velocity.py

      return G;
}

/// 6
Point DirectionFloorfield::GetTarget(Room* room, Pedestrian* ped) const
{
     UNUSED(room);
#if DEBUG
    if (initDone && (ffviafm != nullptr)) {
#endif // DEBUG

        Point p;
        ffviafm->getDirectionToDestination(ped, p);
        p = p.Normalized();     // @todo: argraf : scale with costvalue: " * ffviafm->getCostToTransition(ped->GetTransitionID(), ped->GetPos()) "
        return (p + ped->GetPos());

#if DEBUG
    }
#endif // DEBUG

    //this should not execute:
    std::cerr << "Failure in DirectionFloorfield::GetTarget!!" << std::endl;
    exit(EXIT_FAILURE);
}

Point DirectionFloorfield::GetDir2Wall(Pedestrian* ped) const
{
    Point p;
    ffviafm->getDir2WallAt(ped->GetPos(), p);
    return p;
}

double DirectionFloorfield::GetDistance2Wall(Pedestrian* ped) const
{
    return ffviafm->getDistance2WallAt(ped->GetPos());
}

void DirectionFloorfield::Init(Building* building, double stepsize,
                               double threshold, bool useDistancMap) {
    //implement mechanic, that can read-in an existing floorfield (from a previous run)
    string s = building->GetGeometryFilename();
    Log->Write("INFO: \tGeometryFilename <" + s + ">");
    s.erase(s.find_last_of(".", string::npos)); // delete ending
    if (s.find_last_of("/") != string::npos) {
        s.erase(0, s.find_last_of("/")+1);      // delete directories before filename (espacially "..")
    }
    string FF_filename = (building->GetProjectRootDir() + "FF_" + s +  "_" + std::to_string(threshold) + ".vtk").c_str();
    std::ifstream test(FF_filename);
    if (test.good()) {
          Log->Write("INFO: \tRead Floorfield from file <" + FF_filename + ">");
        ffviafm = new FloorfieldViaFM(FF_filename);
    } else {
          std::chrono::time_point<std::chrono::system_clock> start, end;
          start = std::chrono::system_clock::now();
          Log->Write("INFO: \tCalling Construtor of FloorfieldViaFM");
          ffviafm = new FloorfieldViaFM(building, stepsize, stepsize, threshold,
                                        useDistancMap, false);
          end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsed_seconds = end-start;
          Log->Write("INFO: \tTaken time: " + std::to_string(elapsed_seconds.count()));
    }
    initDone = true;
}

DirectionFloorfield::DirectionFloorfield() {
    ffviafm = nullptr;
    initDone = false;
};

DirectionFloorfield::~DirectionFloorfield() {
    if (ffviafm) {
        delete ffviafm;
    }
}

/// 8
Point DirectionLocalFloorfield::GetTarget(Room* room, Pedestrian* ped) const
{
#if DEBUG
     if (initDone && (ffviafm != nullptr)) {
#endif // DEBUG

     Point p;
//     if (locffviafm.count(room->GetID())) {
//#pragma omp critical
     locffviafm.at(room->GetID())->getDirectionToDestination(ped, p);
//     } else {
          //create locffviafm[room->GetID()] - therefore use all members needed for ctor
          // members will be added to this class
//          locffviafm.emplace(room->GetID(), new LocalFloorfieldViaFM(room, building,
//                                      hx, hy, wallAvoidDistance, useDistancefield,
//                                      filename));
//          locffviafm[room->GetID()]->getDirectionToDestination(ped, p);
//     }
     if (Pedestrian::GetGlobalTime() == 10) Log->Write("DirectionLocalFloorfield::GetTarget(): p == %f, %f", p._x, p._y);
     p = p.Normalized();     // @todo: argraf : scale with costvalue: " * ffviafm->getCostToTransition(ped->GetTransitionID(), ped->GetPos()) "
     if (Pedestrian::GetGlobalTime() == 10) Log->Write("DirectionLocalFloorfield::GetTarget(): p.Normalized == %f, %f", p._x, p._y);
     if (Pedestrian::GetGlobalTime() == 10) Log->Write("DirectionLocalFloorfield::GetTarget(): ped->GetPos() == %f, %f", ped->GetPos()._x, ped->GetPos()._y);
     return (p + ped->GetPos());

#if DEBUG
     }
#endif // DEBUG

     //this should not execute:
     //std::cerr << "Failure in DirectionFloorfield::GetTarget!!" << std::endl;
    // exit(EXIT_FAILURE);
}

Point DirectionLocalFloorfield::GetDir2Wall(Pedestrian* ped) const
{
     Point p;
     int roomID = ped->GetRoomID();
     locffviafm.at(roomID)->getDir2WallAt(ped->GetPos(), p);
     if (Pedestrian::GetGlobalTime() == 10) {
          Log->Write("DirectionLocalFloorfield::GetDir2Wall was called for roomID %d, subroomID %d", roomID, ped->GetSubRoomID());
          Log->Write("at %f, %f, the result is %f, %f", ped->GetPos()._x, ped->GetPos()._y, p._x, p._y);
     }
     return p;
}

double DirectionLocalFloorfield::GetDistance2Wall(Pedestrian* ped) const
{
     return locffviafm.at(ped->GetRoomID())->getDistance2WallAt(ped->GetPos());
}

void DirectionLocalFloorfield::Init(Building* buildingArg, double stepsize,
                                    double threshold, bool useDistancMap) {
     hx = stepsize;
     hy = stepsize;
     building = buildingArg;
     wallAvoidDistance = threshold;
     useDistancefield = useDistancMap;

     //implement mechanic, that can read-in an existing floorfield (from a previous run)
//     string s = building->GetGeometryFilename();
//     Log->Write("INFO: \tGeometryFilename <" + s + ">");
//     s.erase(s.find_last_of(".", string::npos)); // delete ending
//     if (s.find_last_of("/") != string::npos) {
//          s.erase(0, s.find_last_of("/")+1);      // delete directories before filename (espacially "..")
//     }
//     string FF_filename = (building->GetProjectRootDir() + "FF_" + s +  "_" + std::to_string(threshold) + ".vtk").c_str();
//     std::ifstream test(FF_filename);
//     if (test.good() && false) {
//          //Log->Write("INFO: \tRead Floorfield from file <" + FF_filename + ">");
//          //ffviafm = new FloorfieldViaFM(FF_filename);
//     } else {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    Log->Write("INFO: \tCalling Constructor of LocFloorfieldViaFM in DirectionLocalFloorfield::Init(...)");
#pragma omp parallel for
     for (unsigned int i = 0; i < building->GetAllRooms().size(); ++i) {
     //for (auto& roomPair : building->GetAllRooms()) {
         auto roomPairIt = building->GetAllRooms().begin();
         std::advance(roomPairIt, i);
         locffviafm[(*roomPairIt).first] = new LocalFloorfieldViaFM((*roomPairIt).second.get(), building,
                 hx, hy, wallAvoidDistance, useDistancefield);
     }
     end = std::chrono::system_clock::now();
     std::chrono::duration<double> elapsed_seconds = end-start;
     Log->Write("INFO: \tTaken time: " + std::to_string(elapsed_seconds.count()));
//     }
     initDone = true;
    //create all presumably used ff in parallel region: a) find targets and corresponding rooms, b) create ff s
     // @todo f.mack once CalcFloorfield() is complete, the following can go
     /*
    std::vector<std::pair<int, int>> roomAndTargetVector;
    roomAndTargetVector.clear();
    //for now, just calc all ff for every transition in a room
    int roomNr = 0;
    int transUID = 0;
    for (unsigned int i = 0; i < building->GetAllRooms().size(); ++i) {
        auto roomIt = building->GetAllRooms().begin();
        std::advance(roomIt, i);
        roomNr = roomIt->second->GetID();
        for (unsigned int j = 0; j < roomIt->second->GetAllTransitionsIDs().size(); ++j) {
            transUID = roomIt->second->GetAllTransitionsIDs()[j];
            if (building->GetTransitionByUID(transUID)->IsOpen()) {
                std::pair<int, int> key = std::make_pair(roomNr, transUID);
                roomAndTargetVector.emplace_back(key);
            }
        }
    }
#pragma omp parallel for
    for(unsigned int i = 0; i < roomAndTargetVector.size(); ++i){
        auto rAndtIT = roomAndTargetVector.begin();
        std::advance(rAndtIT, i);
        Point dummy;
        locffviafm[(*rAndtIT).first]->getDirectionToUID((*rAndtIT).second, 0, dummy);
    }
     for(unsigned int i = 0; i < building->GetAllRooms().size(); ++i) {
          std::vector<int> targets = {};
          targets.clear();

          auto roomIt = building->GetAllRooms().begin();
          std::advance(roomIt, i);
          roomNr = roomIt->second->GetID();

          for (auto pair : roomAndTargetVector) {
               if (pair.first == roomNr) {
                    targets.emplace_back(pair.second);
               }
          }
          //@todo: ar.graf: plz make switch in ini file and dependand on ini write yes/no (issue 208)
          std::string lfilename = "floorfield" + std::to_string(roomNr) + ".vtk";
          //locffviafm[roomNr]->writeFF(lfilename, targets);
     }
*/

}

void DirectionLocalFloorfield::CalcFloorfield(int room, int destUID) {
     Point dummy;
     locffviafm.at(room)->getDirectionToUID(destUID, 0, dummy);
}

void DirectionLocalFloorfield::writeFF(int room, std::vector<int> targets) {
     std::string lfilename = "floorfield" + std::to_string(room) + ".vtk";
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
     locffviafm[room]->writeFF(lfilename, targets);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    Log->Write("INFO: \twriting %s took %f s ", lfilename.c_str(), elapsed_seconds.count());
}

DirectionLocalFloorfield::DirectionLocalFloorfield() {
     //locffviafm = nullptr;
     initDone = false;
}

DirectionLocalFloorfield::~DirectionLocalFloorfield() {
     //if (locffviafm) {
     for (auto pair : locffviafm) {
          delete pair.second;
     }
     //}
}


///9
Point DirectionSubLocalFloorfield::GetTarget(Room* room, Pedestrian* ped) const
{
     (void)room; // silence warning
#if DEBUG
     if (initDone && (ffviafm != nullptr)) {
#endif // DEBUG

     Point p;
//     if (!locffviafm.count(ped->GetSubRoomUID())) {
//          std::cerr << "Error: no mapentry for key :" << ped->GetSubRoomUID() << std::endl;
//     } else {
//          std::cerr << "Mapentry found: " << ped->GetSubRoomUID() << " with ptr: " << locffviafm.at(ped->GetSubRoomUID()) << std::endl;
//     }
#pragma omp critical
     locffviafm.at(ped->GetSubRoomUID())->getDirectionToDestination(ped, p);

     p = p.Normalized();     // @todo: argraf : scale with costvalue: " * ffviafm->getCostToTransition(ped->GetTransitionID(), ped->GetPos()) "
     return (p + ped->GetPos());

#if DEBUG
     }
#endif // DEBUG

     //this should not execute:
     //std::cerr << "Failure in DirectionFloorfield::GetTarget!!" << std::endl;
     //exit(EXIT_FAILURE);
}

Point DirectionSubLocalFloorfield::GetDir2Wall(Pedestrian* ped) const
{
     Point p;
     int key = ped->GetSubRoomUID();
     locffviafm.at(key)->getDir2WallAt(ped->GetPos(), p);
     return p;
}

double DirectionSubLocalFloorfield::GetDistance2Wall(Pedestrian* ped) const
{
     return locffviafm.at(ped->GetSubRoomUID())->getDistance2WallAt(ped->GetPos());
}

void DirectionSubLocalFloorfield::Init(Building* buildingArg, double stepsize,
      double threshold, bool useDistancMap) {
     hx = stepsize;
     hy = stepsize;
     building = buildingArg;
     wallAvoidDistance = threshold;
     useDistancefield = useDistancMap;

     //implement mechanic, that can read-in an existing floorfield (from a previous run)
//     string s = building->GetGeometryFilename();
//     Log->Write("INFO: \tGeometryFilename <" + s + ">");
//     s.erase(s.find_last_of(".", string::npos)); // delete ending
//     if (s.find_last_of("/") != string::npos) {
//          s.erase(0, s.find_last_of("/")+1);      // delete directories before filename (espacially "..")
//     }
//     string FF_filename = (building->GetProjectRootDir() + "FF_" + s +  "_" + std::to_string(threshold) + ".vtk").c_str();
//     std::ifstream test(FF_filename);
//     if (test.good()) {
//          //Log->Write("INFO: \tRead Floorfield from file <" + FF_filename + ">");
//          //ffviafm = new FloorfieldViaFM(FF_filename);
//     } else {
          std::chrono::time_point<std::chrono::system_clock> start, end;
          start = std::chrono::system_clock::now();
          Log->Write("INFO: \tCalling Construtor of SubLocFloorfieldViaFM");
          std::vector<std::pair<int, int>> subAndTarget;
          subAndTarget.clear();
          std::vector<int> subUIDs;
          subUIDs.clear();
          for (auto& roomPair : building->GetAllRooms()) {
#pragma omp parallel for
               for (unsigned int i = 0; i < roomPair.second->GetAllSubRooms().size(); ++i) {
                    auto subroomIt = roomPair.second->GetAllSubRooms().begin();
                    std::advance(subroomIt, i);
                    int subUID = subroomIt->second->GetUID();
#pragma omp critical
                    subUIDs.emplace_back(subUID);
                    Log->Write("Creating SubLocFF at key: %d", subUID);
                    locffviafm[subUID] = new SubLocalFloorfieldViaFM(
                              subroomIt->second.get(), building,
                              hx, hy, wallAvoidDistance, useDistancefield);
                    auto targets = subroomIt->second->GetAllGoalIDs();
                    for (auto targetUID : targets) {
                         subAndTarget.emplace_back(std::make_pair(subUID, targetUID));
                    }
               }
//               for (auto& subRoomPair : roomPair.second->GetAllSubRooms()) {
//                    std::cerr << "Creating SubLocFF at key: " << subRoomPair.second->GetUID() <<std::endl;
//                    locffviafm[subRoomPair.second->GetUID()] = new SubLocalFloorfieldViaFM(
//                          &(*subRoomPair.second), building,
//                          hx, hy, wallAvoidDistance, useDistancefield,
//                          "FF_filename");
//               }
          }
#pragma omp parallel for
          for (unsigned int i = 0; i < subAndTarget.size(); ++i) {
               auto pairIT = subAndTarget.begin();
               std::advance(pairIT, i);
               Point dummy;
               locffviafm[pairIT->first]->getDirectionToUID(pairIT->second, 0, dummy);
          }
          end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsed_seconds = end-start;
          Log->Write("INFO: \tTaken time: " + std::to_string(elapsed_seconds.count()));

//     }
     initDone = true;

     //write floorfields to file, one file per subroom
     for(unsigned int i = 0; i < subUIDs.size(); ++i) {
          std::vector<int> targets = {};
          targets.clear();
          int subroomUID = subUIDs[i];

          for (auto pair : subAndTarget) {
               if (pair.first == subroomUID) {
                    targets.emplace_back(pair.second);
               }
          }
          //@todo: ar.graf: make ini file switch if files are written (yes/no) (Issue 208)
          std::string filename = "floorfield" + std::to_string(subroomUID) + ".vtk";
          //locffviafm[subroomUID]->writeFF(filename, targets);
     }
}

DirectionSubLocalFloorfield::DirectionSubLocalFloorfield() {
     //locffviafm = nullptr;
     initDone = false;
}

DirectionSubLocalFloorfield::~DirectionSubLocalFloorfield() {
     //if (locffviafm) {
     for (auto pair : locffviafm) {
          delete pair.second;
     }
     //}
}
