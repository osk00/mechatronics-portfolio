# This is a sample Python script for exercise 2 and 3.
import numpy as np
import math
import time
import transformations as t
from random import  uniform
from pynput import keyboard
from coppeliasim_zmqremoteapi_client import *


keypress=''  # global variable


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


############################################################################
# this needs to be implemented
def convert_quaternion_to_matrix(quat):
    w = quat[3]
    x = quat[0]
    y = quat[1]
    z = quat[2]

    # First row of the rotation matrix
    r00 = 1 - 2*(y*y + z*z)
    r01 = 2*(x*y - z*w)
    r02 = 2*(x*z + y*w)
    # Second row of the rotation matrix
    r10 = 2*(x*y + z*w)
    r11 = 1 - 2*(x*x + z*z)
    r12 = 2*(y*z - x*w)
    # Third row of the rotation matrix
    r20 = 2*(x*z - y*w)
    r21 = 2*(y*z + x*w)
    r22 = 1 - 2*(x*x + y*y)
    # 3x3 rotation matrix
    rot_matrix = np.array([[r00, r01, r02],
                           [r10, r11, r12],
                           [r20, r21, r22]])
    return rot_matrix
############################################################################
# this needs to be implemented
def euler_from_matrix(R):
    # CoppeliaSim uses intrinsic XYZ Euler angles
    if abs(R[2][0]) < 1.0:
        y = -math.asin(R[2][0])
        cos_y = math.cos(y)
        x = math.atan2(R[2][1] / cos_y, R[2][2] / cos_y)
        z = math.atan2(R[1][0] / cos_y, R[0][0] / cos_y)
    else:
        # Gimbal lock case
        y = -math.pi / 2 if R[2][0] > 0 else math.pi / 2
        x = math.atan2(-R[0][1], -R[0][2])
        z = 0
    return [x, y, z]
############################################################################
# this needs to be implemented
def matrix_from_euler(angles):
    alpha = angles[0]  # rotation around X
    beta = angles[1]   # rotation around Y
    gamma = angles[2]  # rotation around Z

    # First row of the rotation matrix
    r00 = math.cos(beta) * math.cos(gamma)
    r01 = -math.cos(beta) * math.sin(gamma)
    r02 = math.sin(beta)

    # Second row of the rotation matrix
    r10 = math.sin(alpha) * math.sin(beta) * math.cos(gamma) + math.cos(alpha) * math.sin(gamma)
    r11 = -math.sin(alpha) * math.sin(beta) * math.sin(gamma) + math.cos(alpha) * math.cos(gamma)
    r12 = -math.sin(alpha) * math.cos(beta)
    # Third row of the rotation matrix
    r20 = -math.cos(alpha) * math.sin(beta) * math.cos(gamma) + math.sin(alpha) * math.sin(gamma)
    r21 = math.cos(alpha) * math.sin(beta) * math.sin(gamma) + math.sin(alpha) * math.cos(gamma)
    r22 = math.cos(alpha) * math.cos(beta)
    # 3x3 rotation matrix
    rot_matrix = np.array([[r00, r01, r02],
                           [r10, r11, r12],
                           [r20, r21, r22]])
    return rot_matrix
################################################################################
def forward_kinematics(rob, joints):
    TCP = t.identity_matrix()

    for i in range(len(rob)):
        # 1. Apply joint rotation BEFORE this link
        if i > 0:
            joint_angle = joints[i - 1]
            Rz = t.rotation_matrix(joint_angle, [0, 0, 1])
            TCP = t.concatenate_matrices(TCP, Rz)

        # 2. Apply link’s fixed transform (translation + rotation)
        T = t.translation_matrix(rob[i, 0:3])
        R = t.euler_matrix(rob[i, 3], rob[i, 4], rob[i, 5], 'sxyz')

        TCP = t.concatenate_matrices(TCP, T, R)
    return TCP
################################################################################
if __name__ == '__main__':

    print('Program started')

    # degrees of freedom of snake robot
    dof = 6
    # kinematic structure of snake robot  - !DONE
    snake_robot = np.array([
    [0,    0, 0, 0, 0, 0],           # base frame
    [250, 0, 0, 0, 0, -math.pi/2],  # after joint 1
    [250, 0, 0, 0, 0,  math.pi/2],  # after joint 2
    [250, 0, 0, 0, 0, -math.pi/2],  # after joint 3
    [250, 0, 0, 0, 0,  math.pi/2],  # after joint 4
    [250, 0, 0, 0, 0, -math.pi/2],  # after joint 5
    [250, 0, 0, 0, 0, 0]])          # after joint 6 (to TCP)


    # connect to Remote API
    client = RemoteAPIClient()
    sim = client.getObject('sim')

    # Getting Handles of the Tool Centre Point (TCP)
    handle_tcp = sim.getObjectHandle('./TCP')

    jh = np.empty(dof, dtype=object)  # create an empty array for joint handles
    # getting the handles for the joints
    jh[0]=sim.getObjectHandle('Revolute_joint1')
    jh[1]=sim.getObjectHandle('Revolute_joint2')
    jh[2]=sim.getObjectHandle('Revolute_joint3')
    jh[3]=sim.getObjectHandle('Revolute_joint4')
    jh[4]=sim.getObjectHandle('Revolute_joint5')
    jh[5]=sim.getObjectHandle('Revolute_joint6')
    time.sleep(1)
    joints = np.empty(dof, dtype=object)  # create an empty array for the joints
    # set all joints to 0
    for i in range(dof):
        sim.setJointPosition(jh[i], 0.0)

    listener = keyboard.Listener(on_press=on_press,on_release=on_release)
    listener.start()

    while True:
        while keypress=='':  # wait for keyboard input
            pass
        if keypress == 'q':
            print("You pressed q to quit")
            break
        # set joints to random position between [0, pi/4]
        for i in range(dof):
            sim.setJointPosition(jh[i], uniform(0.0, math.pi/4))

        angles = sim.getObjectOrientation(handle_tcp, -1)  # Try to retrieve orientation
        angles_deg = [math.degrees(a) for a in angles]

        pos    = sim.getObjectPosition(handle_tcp, -1)  # Try to retrieve position
        # retrieve the joint positions
        for i in range(dof):
            joints[i] = sim.getJointPosition(jh[i])
        #print('angles: ', angles)
        print("Sim Euler angles (rad):", [round(a, 4) for a in angles])
        print("Sim Euler angles (deg):", [round(a, 2) for a in angles_deg])
        R1 = matrix_from_euler(angles)
        print('TCP Rotation: ')
        print(R1)
        print('TCP Position: ', pos)
        TCP_frame = forward_kinematics(snake_robot, joints)
        print(TCP_frame)
        rot = TCP_frame[:3,:3]
        euler_rad = euler_from_matrix(rot)
        euler_deg = [math.degrees(a) for a in euler_rad]
        print("TCP Euler angles (deg):", euler_deg)
        #print(euler_from_matrix(rot))
        print("----------------------------------------------------   ")
        #time.sleep(0.005)

        keypress = ''

    listener.stop()
    print('Program ended')
