/**
 * \file        FDSMeshStorage.h
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
 * Container to store all FDSMeshs. Sorted first by coordinates of the corresponding door,
 * secondly by simulation's global time
 *
 *
 **/
#include "FDSMeshStorage.h"
#include "../../IO/OutputHandler.h"
#include <unistd.h>
#include <stdio.h>
#include <glob.h>
#include <string>
//#include <boost/filesystem.hpp>
#if defined(_WIN64) || defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif


FDSMeshStorage::FDSMeshStorage()
{

}

FDSMeshStorage::FDSMeshStorage(const std::string &filepath, const double &finalTime, const double &updateIntervall, const std::string &study, const std::string &irritant) :
    _filepath(filepath), _finalTime(finalTime),
    _updateIntervall(updateIntervall), _study(study),
    _elevationlist(), _timelist(), _irritant(irritant)
{
    ///Check if _filepath is existent
    struct stat file_path;

    if ( stat(_filepath.c_str(), &file_path) != 0 )
    {
        Log->Write("ERROR:\tCould not find directory %s", _filepath.c_str());
        exit(EXIT_FAILURE);
    }
    else {
        CreateQuantityList();
        std::cout << "\nQuantityList PASSED\n" << std::endl;
        CreateElevationList();
        std::cout << "\nCreateElevationList PASSED\n" << std::endl;
        CreateDoorList();
        std::cout << "\nCreateDoorList PASSED\n" << std::endl;
        CreateTimeList();
        std::cout << "CreateTimeList PASSED\n" << std::endl;
        CreateFDSMeshes();
        //std::cout << "CreateFDSMeshes PASSED\n" << std::endl;
    }
}


FDSMeshStorage::~FDSMeshStorage()
{

}


bool FDSMeshStorage::CreateQuantityList()
{
    /// Create quantity list
    glob_t paths;
    int retval;

    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    const char * glob_str = (_filepath + "*").c_str();
    retval = glob( (glob_str) , GLOB_NOSORT, NULL, &paths );
    if( retval == 0 ) {
        int idx;
        for( idx = 0; idx < paths.gl_pathc; idx++ ) {
            std::string glob_str = (paths.gl_pathv[idx]);
            //std::cout << "\n" << paths.gl_pathv[idx] << std::endl;
            std::string quant_dir =  glob_str.substr( glob_str.rfind("/") + 1 );
            _quantitylist.push_back(quant_dir);
        }
        globfree( &paths );
        return true;
    }
    else {
        Log->Write("ERROR:\tCould not find suitable quantities in %s", _filepath.c_str());
        exit(EXIT_FAILURE);
    }
}


bool FDSMeshStorage::CreateElevationList()
{
    /// Create elevation list out of the available Z_* dirs for each quantity
    glob_t paths;
    int retval;

    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    const char * glob_str = (_filepath + _quantitylist[0] + "/Z_*").c_str();
    retval = glob( (glob_str) , GLOB_NOSORT, NULL, &paths );
    if( retval == 0 ) {
        int idx;
        for( idx = 0; idx < paths.gl_pathc; idx++ ) {
            std::string glob_str = (paths.gl_pathv[idx]);
            //std::cout << "\n" << paths.gl_pathv[idx] << std::endl;
            double elev_dir = std::stod( glob_str.substr( glob_str.rfind("_") + 1 ));
            _elevationlist.push_back(elev_dir);
        }
        globfree( &paths );
        return true;
    }
    else {
        Log->Write("ERROR:\tCould not find suitable grid elevations in %s", _filepath.c_str());
        exit(EXIT_FAILURE);
    }
}

void FDSMeshStorage::CreateDoorList()
{
    /// Create door list only neceassry if smoke sensor is active
    glob_t paths;
    int retval;

    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    const char * glob_str = (_filepath + _quantitylist[0] +
            "/Z_" + std::to_string(_elevationlist[0]) + "/Door*").c_str();
    retval = glob( (glob_str) , GLOB_NOSORT, NULL, &paths );
    if( retval == 0 ) {
        int idx;
        for( idx = 0; idx < paths.gl_pathc; idx++ ) {
            std::string glob_str = (paths.gl_pathv[idx]);
            std::cout << "\n" << paths.gl_pathv[idx] << std::endl;
            std::string door_dir =  glob_str.substr( glob_str.rfind("/") + 1 );
            _doorlist.push_back(door_dir);
        }
        globfree( &paths );
    }
}


void FDSMeshStorage::CreateTimeList()
{
    /// Create time list for mesh refreshment
    double i=0;
    while (i<=_finalTime)
    {
        _timelist.push_back(i);
        i+=_updateIntervall;
    }

    ///Check if specified final and update times are compliant with available data
    const char * check_str;
    struct stat times;
    for(auto elem : _timelist) {
        if (_doorlist.size() > 0) {     // Smoke sensor active
        check_str = (_filepath + _quantitylist[0] + "/Z_" + std::to_string(_elevationlist[0]) + "/" +
                _doorlist[0] + "/t_" + std::to_string(elem) + ".csv").c_str();
                std::cout << check_str << std::endl;
        }
        else if (_doorlist.size() == 0) {   // Smoke sensor not active
            check_str = (_filepath + _quantitylist[0] + "/Z_" +
                    std::to_string(_elevationlist[0]) + "/t_" + std::to_string(elem) + ".csv").c_str();
                    std::cout << check_str << std::endl;
        }

        if ( stat(check_str, &times) != 0 )
        {
            Log->Write("ERROR:\tSpecified times are not compliant with JPSfire data ", check_str);
            exit(EXIT_FAILURE);
        }
    }
}

void FDSMeshStorage::CreateFDSMeshes()
{
    _fMContainer.clear();
    if (_doorlist.size() > 0) {     // Smoke sensor active
        for (auto &h:_quantitylist)     //list of quantities
        {
            for (auto &i:_elevationlist)    //list of elevations
            {
                for (auto &j:_doorlist)         //list of doors
                {
                    //std::cout << "i " << i << std::endl;
                    for (auto &k:_timelist)         //list of times
                    {
                        //std::cout << "k " << j << std::endl;
                        std::string str = h + "/Z_" + std::to_string(i) +
                        "/" + j + "/t_"+std::to_string(k);
                        //std::cout << _filepath + str + ".csv" << std::endl;
                        FDSMesh mesh(_filepath + str + ".csv");
                        //std::string str = "t_"+std::to_string(i);
                        _fMContainer.insert(std::make_pair(str, mesh));
                    }
                }
            }
       }
   }
   else if (_doorlist.size() == 0) {     // Smoke sensor not active
        for (auto &h:_quantitylist)     //list of quantities
        {
            for (auto &i:_elevationlist)    //list of elevations
            {
                //std::cout << "i " << i << std::endl;
                for (auto &k:_timelist)         //list of times
                {
                    //std::cout << "k " << j << std::endl;
                    std::string str = h + "/Z_" + std::to_string(i) +
                    "/t_"+std::to_string(k);
                    //std::cout << _filepath + str + ".csv" << std::endl;
                    FDSMesh mesh(_filepath + str + ".csv");
                    //std::string str = "t_"+std::to_string(i);
                    _fMContainer.insert(std::make_pair(str, mesh));
                }
            }
       }
   }
}

const FDSMesh &FDSMeshStorage::GetFDSMesh(const double &simTime, const double &pedElev, const std::string &quantity)
{
    //Smoke Sensor NOT active
    int simT=simTime/_updateIntervall;
    simT*=_updateIntervall;
    _PedEyeHeight = pedElev + 1.8;
    GetNearestHeight(_PedEyeHeight);

    if (simT>=_finalTime)
        simT=_finalTime;

    //std::cout << "\t" << quantity << std::endl;

    std::string str = quantity + "/Z_" +  std::to_string(_NearestHeight) + "/t_"+std::to_string(simT)+".000000";

    //std::cout << str << std::endl;

    return _fMContainer.at(str);

//    TODO
//    if(_fMContainer.??(str) ) {
//        return _fMContainer.at(str);
//    }
//    else {
//        Log->Write("ERROR:\tCould find no appropriate FDS mesh: ", quantity.c_str(), pedElev, simT);
//        exit(EXIT_FAILURE);
    //    }
}

const FDSMesh &FDSMeshStorage::GetFDSMesh(const double &pedElev, const Point &doorCentre, const double &simTime)
{
    //Smoke Sensor active

    std::string quantity = "SOOT_EXTINCTION_COEFFICIENT";
    _PedEyeHeight = pedElev + 1.8;
    GetNearestHeight(_PedEyeHeight);

    int simT=simTime/_updateIntervall;
    simT*=_updateIntervall;

    if (simT>=_finalTime)
        simT=_finalTime;

    std::string str = quantity + "/Z_" +  std::to_string(_NearestHeight) + "/" +
    "Door_X_"+ std::to_string(doorCentre._x) + "_Y_" + std::to_string(doorCentre._y) +
    "/t_"+std::to_string(simT)+".000000";

    std::cout << str << std::endl;

    return _fMContainer.at(str);
}

double FDSMeshStorage::GetNearestHeight(double _PedEyeHeight)
{
    ///find the nearest height in the JPSfire data related to the ped elevation
    double min_val = std::numeric_limits<double>::max();
    int index = 0;

    for(int i=0;i < _elevationlist.size() ;++i) {
        double diff = std::abs(_elevationlist[i] - _PedEyeHeight);
        if(diff < min_val) {
            min_val = diff;
            index = i;
        }
    }
    _NearestHeight = _elevationlist[index];
    //std::cout << "NEAREST" << std::endl << _NearestHeight << std::endl;
    return _NearestHeight;
}

std::string FDSMeshStorage::GetStudy() const
{
    return _study;
}

std::string FDSMeshStorage::IrritantOrNot() const
{
    return _irritant;
}
