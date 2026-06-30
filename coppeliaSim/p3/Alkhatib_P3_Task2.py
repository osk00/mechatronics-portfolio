# This is a sample Python script for the exercise 2 and 3.

import numpy as np
import math
import transformations as t
import time
from random import  uniform
from pynput import keyboard
from coppeliasim_zmqremoteapi_client import *

keypress=''  # global variable

def forward_kinematics_3dof(q):
    l1 = l2 = l3 = 0.25

    q1, q2, q3 = q
    theta = q1 + q2 + q3

    x = l1 * np.cos(q1) + l2 * np.cos(q1 + q2) + l3 * np.cos(theta)
    y = l1 * np.sin(q1) + l2 * np.sin(q1 + q2) + l3 * np.sin(theta)

    return np.array([x, y, theta])

def jacobian_3dof(q):
    l1 = l2 = l3 = 0.25

    q1, q2, q3 = q
    theta1 = q1
    theta2 = q1 + q2
    theta3 = q1 + q2 + q3

    dx_dq1 = -l1 * np.sin(theta1) - l2 * np.sin(theta2) - l3 * np.sin(theta3)
    dx_dq2 = -l2 * np.sin(theta2) - l3 * np.sin(theta3)
    dx_dq3 = -l3 * np.sin(theta3)

    dy_dq1 = l1 * np.cos(theta1) + l2 * np.cos(theta2) + l3 * np.cos(theta3)
    dy_dq2 = l2 * np.cos(theta2) + l3 * np.cos(theta3)
    dy_dq3 = l3 * np.cos(theta3)

    J = np.array([
        [dx_dq1, dx_dq2, dx_dq3],
        [dy_dq1, dy_dq2, dy_dq3],
        [1.0,     1.0,     1.0]  
    ])
    return J

def inverse_kinematics_3dof(target_x, target_y, target_theta, q_init, max_iters=100, tol=1e-3):
    q = np.array(q_init, dtype=float)
    target = np.array([target_x, target_y, target_theta])

    for i in range(max_iters):
        current = forward_kinematics_3dof(q)
        error = target - current

        # Wrap angle error to [-pi, pi]
        error[2] = (error[2] + np.pi) % (2 * np.pi) - np.pi

        if np.linalg.norm(error) < tol:
            print(i)
            return q  # success
        

        J = jacobian_3dof(q)
        dq = np.linalg.pinv(J).dot(error)
        q += dq

    print("Warning: IK did not converge")
    return None


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

    # degrees of freedom of 3-DOF robot
    dof = 3
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
    jh[2] = sim.getObjectHandle('Revolute_joint3')
    elbow_up = 0
    l1 = 0.4
    l2 = 0.3

    joints = np.array([np.radians(20), np.radians(20), np.radians(20)])


    # compute initial TCP position from joint angles
    xyt = forward_kinematics_3dof(joints)

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
        if keypress == "u":
            xyt[1] += delta_pos  # y
        elif keypress == "n":
            xyt[1] -= delta_pos
        elif keypress == "h":
            xyt[0] -= delta_pos  # x
        elif keypress == "j":
            xyt[0] += delta_pos
        elif keypress == "l":
            xyt[2] += np.radians(5)  # theta
        elif keypress == "r":
            xyt[2] -= np.radians(5)

        q_sol = inverse_kinematics_3dof(xyt[0], xyt[1], xyt[2], joints)
        if q_sol is not None:
            joints = q_sol
            for i in range(dof):
                sim.setJointPosition(jh[i], joints[i])
        else:
            print(f"Target ({xyt[0]:.3f}, {xyt[1]:.3f}, {xyt[2]:.2f}) is out of reach")

        print(f"TCP: x={xyt[0]:.3f}, y={xyt[1]:.3f}, θ={np.degrees(xyt[2]):.1f}° | q1={np.degrees(joints[0]):.1f}, q2={np.degrees(joints[1]):.1f}, q3={np.degrees(joints[2]):.1f}")
        client.step()
        if (sim.getSimulationState() == sim.simulation_stopped):
            break
        keypress = ''

    listener.stop()
    # Now stop the simulation:
    sim.stopSimulation()
    sim.setInt32Param(sim.intparam_idle_fps, defaultIdleFps)
