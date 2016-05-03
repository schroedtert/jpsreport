/**
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
 **/
//
// Created by laemmel on 30.03.16.
//

#ifndef JPSCORE_GEOFILEPARSER_H
#define JPSCORE_GEOFILEPARSER_H

#include "../general/Configuration.h"
#include "../geometry/Building.h"
#include "../geometry/GeometryReader.h"

//TODO: the class name GeoFileParser is misleading as the ``geometry'' file contains among others also relations (transitions)
//TODO: between geometries/rooms. Probably, EnvironmentFileParser would be better, still parts of the environment are
//TODO: currently stored in the ``ini'' file (e.g. goals) [gl march '16]
//TODO: currently stored in the ``ini'' file (e.g. goals) [gl march '16]
class GeoFileParser : public GeometryReader {

public:
     GeoFileParser(const Configuration* configuration);

     virtual void LoadBuilding(Building* building) override;

     virtual bool LoadTrafficInfo(Building* building) override;

private:
     const Configuration* _configuration;

     bool LoadGeometry(Building* building);

     bool LoadRoutingInfo(Building* filename);
};

#endif //JPSCORE_GEOFILEPARSER_H
