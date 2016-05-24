/**
 * \file        WalkingSpeed.cpp
 * \date        Jan 1, 2014
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
 * detects smoke in the next rooms
 *
 *
 **/

#include "WalkingSpeed.h"
#include "../../geometry/Building.h"
#include "../../pedestrian/Pedestrian.h"
#include "../generic/FDSMesh.h"
#include "../generic/FDSMeshStorage.h"
#include "../../pedestrian/PedDistributor.h"
#include "../../tinyxml/tinyxml.h"
#include <set>
#include <algorithm>
#include <math.h>
#include <string>

WalkingSpeed::WalkingSpeed(const Building * b)
{
    _FMStorage = nullptr;
    _building = b;
    LoadJPSfireInfo(b->GetProjectFilename());
}

WalkingSpeed::~WalkingSpeed()
{
}



bool WalkingSpeed::LoadJPSfireInfo(const std::string &projectFilename )
{
   TiXmlDocument doc(projectFilename);
   if (!doc.LoadFile()) {
        Log->Write("ERROR: \t%s", doc.ErrorDesc());
        Log->Write("ERROR: \t could not parse the project file");
        return false;
   }

   TiXmlNode* JPSfireNode = doc.RootElement()->FirstChild("JPSfire");
   if( ! JPSfireNode ) {
        Log->Write("INFO:\tcould not find any JPSfire information");
        return true;
   }

   TiXmlElement* JPSfireCompElem = JPSfireNode->FirstChildElement("B_walking_speed");
   if(JPSfireCompElem) {
       if(JPSfireCompElem->FirstAttribute()){
           std::string study = xmltoa(JPSfireCompElem->Attribute("study"), "");
           std::string irritant = xmltoa(JPSfireCompElem->Attribute("irritant"), "");
           //std::string filepath = xmltoa(JPSfireCompElem->Attribute("extinction_grids"), "");
           std::string filepath = _building->GetProjectRootDir() + xmltoa(JPSfireCompElem->Attribute("extinction_grids"), "");
           double updateIntervall = xmltof(JPSfireCompElem->Attribute("update_time"), 0.);
           double finalTime = xmltof(JPSfireCompElem->Attribute("final_time"), 0.);
           Log->Write("INFO:\tJPSfire Module B_walking_speed: \n \tstudy: %s \n\tdata: %s \n\tupdate time: %.1f s | final time: %.1f s | irritant: %s",
                      study.c_str(), filepath.c_str(), updateIntervall, finalTime, irritant.c_str());
           _FMStorage = std::make_shared<FDSMeshStorage>(filepath, finalTime, updateIntervall, study, irritant);
           return true;
       }
   }
   return false;
}


std::string WalkingSpeed::GetName() const
{
    return "WalkingSpeed";
}


double WalkingSpeed::GetExtinction(const Pedestrian * pedestrian)
{
    std::string quantity = "SOOT_EXTINCTION_COEFFICIENT";
    double ExtinctionCoefficient = _FMStorage->GetFDSMesh(pedestrian->GetGlobalTime(), pedestrian->GetElevation(), quantity).GetKnotValue(pedestrian->GetPos()._x , pedestrian->GetPos()._y);
 return ExtinctionCoefficient;
}

void WalkingSpeed::set_FMStorage(const std::shared_ptr<FDSMeshStorage> fmStorage)
{
    _FMStorage=fmStorage;
}

const std::shared_ptr<FDSMeshStorage> WalkingSpeed::get_FMStorage()
{
    return _FMStorage;
}

double WalkingSpeed::FrantzichNilsson2003(double &walking_speed, double ExtinctionCoefficient)
{
    //According to Frantzich+Nilsson2003
    walking_speed = std::fmax(0.3, walking_speed * (1 + (-0.057 / 0.706) * ExtinctionCoefficient) );
    return walking_speed;
}

double WalkingSpeed::Jin1978(double &walking_speed, double ExtinctionCoefficient)
{
    //According to Jin1978
    std::string irritant = _FMStorage->IrritantOrNot();
    if(irritant=="false") {
        walking_speed = std::fmax(0.3, walking_speed * ( -0.54991616 * std::pow(ExtinctionCoefficient, 3) +
                                 -0.05957671 * std::pow(ExtinctionCoefficient, 2)
                                 -0.06606845 * ExtinctionCoefficient + 1.0025715) );
    }
    else if(irritant=="true") {
        walking_speed = std::fmax(0.3, walking_speed *
                                 (- std::pow( 112236.0553, (ExtinctionCoefficient-0.532027513) ) + 0.988158598 ));
    }
    else {
        Log->Write("ERROR:\tSpecify if irritant or non-irritant smoke shall be cosidered");
        exit(EXIT_FAILURE);
    }
    return walking_speed;
}

double WalkingSpeed::WalkingInSmoke(const Pedestrian* p, double walking_speed)
{
    double ExtinctionCoefficient = GetExtinction(p);
    //std::cout << ExtinctionCoefficient << std::endl;
    //fprintf(stderr, "%f\n", ExtinctionCoefficient);
    std::string study = _FMStorage->GetStudy();

    if((ExtinctionCoefficient < 10E-6) || (std::isnan(ExtinctionCoefficient)))   //no obstruction by smoke or NaN check
    {
        //fprintf(stderr, "%f \t%f\n", ExtinctionCoefficient, p->GetEllipse().GetV0());
        return walking_speed;    // walking_speed is returned as V0 for plane or stairs
    }
    else {
            if (study=="Frantzich+Nilsson2003"){
                walking_speed = FrantzichNilsson2003(walking_speed, ExtinctionCoefficient);
            }
            else if (study=="Jin1978"){
                walking_speed = Jin1978(walking_speed, ExtinctionCoefficient);
            }
            else {
                Log->Write("ERROR:\tNo study specified");
                exit(EXIT_FAILURE);
            }
    //Generally check if v0 < reduced walking_speed
//    if(walking_speed > p->GetEllipse().GetV0()) {
//       walking_speed = p->GetEllipse().GetV0();
//    }
    }
   //fprintf(stderr, "%f \t%f \t%f \t%f \n", p->GetGlobalTime(), ExtinctionCoefficient, walking_speed, p->GetV().Norm() );

   return walking_speed;
}

bool WalkingSpeed::ReduceWalkingSpeed()
{
   return _FMStorage!=nullptr;
}
