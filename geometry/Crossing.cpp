/**
 * \file        Crossing.cpp
 * \date        Nov 16, 2010
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


#include "Crossing.h"
#include "SubRoom.h"

using namespace std;


Crossing::Crossing()
{
     _id = -1;
     _isOpen=true;
     _temporaryClosed=false; // some doors have to be closed and opened again
                             // depending on _outflowRate.
     _doorUsage = 0;
     _maxDoorUsage = (std::numeric_limits<int>::max)(); //avoid name conflicts in windows winmindef.h
     _outflowRate = (std::numeric_limits<double>::max)();
     _lastPassingTime = 0;
     _lastFlowMeasurement = 0;
     _DT = 1;
     _DN = 1;
     _partialDoorUsage = 0;
     _closingTime = 0;

}

Crossing::~Crossing()
{
}

//void Crossing::SetID(int ID)
//{
//     _id = ID;
//}

//void Crossing::SetSubRoom2(SubRoom* r2)
//{
//     _subRoom2 = r2;
//}

// Getter-Funktionen

//int Crossing::GetID() const
//{
//     return _id;
//}

//SubRoom* Crossing::GetSubRoom2() const
//{
//     return _subRoom2;
//}
// Sonstiges


bool Crossing::IsExit() const
{
     return false;
}


bool Crossing::IsOpen() const
{
     return _isOpen;
}

bool Crossing::IsTransition() const
{
     return false;
}

void Crossing::Close()
{
     _isOpen = false;
}

void Crossing::Open()
{
     _isOpen = true;
}

bool Crossing::IsInSubRoom(int subroomID) const
{
     bool r1, r2;
     if (_subRoom1 != nullptr)
          r1 = _subRoom1->GetSubRoomID() == subroomID;
     else
          r1 = false;
     if (_subRoom2 != nullptr)
          r2 = _subRoom2->GetSubRoomID() == subroomID;
     else
          r2 = false;
     return (r1 || r2);
}

/* gibt den ANDEREN Subroom != subroomID zurück
 * roomID wird hier nicht benötigt, aber in Transition::GetOtherSubRoom()
 * (virtuelle Funktion) */
SubRoom* Crossing::GetOtherSubRoom(int roomID, int subroomID) const
{
     if (_subRoom1->GetSubRoomID() == subroomID)
          return _subRoom2;
     else if (_subRoom2->GetSubRoomID() == subroomID)
          return _subRoom1;
     else {
          Log->Write("WARMING: \tCrossing::GetOtherSubRoom No exit found "
                     "on the other side\n ID=%hd, roomID=%hd, subroomID=%hd\n",GetID(),roomID,subroomID);
          return nullptr;
     }
}


// Ausgabe
void Crossing::WriteToErrorLog() const
{
     string s;
     char tmp[CLENGTH];
     sprintf(tmp, "\t\tCROSS: %d (%f, %f) -- (%f, %f)\n", GetID(), GetPoint1()._x,
             GetPoint1()._y, GetPoint2()._x, GetPoint2()._y);
     s.append(tmp);
     sprintf(tmp, "\t\t\t\tSubRoom: %d <-> SubRoom: %d\n", GetSubRoom1()->GetSubRoomID(),
             GetSubRoom2()->GetSubRoomID());
     s.append(tmp);
     Log->Write(s);
}

// TraVisTo Ausgabe
string Crossing::GetDescription() const
{
     //return "";
     string geometry;
     char tmp[CLENGTH] = "";
     sprintf(tmp,"\t\t<crossing ID=\"%d\" color = \"250\" caption=\"%d_%d\">\n",GetUniqueID(),GetID(),GetUniqueID());
     geometry.append(tmp);
     //geometry.append("\t\t<door color=\"250\">\n");
     sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\" zPos=\"%.2f\" />\n",
             (GetPoint1()._x) * FAKTOR,
             (GetPoint1()._y) * FAKTOR,
             _subRoom1->GetElevation(GetPoint1())*FAKTOR);
     geometry.append(tmp);
     sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\" zPos=\"%.2f\" />\n",
             (GetPoint2()._x) * FAKTOR,
             (GetPoint2()._y) * FAKTOR,
             _subRoom1->GetElevation(GetPoint2())*FAKTOR);
     geometry.append(tmp);
     geometry.append("\t\t</crossing>\n");
     return geometry;
}

int Crossing::CommonSubroomWith(Crossing* other, SubRoom* &subroom) {
     int result = 0;
     if (_subRoom1 && (_subRoom1 == other->_subRoom1 || _subRoom1 == other->_subRoom2)) {
          ++result;
          subroom = _subRoom1;
     }
     if (_subRoom2 && (_subRoom2 == other->_subRoom1 || _subRoom2 == other->_subRoom2)) {
          ++result;
          subroom = _subRoom2;
     }
     return result;
}

void Crossing::IncreaseDoorUsage(int number, double time)
{

     _doorUsage += number;
     _lastPassingTime = time;
     _flowAtExit += to_string(time) + "  " + to_string(_doorUsage) + "\n";
}

void Crossing::IncreasePartialDoorUsage(int number)
{
     _partialDoorUsage += number;
}
void Crossing::ResetPartialDoorUsage()
{
     _partialDoorUsage = 0;
}

int Crossing::GetDoorUsage() const
{
     return _doorUsage;
}

int Crossing::GetPartialDoorUsage() const
{
     return _partialDoorUsage;
}


int Crossing::GetMaxDoorUsage() const
{
     return  _maxDoorUsage;
}


double Crossing::GetOutflowRate() const
{
     return _outflowRate;
}


double Crossing::GetLastPassingTime() const
{
     return _lastPassingTime;
}

const std::string & Crossing::GetFlowCurve() const
{
     return _flowAtExit;
}

void Crossing::SetOutflowRate(double outflow)
{
     _outflowRate = outflow;
}

void Crossing::SetMaxDoorUsage(int mdu)
{
     _maxDoorUsage = mdu;
}

double Crossing::GetClosingTime() const
{
     return _closingTime;
}

void Crossing::UpdateClosingTime(double dt)
{
     _closingTime -= dt;
}

bool Crossing::isTemporaryClosed()
{
     return  _temporaryClosed;
}


double Crossing::GetDT()
{
     return _DT;
}

void Crossing::SetDT(double dt)
{
     _DT = dt;
}
int Crossing::GetDN()
{
     return _DN;
}

void Crossing::SetDN(int dn)
{
     _DN = dn;
}

// changes:
// - _lasFlowMeasurement
// - _closingTime
// - state of door (close/open)
// - _temporaryClosed (false if maxDoorUsage is reached)
void Crossing::regulateFlow(double time)
{
     double number = GetPartialDoorUsage();
     double T = time -  _lastFlowMeasurement;
     double flow =  number / T;
     if(flow >  _outflowRate)
     {
          // _outflowRate > flow (=number/deltaTime)
          // _outflowRate = number/(deltaTime + t1)
          // --> [1]
          //---------------------------
          _closingTime = number / _outflowRate -  T; //[1]
           _temporaryClosed = true;















          this->Close();
          Log-> Write("INFO:\tClosing door %d. DoorUsage = %d (max = %d). Flow = %.2f (max =  %.2f) Time=%.2f", GetID(), GetDoorUsage(), GetMaxDoorUsage(), flow, _outflowRate, time);
     }

     // close the door is mdu is reached
     if(_doorUsage >= _maxDoorUsage)
     {
          Log-> Write("INFO:\tClosing door %d. DoorUsage = %d (>= %d). Time=%.2f", GetID(), GetDoorUsage(), GetMaxDoorUsage(), time);
          this->Close();
          _temporaryClosed = false;
     }
     _lastFlowMeasurement = time +  _closingTime;
}

void Crossing::changeTemporaryState()
{
       _temporaryClosed = false;
       _closingTime = 0;
       this->Open();
}
