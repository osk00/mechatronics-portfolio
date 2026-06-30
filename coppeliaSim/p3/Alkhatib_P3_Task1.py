# This is a sample Python script for the exercise 2 and 3.

import numpy as np
import math
import transformations as t
import time
from random import  uniform
from pynput import keyboard
from coppeliasim_zmqremoteapi_client import *

keypress=''  # global variable

def inverse_kinematics_2dof(x, y, elbow_up=True):
    # Link lengths
    l1 = 0.25
    l2 = 0.25

    # Distance to target
    r = np.hypot(x, y)

    # Check reachability
    if r > (l1 + l2):
        print("Target out of reach")
        return None, None

    # Compute angle q2
    cos_q2 = (x**2 + y**2 - l1**2 - l2**2) / (2 * l1 * l2)
    cos_q2 = np.clip(cos_q2, -1.0, 1.0)  # numerical stability
    q2 = np.arccos(cos_q2)
    if not elbow_up:
        q2 = -q2

    # Compute angle q1
    k1 = l1 + l2 * np.cos(q2)
    k2 = l2 * np.sin(q2)
    q1 = np.arctan2(y, x) - np.arctan2(k2, k1)

    return q1, q2


############################################################################
def on_press(key):
    global keypress
    try:
        keypress = key.char
    except AttributeError:
        keypress = key
############################################################################
def on_release(key):
    global keypress
    keypress = ''

################################################################################
if __name__ == '__main__':
    print('Program started')
    # connect to Remote API
    client = RemoteAPIClient()
    sim = client.getObject('sim')

    # degrees of freedom of 2-DOF robot
    dof = 2
    # kinematic structure of 2-DOF robot  - to be filled out
    dx = np.array([0.0, 0.0, 0.0])
    DOF2_robot = np.array([
        [0, 0, 0, 0, 0, 0],     #transformation from world coordinate system (WCS) to robot base
        [0.0, 0, 0, 0, 0, 0],   # transformation from robot base to link2
        [0.0, 0, 0, 0, 0, 0]])  # transformation from link2 to TCP

    defaultIdleFps = sim.getInt32Param(sim.intparam_idle_fps)
    sim.setInt32Param(sim.intparam_idle_fps, 0)
    client.setStepping(True)
    sim.startSimulation()

    # Getting Handles of the TCP
    handle_tcp = sim.getObjectHandle('TCP')

    jh = np.empty(dof, dtype=object)  # create an empty array for joint handles
    # getting the handles for the joints
    jh[0]=sim.getObjectHandle('Revolute_joint1')
    jh[1]=sim.getObjectHandle('Revolute_joint2')
    elbow_up = 0
    l1 = 0.25
    l2 = 0.25

    joints = np.empty(dof, dtype=object)
    joints[0] = 20.0 * math.pi/180.0
    joints[1] = 20.0 * math.pi/180.0

    # compute initial TCP position from joint angles
    x = l1 * np.cos(joints[0]) + l2 * np.cos(joints[0] + joints[1])
    y = l1 * np.sin(joints[0]) + l2 * np.sin(joints[0] + joints[1])
    for i in range(dof):
            sim.setJointPosition(jh[i], joints[i])


    listener = keyboard.Listener(on_press=on_press, on_release=on_release)
    listener.start()

    while True:
        while keypress == '':  # wait for keyboard input
            pass
        if keypress == 'q':
            print("You pressed q to quit")
            break

        delta_pos = 0.02  # in m for each simulation step
        if keypress == "u":   # move up
            y += delta_pos
        elif keypress == "n":   # move down
            y -= delta_pos
        elif keypress == "h":    # move left
            x -= delta_pos
        elif keypress == "j":    # move right
            x += delta_pos
        elif keypress == "s":    # stop movement (do nothing)
            pass
        elif keypress == "e":    # toggle elbow mode
            elbow_up = elbow_up ^ 1
            print(f"Toggled elbow mode. Elbow is now {'up' if elbow_up else 'down'}")

        # Calculate new joint angles from current (x, y)
        q1, q2 = inverse_kinematics_2dof(x, y, elbow_up=elbow_up)
        if q1 is not None and q2 is not None:
            joints[0] = q1
            joints[1] = q2
            for i in range(dof):
                sim.setJointPosition(jh[i], joints[i])
        else:
            print(f"Target ({x:.3f}, {y:.3f}) is out of reach")

        print(f"TCP: x={x:.3f}, y={y:.3f} | q1={np.degrees(joints[0]):.1f}, q2={np.degrees(joints[1]):.1f}")
        client.step()
        if (sim.getSimulationState() == sim.simulation_stopped):
            break
        keypress = ''

    listener.stop()
    # Now stop the simulation:
    sim.stopSimulation()
    sim.setInt32Param(sim.intparam_idle_fps, defaultIdleFps)
