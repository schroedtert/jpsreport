/**
 * \file        NavLineState.h
 * \date        Sep 18, 2012
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

#ifndef NAVLINESTATE_H_
#define NAVLINESTATE_H_

#include <time.h>
#include <iostream>

//time between a pedestrian got the information and uses the information
#define INFO_OFFSET 1.5

class NavLineState
{

public:
     NavLineState();
     ~NavLineState();

     bool closed();
     bool isShareable(double time);
     void close(double time);
     bool mergeDoor(NavLineState & orig, double time);
     void print();

     void SetState(bool is_closed, double time);
     bool GetState() const;
     int GetQuality() const;
     int GetTime() const;

private:
     bool open; // aka state
     int timeFirstSeen; // number of clocks till the door was seen changed the first time
     int timeOfInformation; // number of clocks when i got the information. should be set to zero after a period of time is over (to

     // information quality in [0..1]. 1 is very reliable information
     int _quality; //
     // last time the state was recorded.
     int _time;
     // state 0=open, 1=close
     bool _isClosed;

};
#endif /* ROUTINGGRAPHSTORAGE_H_ */

