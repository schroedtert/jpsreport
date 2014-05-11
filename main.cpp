
/**
 * @file    main.cpp
 * @author  U.Kemloh, A.Portz
 * @version 0.5
 * Copyright (C) <2009-2012>
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
 * along with JuPedSim. If not, see <a href="http://www.gnu.org/licenses/">GNU LICENCE</a>.
 *
 * @section DESCRIPTION
 *
 *
 */
/**
 * @mainpage
 *
 * \date 15.1.2013
 *
 * JuPedSim stands for Jülich Pedestrians Simulator and is currently developed at the Forschungszentrum Jülich in Germany.
 *
 *
 *
 * Some useful links:
 *
 *      1: <A HREF="http://www.openpedsim.org">www.openpedsim.org</A> <BR>
 *      2: <a href="http://www.vtk.org">www.vtk.org</a> <br>
 *      3: <a href="http://www.fz-juelich.de">www.fz-juelich.de</a> <br>
 *      4: <a href="http://www.jupedsim.org">www.fz-juelich.de</a> <br>
 *
 */

#include "geometry/Building.h"
#include "general/ArgumentParser.h"
#include "./Simulation.h"


/// global unique log variable
OutputHandler* Log;

int main(int argc, char **argv)
{
     time_t starttime, endtime;
     // Log = new FileHandler("./Logfile.dat");
     Log = new STDIOHandler();
     // Parsing the arguments

     ArgumentParser* args = new ArgumentParser();
     args->ParseArgs(argc, argv);

     // create and init the simulation engine
     // Simulation
     time(&starttime);
     Log->Write("INFO: \tStart runSimulation()\n");

     Simulation sim = Simulation();
     sim.InitArgs(args);
     int evacTime = sim.RunSimulation();
     Log->Write("\nINFO: \tEnd runSimulation()\n");
     time(&endtime);

     // some output
     double execTime = difftime(endtime, starttime);

     if (sim.GetPedsNumber())
          Log->Write("\nPedestrians not evacuated [%d] using [%d] threads",
                     sim.GetPedsNumber(),
                     args->GetMaxOpenMPThreads());

     Log->Write("\nExec Time [s]   : %.2f", execTime);
     Log->Write("Evac Time [s]     : %d", evacTime);
     Log->Write("Real Time Factor  : %.2f X", evacTime / execTime);
     Log->Write("Warnings          : %d\n", Log->GetWarnings() );
     Log->Write("Critical          : %d\n", Log->GetCriticals() );
     Log->Write("Errors            : %d\n", Log->GetErrors() );
     // sim.PrintStatistics();
     if (NULL == dynamic_cast<STDIOHandler*>(Log)) {
          printf("\nExec Time [s]       : %4.2f\n", execTime);
          printf("Evac Time [s]       : %d\n", evacTime);
          printf("Real Time Factor    : %.2f (X)\n", evacTime / execTime);
          printf("Warnings            : %d\n", Log->GetWarnings() );
          printf("Critical            : %d\n", Log->GetCriticals() );
          printf("Errors              : %d\n", Log->GetErrors() );
     }

     // do the last cleaning
     delete args;
     delete Log;

     return (EXIT_SUCCESS);
}
