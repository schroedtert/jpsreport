/**
 * \file        CognitiveMapStorage.h
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
 * Cognitive Map Storage
 *
 *
 **/


#ifndef COGNITIVEMAPSTORAGE_H_
#define COGNITIVEMAPSTORAGE_H_

#include <unordered_map>
#include <vector>
#include "./cognitiveMap/cognitivemap.h"
#include "./perception/visibleenvironment.h"

class Building;
class Pedestrian;
class CognitiveMap;
class AbstractCognitiveMapCreator;



typedef const Pedestrian * CMStorageKeyType;
typedef CognitiveMap * CMStorageValueType;
typedef std::unordered_map<CMStorageKeyType, CMStorageValueType> CMStorageType;



/**
 * @brief Cognitive Map Storage
 *
 * Cares about Cognitive map storage, creation and delivery
 *
 */
class CognitiveMapStorage {
public:
     CognitiveMapStorage(const Building * const b, std::string cogMapStatus, std::string cogMapFiles="");
     virtual ~CognitiveMapStorage();


     CMStorageValueType operator[] (CMStorageKeyType key);


private:
     const Building * const _building;
     CMStorageType cognitive_maps;
     AbstractCognitiveMapCreator * creator;


     //perception
     //Complete environment
     VisibleEnvironment _visibleEnv;

     //cognitive map
     std::vector<ptrRegion> _regions;
     std::string _cogMapStatus;
     std::string _cogMapFiles;


     void CreateCognitiveMap(CMStorageKeyType ped);
     void ParseCogMap(CMStorageKeyType ped);

};

#endif /* COGNITIVEMAPSTORAGE_H_ */
