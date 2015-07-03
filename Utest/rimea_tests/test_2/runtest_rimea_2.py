#!/usr/bin/env python
import os
import sys
utestdir = os.path.abspath(os.path.dirname(os.path.dirname(sys.path[0])))
from sys import *
sys.path.append(utestdir)
from JPSRunTest import JPSRunTestDriver
from utils import *



def run_rimea_test2(inifile, trajfile):
    must_min_time = 19
    must_max_time = 21
    # pedestrians in force-based models accelerate from 0 to v0 after some time (\tau)
    fps, n, traj = parse_file(trajfile)
    # filter trajecetries. Consider only the stair
    traj_stair = traj[0 <= traj[:, 2]]
    traj_stair = traj_stair[traj_stair[:, 2] <= 10]

    evac_time = (max(traj_stair[:, 1]) - min(traj_stair[:, 1])) / float(fps)
    if must_min_time <= evac_time <= must_max_time:
        logging.info("evac_time: %f <= %f <= %f", must_min_time, evac_time, must_max_time)
    else:
        logging.critical("%s exits with FAILURE. evac_time: %f <= %f <= %f ?",
                         argv[0], must_min_time, evac_time, must_max_time)
        exit(FAILURE)



if __name__ == "__main__":
    test = JPSRunTestDriver(2, argv0=argv[0], testdir=sys.path[0], utestdir=utestdir)
    test.run_test(testfunction=run_rimea_test2)
    logging.info("%s exits with SUCCESS" % (argv[0]))
    exit(SUCCESS)




