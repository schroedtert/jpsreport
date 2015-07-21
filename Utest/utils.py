from xml.dom import minidom
import logging
import numpy as np

SUCCESS = 0
FAILURE = -1


def is_inside(trajectories, left, right, down, up):
    """
    up --> . _____.
           |      |
  down --> . -----.
           ^      ^
          left   right
    """
    condition = (trajectories[0, 2] > left) & \
                (trajectories[0, 2] < right) & \
                (trajectories[0, 3] > down) & \
                (trajectories[0, 3] < up)
    return condition.all()

def PassedLine(p, e):
    """
    check if pedestrian (given by matrix p) passed the line [x1, x2, y1, y2] with x1<x2 and  y1<y2
    """
    assert (isinstance(e, list) or isinstance(e, np.ndarray)) and len(e) == 4,\
        "exit should be a list with len=4"

    x1 = e[0]
    y1 = e[1]
    x2 = e[2]
    y2 = e[3]
    A = np.array([x1, y1])
    B = np.array([x2, y2])

    is_left_and_right = (any(np.cross(B-A, p[:, 2:]-A)) < 0) & (any(np.cross(B-A, p[:, 2:]-A)) > 0)
    is_between = any(np.dot(p[:, 2:]-A, B-A) > 0) & any(np.dot(p[:, 2:]-B, A-B) > 0)

    return  is_left_and_right and is_between


def PassedLineX(p, exit):
    """
    check if pedestrian (given by matrix p) passed the vertical line x, [y1, y2] y1<y2
    """
    x = exit[0]
    y1 = exit[1]
    y2 = exit[2]
    return any(p[:, 2] <= x) & any(p[:, 2] >= x) & any(p[:, 3] >= y1) & any(p[:, 3] <= y2)


def PassedLineY(p, exit):
    """
    check if pedestrian (given by matrix p) passed the horizontal line y, [x1, x2] x1<x2
    """
    y = exit[0]
    x1 = exit[1]
    x2 = exit[2]
    return any(p[:, 3] <= y) & any(p[:, 3] >= y) & any(p[:, 2] >= x1) & any(p[:, 2] <= x2)


def get_maxtime(filename):
    """
    get max sim time
    """
    logging.info("parsing <%s>"%filename)
    try:
        xmldoc = minidom.parse(filename)
    except:
        logging.critical('could not parse file %s. exit'%filename)
        exit(FAILURE)
    maxtime = float(xmldoc.getElementsByTagName('max_sim_time')[0].firstChild.nodeValue)
    return maxtime


def parse_file(filename):
    """
    parse trajectories in Travisto-format and output results
    in the following  format: id    frame    x    y
    (no sorting of the data is performed)
    returns:
    fps: frames per second
    N: number of pedestrians
    data: trajectories (numpy.array) [id fr x y]
    """
    logging.info("parsing <%s>"%filename)
    try:
        xmldoc = minidom.parse(filename)
    except:
        logging.critical('could not parse file. exit')
        exit(FAILURE)
    N = int(xmldoc.getElementsByTagName('agents')[0].childNodes[0].data)
    fps = xmldoc.getElementsByTagName('frameRate')[0].childNodes[0].data #type unicode
    fps = float(fps)
    fps = int(fps)
    #print "fps=", fps
    #fps = int(xmldoc.getElementsByTagName('frameRate')[0].childNodes[0].data)
    logging.info("Npeds = %d, fps = %d"%(N, fps))
    frames = xmldoc.childNodes[0].getElementsByTagName('frame')
    data = []
    for frame in frames:
        frame_number = int(frame.attributes["ID"].value)
        for agent in frame.getElementsByTagName("agent"):
            agent_id = int(agent.attributes["ID"].value)
            x = float(agent.attributes["x"].value)
            y = float(agent.attributes["y"].value)
            data += [agent_id, frame_number, x, y]
    data = np.array(data).reshape((-1, 4))
    return fps, N, data


def flow(fps, N, data, x0):
    """
    measure the flow at a vertical line given by <x0>
    trajectories are given by <data> in the following format: id    frame    x    y
    input:
    - fps: frame per second
    - N: number of peds
    - data: trajectories
    - x0: x-coordinate of the vertical measurement line
    output:
    - flow
    """
    logging.info('Measure flow at %f'%x0)
    if not isinstance(data, np.ndarray):
        logging.critical("flow() accepts data of type <ndarray>. exit")
        exit(FAILURE)
    peds = np.unique(data[:, 0]).astype(int)
    times = []
    for ped in peds:
        d = data[data[:, 0] == ped]
        passed = d[d[:, 2] >= x0]
        if passed.size == 0:  # pedestrian did not pass the line
            logging.critical("Pedestrian <%d> did not pass the line at <%.2f>"%(ped, x0))
            exit(FAILURE)
        first = min(passed[:, 1])
        #print "ped= ", ped, "first=",first
        times.append(first)
    if len(times) < 2:
        logging.warning("Number of pedestrians passing the line is small. return 0")
        return 0
    logging.info("min(times)=%f max(times)=%f"%(min(times), max(times)))
    return fps * float(N-1) / (max(times) - min(times))




