from coppeliasim_zmqremoteapi_client import *

def visualizePath(path):
    global lineContainer
    global initPos
    sim.addDrawingObjectItem(lineContainer, None)
    if path is not None:
        for i in range(len(path) // 3):
            lineDat = [path[3 * i + 0], path[3 * i + 1], initPos[2]]
            sim.addDrawingObjectItem(lineContainer, lineDat)

if __name__ == '__main__':
    print('Program started')
    client = RemoteAPIClient()
    sim = client.require('sim')
    simOMPL = client.require('simOMPL')

    client.setStepping(True)
    sim.startSimulation()
    print("Simulation started")

    global lineContainer
    global initPos
    lineContainer = sim.addDrawingObject(sim.drawing_linestrip, 3, 0, -1, 0, [0, 0, 1])

    robotHandle = sim.getObject('./StartConfiguration')
    targetHandle = sim.getObject('/GoalConfiguration')
    initPos = sim.getObjectPosition(robotHandle, sim.handle_world)
    initOrient = sim.getObjectOrientation(robotHandle, sim.handle_world)

    t = simOMPL.createTask('t')
    ss = [simOMPL.createStateSpace('2d', simOMPL.StateSpaceType.pose2d, robotHandle, [-0.5, -0.5], [0.5, 0.5], 1)]
    simOMPL.setStateSpace(t, ss)
    simOMPL.setAlgorithm(t, simOMPL.Algorithm.RRTConnect)

    # Set correct collision pairs: check collision between robot and all obstacles
    robotShape = sim.getObject('./start')
    simOMPL.setCollisionPairs(t, [robotShape, sim.handle_all])

    # Start pose (x, y, yaw)
    startPos = sim.getObjectPosition(robotHandle, sim.handle_world)
    startOrient = sim.getObjectOrientation(robotHandle, sim.handle_world)
    simOMPL.setStartState(t, [startPos[0], startPos[1], startOrient[2]])

    # Goal pose
    goalPos = sim.getObjectPosition(targetHandle, sim.handle_world)
    goalOrient = sim.getObjectOrientation(targetHandle, sim.handle_world)
    simOMPL.setGoalState(t, [goalPos[0], goalPos[1], goalOrient[2]])

    simOMPL.setup(t)
    r, path = simOMPL.compute(t, 4, -1, 800)

    if path is not None:
        print("Path found!")
        visualizePath(path)

        # Execute path (position + orientation)
        for i in range(len(path) // 3):
            pos = [path[3 * i + 0], path[3 * i + 1], initPos[2]]
            orient = [0, 0, path[3 * i + 2]]
            sim.setObjectPosition(robotHandle, sim.handle_world, pos)
            sim.setObjectOrientation(robotHandle, sim.handle_world, orient)
            client.step()
    else:
        print("No path found!")

    sim.stopSimulation()
    print("Simulation stopped")
