/**
 * @file    GlobalRouter.h
 * @date Created on: Dec 15, 2010
 * Copyright (C) <2009-2011>
 *
 * @section LICENSE
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * implement the global shortest path using the dijkstra algorithm
 *
 *
 */

#ifndef GLOBALROUTER_H_
#define GLOBALROUTER_H_

#include <string.h>
#include <vector>
#include "Router.h"
#include "../geometry/Building.h"

// forwarded classes
class Pedestrian;
class AccessPoint;
class Building;
class OutputHandler;

//log output
extern OutputHandler* Log;



class GlobalRouter: public Router {

public:
	GlobalRouter();
	virtual ~GlobalRouter();

	virtual void Init(Building* building);


	virtual int FindExit(Pedestrian* p);

	/**
	 * Performs a check of the geometry and fixes if possible.
	 * NOT IMPLEMENTED
	 */
	void CheckInconsistencies();

	/**
	 * write the graph as GV format to be used with graphviz
	 * @param filename
	 */
	void WriteGraphGV(std::string filename, int finalDestination,
			const std::vector<std::string> rooms= std::vector<std::string>());



protected:
	void DumpAccessPoints(int p=-1);

	/**
	 * @return true if the two points are in the visibility range of each other
	 * @note based on http://alienryderflex.com/intersect/
	 */
	bool CanSeeEachother(const Point&pt1, const Point&pt2);

	/**
	 * @return true if the two segments are in the visibility range of each other
	 */
	bool CanSeeEachOther(Crossing* c1, Crossing* c2);

	/**
	 * @obsolete
	 * return a random exit
	 */
	int GetBestDefaultRandomExit(Pedestrian* p);

	/**
	 * @return the subroom which contains both crossings.
	 *  Null is return is there is no such subroom.
	 */
	SubRoom* GetCommonSubRoom(Crossing* c1, Crossing* c2);

	/**
	 * @return true if the element is present in the vector
	 */
	template<typename A>
	bool IsElementInVector(const std::vector<A> &vec, A& el) {
		typename std::vector<A>::const_iterator it;
		it = std::find (vec.begin(), vec.end(), el);
		if(it==vec.end()){
			return false;
		}else{
			return true;
		}
	}

	/**
	 * Implementation of a map with a default value.
	 * @return the default value if the element was not found in the map
	 */
	template <typename K, typename V>
	V GetWithDef(const  std::map <K,V> & m, const K & key, const V & defval ) {
	   typename std::map<K,V>::const_iterator it = m.find( key );
	   if ( it == m.end() ) {
	      return defval;
	   }
	   else {
	      return it->second;
	   }
	}


private:
	/**
	 * Compute the intermediate paths between the two given transitions IDs
	 */
	void GetPath(int transID1, int transID2);

	/**
	 * Perform the FloydWahrshal algorithm
	 */
	void FloydWarshall();


private:
	int **_pathsMatrix;
	double **_distMatrix;
	std::vector< int > _tmpPedPath;
	std::map<int,int> _map_id_to_index;
	std::map<int,int> _map_index_to_id;
	///map the internal crossings/transition id to
	///the global ID (description) for that final destination
	std::map<int, int> _mapIdToFinalDestination;

protected:
	std::map <int, AccessPoint*> _accessPoints;
	Building *_building;

};

#endif /* GLOBALROUTER_H_ */