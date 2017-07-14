//
// Created by arne on 5/9/17.
//

#include "UnivFFviaFM.h"
#include "../../geometry/Line.h"
#include "../../geometry/Building.h"
#include "../../geometry/Room.h"
#include "../../geometry/SubRoom.h"
#include "mesh/RectGrid.h"


UnivFFviaFM::UnivFFviaFM(Room* r, Building* b, double hx, double wallAvoid, bool useWallAvoid)
          : UnivFFviaFM(r, b->GetConfig(), hx, wallAvoid, useWallAvoid) {
     _building = b;
}

UnivFFviaFM::UnivFFviaFM(SubRoom* sr, Building* b, double hx, double wallAvoid, bool useWallAvoid)
          : UnivFFviaFM(sr, b->GetConfig(), hx, wallAvoid, useWallAvoid) {
     _building = b;
}

UnivFFviaFM::UnivFFviaFM(Room* r, Configuration* const conf, double hx, double wallAvoid, bool useWallAvoid)
          : UnivFFviaFM(r, conf, hx, wallAvoid, useWallAvoid, std::vector<int>()){
}

UnivFFviaFM::UnivFFviaFM(Room* roomArg, Configuration* const confArg, double hx, double wallAvoid, bool useWallAvoid, std::vector<int> wantedDoors) {
     //build the vector with walls(wall or obstacle), the map with <UID, Door(Cross or Trans)>, the vector with targets(UIDs)
     //then call other constructor including the mode

     _configuration = confArg;
     _scope = FF_ROOM_SCALE;
     std::vector<Line> lines;
     std::map<int, Line> tmpDoors;

     for (auto& subroomMap : roomArg->GetAllSubRooms()) {
          SubRoom* subRoomPtr = subroomMap.second.get();
          std::vector<Wall> walls = std::vector<Wall>(subRoomPtr->GetAllWalls());
          for (auto& wall : walls) {
               lines.emplace_back((Line)wall);
          }

          std::vector<Obstacle*> tmpObsPtrVec = subRoomPtr->GetAllObstacles();
          for (Obstacle* ptrObs : tmpObsPtrVec) {
               const std::vector<Wall> obsWalls = ptrObs->GetAllWalls();
               for (auto& owall : obsWalls) {
                    lines.emplace_back((Line)owall);
               }
          }

          const std::vector<Crossing*> tmpCross = subRoomPtr->GetAllCrossings();
          const std::vector<Transition*> tmpTrans = subRoomPtr->GetAllTransitions();

          int uidNotConst = 0;
          for (auto& cross : tmpCross) {
               uidNotConst = cross->GetUniqueID();
               if (tmpDoors.count(uidNotConst) == 0) {
                    tmpDoors.emplace(std::make_pair(uidNotConst, (Line) *cross));
               }
          }
          for (auto& trans : tmpTrans) {
               uidNotConst = trans->GetUniqueID();
               if (tmpDoors.count(uidNotConst) == 0) {
                    tmpDoors.emplace(std::make_pair(uidNotConst, (Line) *trans));
               }
          }
     }

     //create(lines, tmpDoors, wantedDoors, FF_HOMO_SPEED, hx, wallAvoid, useWallAvoid);
     create(lines, tmpDoors, wantedDoors, FF_WALL_AVOID, hx, wallAvoid, useWallAvoid);
     //writeFF("UnivFFRoom.vtk", this->getKnownDoorUIDs());
}

UnivFFviaFM::UnivFFviaFM(SubRoom* sr, Configuration* const conf, double hx, double wallAvoid, bool useWallAvoid)
          : UnivFFviaFM(sr, conf, hx, wallAvoid, useWallAvoid, std::vector<int>()){
}

UnivFFviaFM::UnivFFviaFM(SubRoom* subRoomArg, Configuration* const confArg, double hx, double wallAvoid, bool useWallAvoid, std::vector<int> wantedDoors) {
     //build the vector with walls(wall or obstacle), the map with <UID, Door(Cross or Trans)>, the vector with targets(UIDs)
     //then call other constructor including the mode
     _configuration = confArg;
     _scope = FF_SUBROOM_SCALE;
     std::vector<Line> lines;
     std::map<int, Line> tmpDoors;

     std::vector<Wall> walls = std::vector<Wall> (subRoomArg->GetAllWalls());
     for (auto& wall : walls) {
          lines.emplace_back((Line)wall);
     }

     std::vector<Obstacle*> tmpObsPtrVec = subRoomArg->GetAllObstacles();
     for (Obstacle* ptrObs : tmpObsPtrVec) {
          const std::vector<Wall> obsWalls = ptrObs->GetAllWalls();
          for (auto& owall : obsWalls) {
               lines.emplace_back((Line)owall);
          }
     }

     const std::vector<Crossing*> tmpCross = subRoomArg->GetAllCrossings();
     const std::vector<Transition*> tmpTrans = subRoomArg->GetAllTransitions();

     int uidNotConst = 0;
     for (auto& cross : tmpCross) {
          uidNotConst = cross->GetUniqueID();
          tmpDoors.emplace(std::make_pair(uidNotConst, (Line) *cross));
     }
     for (auto& trans : tmpTrans) {
          uidNotConst = trans->GetUniqueID();
          tmpDoors.emplace(std::make_pair(uidNotConst, (Line) *trans));
     }

     create(lines, tmpDoors, wantedDoors, FF_HOMO_SPEED, hx, wallAvoid, useWallAvoid);
     writeFF("UnivFFSubroom.vtk", this->getKnownDoorUIDs());
}

void UnivFFviaFM::create(std::vector<Line>& walls, std::map<int, Line>& doors, std::vector<int> targetUIDs, int mode,
                         double spacing, double wallAvoid, bool useWallAvoid) {

     _wallAvoidDistance = wallAvoid;
     _useWallAvoidance = useWallAvoid;
     _speedmode = mode;

     //find circumscribing rectangle (x_min/max, y_min/max) //create RectGrid
     createRectGrid(walls, doors, spacing);
     _nPoints = _grid->GetnPoints();

     //allocate _gridCode and  _speedFieldSelector and initialize them ("draw" walls and doors)
     _gridCode = new int[_nPoints];
     processGeometry(walls, doors);
     _speedFieldSelector.emplace(_speedFieldSelector.begin()+INITIAL_SPEED, new double[_nPoints]);
     std::fill(_speedFieldSelector[INITIAL_SPEED], _speedFieldSelector[INITIAL_SPEED]+_nPoints, 1.0);

     //allocate _initalSpeed and maybe _modifiedSpeed
     if (mode == FF_WALL_AVOID) {
          double* cost_alias_walldistance = new double[_nPoints];
          _costFieldWithKey[0] = cost_alias_walldistance;
          Point* gradient_alias_walldirection = new Point[_nPoints];
          _directionFieldWithKey[0] = gradient_alias_walldirection;

          //create wall distance field
          //init costarray
          for (int i = 0; i < _nPoints; ++i) {
               if (_gridCode[i] == WALL) {
                    cost_alias_walldistance[i] = magicnum(WALL_ON_COSTARRAY);
               } else {
                    cost_alias_walldistance[i] = magicnum(UNKNOWN_COST);
               }
          }
          drawLinesOnWall(walls, cost_alias_walldistance, magicnum(TARGET_REGION));
          calcDF(cost_alias_walldistance, gradient_alias_walldirection, _speedFieldSelector[INITIAL_SPEED]);
          //_uids.emplace_back(0);

          double* temp_reduWallSpeed = new double[_nPoints];
          if (_speedFieldSelector.size() > 1) { //free memory before overwriting
               delete[] _speedFieldSelector[REDU_WALL_SPEED];
          }
          _speedFieldSelector[REDU_WALL_SPEED] = temp_reduWallSpeed;
          //init _reducedWallSpeed by using distance field
          //@todo: @ar.graf @newFF
          createReduWallSpeed(temp_reduWallSpeed);
     }

     //the memory will be allocated in "addTarget". for parallel processing, we might change it to allocate before the
     //parallel region and call a fct "addTarget(int, [ptr_to_preallocated mem])"
     for (auto targetUID : targetUIDs ) {
          addTarget(targetUID);
     }    //loop over targets



}

void UnivFFviaFM::createRectGrid(std::vector<Line>& walls, std::map<int, Line>& doors, double spacing) {
     double x_min = DBL_MAX;  double x_max = DBL_MIN;
     double y_min = DBL_MAX;  double y_max = DBL_MIN;

     for(auto& wall : walls) {
          if (wall.GetPoint1()._x < x_min) x_min = wall.GetPoint1()._x;
          if (wall.GetPoint1()._y < y_min) y_min = wall.GetPoint1()._y;
          if (wall.GetPoint2()._x < x_min) x_min = wall.GetPoint2()._x;
          if (wall.GetPoint2()._y < y_min) y_min = wall.GetPoint2()._y;

          if (wall.GetPoint1()._x > x_max) x_max = wall.GetPoint1()._x;
          if (wall.GetPoint1()._y > y_max) y_max = wall.GetPoint1()._y;
          if (wall.GetPoint2()._x > x_max) x_max = wall.GetPoint2()._x;
          if (wall.GetPoint2()._y > y_max) y_max = wall.GetPoint2()._y;
     }

     for(auto& doorPair:doors) {
          Line& door = doorPair.second;
          if (door.GetPoint1()._x < x_min) x_min = door.GetPoint1()._x;
          if (door.GetPoint1()._y < y_min) y_min = door.GetPoint1()._y;
          if (door.GetPoint2()._x < x_min) x_min = door.GetPoint2()._x;
          if (door.GetPoint2()._y < y_min) y_min = door.GetPoint2()._y;

          if (door.GetPoint1()._x > x_max) x_max = door.GetPoint1()._x;
          if (door.GetPoint1()._y > y_max) y_max = door.GetPoint1()._y;
          if (door.GetPoint2()._x > x_max) x_max = door.GetPoint2()._x;
          if (door.GetPoint2()._y > y_max) y_max = door.GetPoint2()._y;
     }

     //create Rect Grid
     _grid = new RectGrid();
     _grid->setBoundaries(x_min, y_min, x_max, y_max);
     _grid->setSpacing(spacing, spacing);
     _grid->createGrid();
}

void UnivFFviaFM::processGeometry(std::vector<Line>&walls, std::map<int, Line>& doors) {
     for (int i = 0; i < _nPoints; ++i) {
          _gridCode[i] = OUTSIDE;
     }

     for (auto mapentry : doors) {
          _doors.insert(mapentry);
     }
     //_doors = doors;

     drawLinesOnGrid<int>(walls, _gridCode, WALL);
     drawLinesOnGrid(doors, _gridCode); //UIDs of doors will be drawn on _gridCode
}

void UnivFFviaFM::createReduWallSpeed(double* reduWallSpeed){
     double factor = 1/_wallAvoidDistance;
     double* wallDstAlias = _costFieldWithKey[0];

     for (long int i = 0; i < _nPoints; ++i) {
          if (wallDstAlias[i] > 0.) {
               reduWallSpeed[i] = (wallDstAlias[i] > _wallAvoidDistance) ? 1.0 : (factor * wallDstAlias[i]);
          }
     }
}
void UnivFFviaFM::drawLinesOnGrid(std::map<int, Line>& doors, int *const grid) {
     for (auto&& doorPair : doors) {
          int tempUID = doorPair.first;
          Line tempDoorLine = Line(doorPair.second);
          drawLinesOnGrid(tempDoorLine, grid, tempUID);
     }
}

template <typename T>
void UnivFFviaFM::drawLinesOnGrid(std::vector<Line>& wallArg, T* const target, const T value) { //no init, plz init elsewhere

     for (auto& line : wallArg) {
          drawLinesOnGrid(line, target, value);
     } //loop over all walls

} //drawLinesOnGrid

template <typename T>
void UnivFFviaFM::drawLinesOnGrid(Line& line, T* const target, const T value) { //no init, plz init elsewhere
// i~x; j~y;
//http://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm
//src in answer of "Avi"; adapted to fit this application

     //grid handeling local vars:
     long int iMax  = _grid->GetiMax();

     long int iStart, iEnd;
     long int jStart, jEnd;
     long int iDot, jDot;
     long int key;
     long int deltaX, deltaY, deltaX1, deltaY1, px, py, xe, ye, i; //Bresenham Algorithm


     key = _grid->getKeyAtPoint(line.GetPoint1());
     iStart = (long) _grid->get_i_fromKey(key);
     jStart = (long) _grid->get_j_fromKey(key);

     key = _grid->getKeyAtPoint(line.GetPoint2());
     iEnd = (long) _grid->get_i_fromKey(key);
     jEnd = (long) _grid->get_j_fromKey(key);

     deltaX = (int) (iEnd - iStart);
     deltaY = (int) (jEnd - jStart);
     deltaX1 = abs( (int) (iEnd - iStart));
     deltaY1 = abs( (int) (jEnd - jStart));

     px = 2*deltaY1 - deltaX1;
     py = 2*deltaX1 - deltaY1;

     if(deltaY1<=deltaX1) {
          if(deltaX>=0) {
               iDot = iStart;
               jDot = jStart;
               xe = iEnd;
          } else {
               iDot = iEnd;
               jDot = jEnd;
               xe = iStart;
          }
          if ((_gridCode[jDot*iMax + iDot] != WALL) && (_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
               target[jDot * iMax + iDot] = value;
          }
          for (i=0; iDot < xe; ++i) {
               ++iDot;
               if(px<0) {
                    px+=2*deltaY1;
               } else {
                    if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY>0)) {
                         ++jDot;
                    } else {
                         --jDot;
                    }
                    px+=2*(deltaY1-deltaX1);
               }
               if ((_gridCode[jDot*iMax + iDot] != WALL) && (_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
                    target[jDot * iMax + iDot] = value;
               }
          }
     } else {
          if(deltaY>=0) {
               iDot = iStart;
               jDot = jStart;
               ye = jEnd;
          } else {
               iDot = iEnd;
               jDot = jEnd;
               ye = jStart;
          }
          if ((_gridCode[jDot*iMax + iDot] != WALL) && (_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
               target[jDot * iMax + iDot] = value;
          }
          for(i=0; jDot<ye; ++i) {
               ++jDot;
               if (py<=0) {
                    py+=2*deltaX1;
               } else {
                    if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY>0)) {
                         ++iDot;
                    } else {
                         --iDot;
                    }
                    py+=2*(deltaX1-deltaY1);
               }
               if ((_gridCode[jDot*iMax + iDot] != WALL) && (_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
                    target[jDot * iMax + iDot] = value;
               }
          }
     }
} //drawLinesOnGrid

template <typename T>
void UnivFFviaFM::drawLinesOnWall(std::vector<Line>& wallArg, T* const target, const T value) { //no init, plz init elsewhere

     for (auto& line : wallArg) {
          drawLinesOnWall(line, target, value);
     } //loop over all walls

} //drawLinesOnWall

template <typename T>
void UnivFFviaFM::drawLinesOnWall(Line& line, T* const target, const T value) { //no init, plz init elsewhere
// i~x; j~y;
//http://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm
//src in answer of "Avi"; adapted to fit this application

     //grid handeling local vars:
     long int iMax  = _grid->GetiMax();

     long int iStart, iEnd;
     long int jStart, jEnd;
     long int iDot, jDot;
     long int key;
     long int deltaX, deltaY, deltaX1, deltaY1, px, py, xe, ye, i; //Bresenham Algorithm


     key = _grid->getKeyAtPoint(line.GetPoint1());
     iStart = (long) _grid->get_i_fromKey(key);
     jStart = (long) _grid->get_j_fromKey(key);

     key = _grid->getKeyAtPoint(line.GetPoint2());
     iEnd = (long) _grid->get_i_fromKey(key);
     jEnd = (long) _grid->get_j_fromKey(key);

     deltaX = (int) (iEnd - iStart);
     deltaY = (int) (jEnd - jStart);
     deltaX1 = abs( (int) (iEnd - iStart));
     deltaY1 = abs( (int) (jEnd - jStart));

     px = 2*deltaY1 - deltaX1;
     py = 2*deltaX1 - deltaY1;

     if(deltaY1<=deltaX1) {
          if(deltaX>=0) {
               iDot = iStart;
               jDot = jStart;
               xe = iEnd;
          } else {
               iDot = iEnd;
               jDot = jEnd;
               xe = iStart;
          }
          if ((_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
               target[jDot * iMax + iDot] = value;
          }
          for (i=0; iDot < xe; ++i) {
               ++iDot;
               if(px<0) {
                    px+=2*deltaY1;
               } else {
                    if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY>0)) {
                         ++jDot;
                    } else {
                         --jDot;
                    }
                    px+=2*(deltaY1-deltaX1);
               }
               if ((_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
                    target[jDot * iMax + iDot] = value;
               }
          }
     } else {
          if(deltaY>=0) {
               iDot = iStart;
               jDot = jStart;
               ye = jEnd;
          } else {
               iDot = iEnd;
               jDot = jEnd;
               ye = jStart;
          }
          if ((_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
               target[jDot * iMax + iDot] = value;
          }
          for(i=0; jDot<ye; ++i) {
               ++jDot;
               if (py<=0) {
                    py+=2*deltaX1;
               } else {
                    if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY>0)) {
                         ++iDot;
                    } else {
                         --iDot;
                    }
                    py+=2*(deltaX1-deltaY1);
               }
               if ((_gridCode[jDot*iMax + iDot] != CLOSED_CROSSING) && (_gridCode[jDot*iMax + iDot] != CLOSED_TRANSITION)) {
                    target[jDot * iMax + iDot] = value;
               }
          }
     }
} //drawLinesOnWall

void UnivFFviaFM::calcFF(double* costOutput, Point* directionOutput, const double *const speed) {
     CompareCost comp = CompareCost(costOutput);
     std::priority_queue<long int, std::vector<long int>, CompareCost> trialfield(costOutput); //pass the argument for the constr of CompareCost
     std::priority_queue<long int, std::vector<long int>, CompareCost> trialfield2(comp);      //pass the CompareCost object directly

     directNeighbor local_neighbor = _grid->getNeighbors(0);
     long int aux = 0;
     //init trial field
     for (long int i = 0; i < _nPoints; ++i) {
          if (costOutput[i] == 0.0) {
               //check for negative neighbours, calc that ones and add to queue trialfield
               local_neighbor = _grid->getNeighbors(i);

               //check for valid neigh
               aux = local_neighbor.key[0];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcCost(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[1];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcCost(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[2];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcCost(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[3];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcCost(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
          }
     }

     while(!trialfield.empty()) {
          local_neighbor = _grid->getNeighbors(trialfield.top());
          trialfield.pop();

          //check for valid neigh
          aux = local_neighbor.key[0];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcCost(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[1];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcCost(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[2];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcCost(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[3];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcCost(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
     }
}

void UnivFFviaFM::calcCost(const long int key, double* cost, Point* dir, const double* const speed) {
     //adapt from calcFloorfield
     double row = DBL_MAX;
     double col = DBL_MAX;
     long int aux = -1; //will be set below
     bool pointsUp = false;
     bool pointsRight = false;

     directNeighbor dNeigh = _grid->getNeighbors(key);

     aux = dNeigh.key[0];
     //hint: trialfield[i].cost = dist2Wall + i; <<< set in resetGoalAndCosts
     if  ((aux != -2) &&                                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE)) &&  //gridpoint holds a calculated value
          (_gridCode[aux] != WALL))                                                              //gridpoint holds a calculated value
     {
          row = cost[aux];
          pointsRight = true;
          if (row < 0) {
               std::cerr << "hier ist was schief " << row << " " << aux << " " <<  std::endl;
               row = DBL_MAX;
          }
     }
     aux = dNeigh.key[2];
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE)) &&  //gridpoint holds a calculated value
          (_gridCode[aux] != WALL) &&
          (cost[aux] < row))                                       //calculated value promises smaller cost
     {
          row = cost[aux];
          pointsRight = false;
     }

     aux = dNeigh.key[1];
     //hint: trialfield[i].cost = dist2Wall + i; <<< set in parseBuilding after linescan call
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE)) &&  //gridpoint holds a calculated value
          (_gridCode[aux] != WALL))
     {
          col = cost[aux];
          pointsUp = true;
          if (col < 0) {
               std::cerr << "hier ist was schief " << col << " " << aux << " "  << std::endl;
               col = DBL_MAX;
          }
     }
     aux = dNeigh.key[3];
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE)) &&  //gridpoint holds a calculated value
          (_gridCode[aux] != WALL) &&
          (cost[aux] < col))                                       //calculated value promises smaller cost
     {
          col = cost[aux];
          pointsUp = false;
     }
     if (col == DBL_MAX) { //one sided update with row
          cost[key] = onesidedCalc(row, _grid->Gethx()/speed[key]);
          //flag[key] = FM_SINGLE;
          if (pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (0.);
          } else {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (0.);
          }
          dir[key] = dir[key].Normalized(); //@todo: ar.graf: what yields better performance? scale every point here or scale each read value? more points or more calls to any element of dir2Wall
          return;
     }

     if (row == DBL_MAX) { //one sided update with col
          cost[key] = onesidedCalc(col, _grid->Gethy()/speed[key]);
          //flag[key] = FM_SINGLE;
          if (pointsUp) {
               dir[key]._x = (0.);
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          } else {
               dir[key]._x = (0.);
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
          dir[key] = dir[key].Normalized();
          return;
     }

     //two sided update
     double precheck = twosidedCalc(row, col, _grid->Gethx()/speed[key]);
     if (precheck >= 0) {
          cost[key] = precheck;
          //flag[key] = FM_DOUBLE;
          if (pointsUp && pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          }
          if (pointsUp && !pointsRight) {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          }
          if (!pointsUp && pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
          if (!pointsUp && !pointsRight) {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
     } else {
          std::cerr << "else in twosided Dist " << std::endl;
     }
     dir[key] = dir[key].Normalized();
}

void UnivFFviaFM::calcDF(double* costOutput, Point* directionOutput, const double *const speed) {
     CompareCost comp = CompareCost(costOutput);
     std::priority_queue<long int, std::vector<long int>, CompareCost> trialfield(costOutput); //pass the argument for the constr of CompareCost
     std::priority_queue<long int, std::vector<long int>, CompareCost> trialfield2(comp);      //pass the CompareCost object directly

     directNeighbor local_neighbor = _grid->getNeighbors(0);
     long int aux = 0;
     //init trial field
     for (long int i = 0; i < _nPoints; ++i) {
          if (costOutput[i] == 0.0) {
               //check for negative neighbours, calc that ones and add to queue trialfield
               local_neighbor = _grid->getNeighbors(i);

               //check for valid neigh
               aux = local_neighbor.key[0];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcDist(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[1];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcDist(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[2];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcDist(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
               aux = local_neighbor.key[3];
               if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
                    calcDist(aux, costOutput, directionOutput, speed);
                    trialfield.emplace(aux);
                    trialfield2.emplace(aux);
               }
          }
     }

     while(!trialfield.empty()) {
          local_neighbor = _grid->getNeighbors(trialfield.top());
          trialfield.pop();

          //check for valid neigh
          aux = local_neighbor.key[0];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcDist(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[1];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcDist(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[2];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcDist(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
          aux = local_neighbor.key[3];
          if ((aux != -2) && (_gridCode[aux] != WALL) && (costOutput[aux] < 0.0)) {
               calcDist(aux, costOutput, directionOutput, speed);
               trialfield.emplace(aux);
               trialfield2.emplace(aux);
          }
     }
}

void UnivFFviaFM::calcDist(const long int key, double* cost, Point* dir, const double* const speed) {
     //adapt from calcFloorfield
     double row = DBL_MAX;
     double col = DBL_MAX;
     long int aux = -1; //will be set below
     bool pointsUp = false;
     bool pointsRight = false;

     directNeighbor dNeigh = _grid->getNeighbors(key);

     aux = dNeigh.key[0];
     //hint: trialfield[i].cost = dist2Wall + i; <<< set in resetGoalAndCosts
     if  ((aux != -2) &&                                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE))     //gridpoint holds a calculated value
          )                                                              //gridpoint holds a calculated value
     {
          row = cost[aux];
          pointsRight = true;
          if (row < 0) {
               std::cerr << "hier ist was schief " << row << " " << aux << " " <<  std::endl;
               row = DBL_MAX;
          }
     }
     aux = dNeigh.key[2];
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE))   //gridpoint holds a calculated value
           &&
          (cost[aux] < row))                                       //calculated value promises smaller cost
     {
          row = cost[aux];
          pointsRight = false;
     }

     aux = dNeigh.key[1];
     //hint: trialfield[i].cost = dist2Wall + i; <<< set in parseBuilding after linescan call
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE))   //gridpoint holds a calculated value
          )
     {
          col = cost[aux];
          pointsUp = true;
          if (col < 0) {
               std::cerr << "hier ist was schief " << col << " " << aux << " "  << std::endl;
               col = DBL_MAX;
          }
     }
     aux = dNeigh.key[3];
     if  ((aux != -2) &&                                                         //neighbor is a gridpoint
          (cost[aux] != magicnum(UNKNOWN_COST)) && (cost[aux] != magicnum(UNKNOWN_DISTANCE)) &&  //gridpoint holds a calculated value

          (cost[aux] < col))                                       //calculated value promises smaller cost
     {
          col = cost[aux];
          pointsUp = false;
     }
     if (col == DBL_MAX) { //one sided update with row
          cost[key] = onesidedCalc(row, _grid->Gethx()/speed[key]);
          //flag[key] = FM_SINGLE;
          if (pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (0.);
          } else {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (0.);
          }
          dir[key] = dir[key].Normalized(); //@todo: ar.graf: what yields better performance? scale every point here or scale each read value? more points or more calls to any element of dir2Wall
          return;
     }

     if (row == DBL_MAX) { //one sided update with col
          cost[key] = onesidedCalc(col, _grid->Gethy()/speed[key]);
          //flag[key] = FM_SINGLE;
          if (pointsUp) {
               dir[key]._x = (0.);
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          } else {
               dir[key]._x = (0.);
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
          dir[key] = dir[key].Normalized();
          return;
     }

     //two sided update
     double precheck = twosidedCalc(row, col, _grid->Gethx()/speed[key]);
     if (precheck >= 0) {
          cost[key] = precheck;
          //flag[key] = FM_DOUBLE;
          if (pointsUp && pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          }
          if (pointsUp && !pointsRight) {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (-(cost[key+(_grid->GetiMax())]-cost[key])/_grid->Gethy());
          }
          if (!pointsUp && pointsRight) {
               dir[key]._x = (-(cost[key+1]-cost[key])/_grid->Gethx());
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
          if (!pointsUp && !pointsRight) {
               dir[key]._x = (-(cost[key]-cost[key-1])/_grid->Gethx());
               dir[key]._y = (-(cost[key]-cost[key-(_grid->GetiMax())])/_grid->Gethy());
          }
     } else {
          std::cerr << "else in twosided Dist " << std::endl;
     }
     dir[key] = dir[key].Normalized();
}

inline double UnivFFviaFM::onesidedCalc(double xy, double hDivF) {
     //if ( (xy+hDivF) > 10000) std::cerr << "error in onesided " << xy << std::endl;
     return xy + hDivF;
}

inline double UnivFFviaFM::twosidedCalc(double x, double y, double hDivF) { //on error return -2
     double determinante = (2*hDivF*hDivF - (x-y)*(x-y));
     if (determinante >= 0) {
          return (x + y + sqrt(determinante))/2;
     } else {
          return (x < y) ? (x + hDivF) : (y + hDivF);
     }
     std::cerr << "error in two-sided 2!!!!!!!!!!!!!!!!!!!!!!! o_O??" << std::endl;
     return -2.; //this line should never execute
} //twosidedCalc

void UnivFFviaFM::addTarget(const int uid) {
     Line tempTargetLine = Line(_doors[uid]);
     Point tempCenterPoint = Point(tempTargetLine.GetCentre());

     //this allocation must be on shared heap! to be accessible by any thread later (should be shared in openmp)
     double* newArrayDBL = new double[_nPoints];
     Point* newArrayPt = nullptr;
     if (_user == DISTANCE_AND_DIRECTIONS_USED) {
          newArrayPt = new Point[_nPoints];
     }

     if (_costFieldWithKey[uid])
          delete[] _costFieldWithKey[uid];
     _costFieldWithKey[uid] = newArrayDBL;

     //init costarray
     for (int i = 0; i < _nPoints; ++i) {
          if (_gridCode[i] == WALL) {
               newArrayDBL[i] = magicnum(WALL_ON_COSTARRAY);
          } else {
               newArrayDBL[i] = magicnum(UNKNOWN_COST);
          }
     }

     if (_directionFieldWithKey[uid])
          delete[] _directionFieldWithKey[uid];
     if (newArrayPt)
          _directionFieldWithKey[uid] = newArrayPt;

     //initialize start area
     if (_mode == LINESEGMENT) {
          drawLinesOnGrid(tempTargetLine, newArrayDBL, magicnum(TARGET_REGION));
     }
     if (_mode == CENTERPOINT) {
          newArrayDBL[_grid->getKeyAtPoint(tempCenterPoint)] = magicnum(TARGET_REGION);
     }

     if (_speedmode == FF_WALL_AVOID) {
          calcFF(newArrayDBL, newArrayPt, _speedFieldSelector[REDU_WALL_SPEED]);
     } else if (_speedmode == FF_HOMO_SPEED) {
          calcFF(newArrayDBL, newArrayPt, _speedFieldSelector[INITIAL_SPEED]);
     }
     _uids.emplace_back(uid);
}

void UnivFFviaFM::addAllTargets() {
     for (auto uidmap : _doors) {
          addTarget(uidmap.first);
     }
}

std::vector<int> UnivFFviaFM::getKnownDoorUIDs(){
     return _uids;
}

void UnivFFviaFM::writeFF(const std::string& filename, std::vector<int> targetID) {
    Log->Write("INFO: \tWrite Floorfield to file");
    Log->Write(filename);
    std::ofstream file;

    Log->Write("FloorfieldViaFM::writeFF(): writing to file %s: There are %d targets.", filename.c_str(), targetID.size());

    int numX = (int) ((_grid->GetxMax()-_grid->GetxMin())/_grid->Gethx());
    int numY = (int) ((_grid->GetyMax()-_grid->GetyMin())/_grid->Gethy());
    int numTotal = numX * numY;
    //std::cerr << numTotal << " numTotal" << std::endl;
    //std::cerr << grid->GetnPoints() << " grid" << std::endl;
    file.open(filename);

    file << "# vtk DataFile Version 3.0" << std::endl;
    file << "Testdata: Fast Marching: Test: " << std::endl;
    file << "ASCII" << std::endl;
    file << "DATASET STRUCTURED_POINTS" << std::endl;
    file << "DIMENSIONS " <<
                                std::to_string(_grid->GetiMax()) <<
                                " " <<
                                std::to_string(_grid->GetjMax()) <<
                                " 1" << std::endl;
    file << "ORIGIN " << _grid->GetxMin() << " " << _grid->GetyMin() << " 0" << std::endl;
    file << "SPACING " << std::to_string(_grid->Gethx()) << " " << std::to_string(_grid->Gethy()) << " 1" << std::endl;
    file << "POINT_DATA " << std::to_string(numTotal) << std::endl;
    file << "SCALARS GCode float 1" << std::endl;
    file << "LOOKUP_TABLE default" << std::endl;
    if (!_gridCode) {
         return;
    }
    for (long int i = 0; i < _grid->GetnPoints(); ++i) {
         file << _gridCode[i] << std::endl;
    }

    if (_directionFieldWithKey[0]) {
         file << "VECTORS Dir2Wall float" << std::endl;
         for (long int i = 0; i < _grid->GetnPoints(); ++i) {
              file << _directionFieldWithKey[0][i]._x << " " << _directionFieldWithKey[0][i]._y << " 0.0" << std::endl;
         }

         file << "SCALARS Dist2Wall float 1" << std::endl;
         file << "LOOKUP_TABLE default" << std::endl;
         for (long int i = 0; i < _grid->GetnPoints(); ++i) {
              file << _costFieldWithKey[0][i] << std::endl; //@todo: change target to all dist2wall
         }
    }

//    file << "SCALARS SubroomPtr float 1" << std::endl;
//    file << "LOOKUP_TABLE default" << std::endl;
//    for (long int i = 0; i < _grid->GetnPoints(); ++i) {
//        if (_subrooms[i]) {
//            file << _subrooms[i]->GetUID() << std::endl;
//        } else {
//            file << 0.0 << std::endl;
//        }
//    }
    if (!targetID.empty()) {
         for (unsigned int iTarget = 0; iTarget < targetID.size(); ++iTarget) {
              Log->Write("%s: target number %d: UID %d", filename.c_str(), iTarget, targetID[iTarget]);
              if (_directionFieldWithKey.count(targetID[iTarget]) == 0) {
                   continue;
              }

              Point *gradarray = _directionFieldWithKey[targetID[iTarget]];
              if (gradarray == nullptr) {
                   continue;
              }

              std::string name = _building->GetTransOrCrossByUID(targetID[iTarget])->GetCaption() + "-" +
                                 std::to_string(targetID[iTarget]);
              std::replace(name.begin(), name.end(), ' ', '_');
              file << "VECTORS GradientTarget" << name << " float" << std::endl;
              for (int i = 0; i < _grid->GetnPoints(); ++i) {
                   file << gradarray[i]._x << " " << gradarray[i]._y << " 0.0" << std::endl;
              }

              double *costarray = _costFieldWithKey[targetID[iTarget]];
              file << "SCALARS CostTarget" << name << " float 1" << std::endl;
              file << "LOOKUP_TABLE default" << std::endl;
              for (long int i = 0; i < _grid->GetnPoints(); ++i) {
                   file << costarray[i] << std::endl;
              }
         }
    }
    file.close();
}