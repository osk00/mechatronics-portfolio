# Robotics Control with CoppeliaSim and Python

## Overview

This repository contains a comprehensive collection of robotics control projects implemented in Python using the CoppeliaSim simulation environment. The projects cover fundamental robotics concepts from basic robot control to advanced motion planning, all connected through the ZeroMQ Remote API.

### Key Topics Covered
- Robot control and kinematics (forward/inverse)
- Motion planning (potential fields, OMPL)
- Trajectory generation (Bezier splines)
- GUI development (PySide6)
- Holonomic and non-holonomic path planning
- Real-time simulation and control

---

## Project Structure


---

## Project Descriptions

### 1. BubbleRob Control

**Aim**: Control a two-wheeled robot in CoppeliaSim via a PySide6 GUI.

**Features**:
- Real-time robot control through intuitive GUI
- Start/Stop functionality
- Directional control (forward/backward/left/right)

**Key Technologies**: PySide6, CoppeliaSim ZMQ API

---

### 2. Snake Robot Kinematics

**Aim**: Implement forward kinematics for a 6-DOF snake robot with rotation representations.

**Features**:
- Euler angle ↔ Quaternion ↔ Rotation matrix conversions
- Transform chain construction
- Generic forward kinematics function
- Validation against CoppeliaSim simulation

**Key Technologies**: NumPy, Transformations Library

---

### 3. Inverse Kinematics

**Aim**: Solve inverse kinematics for planar robots using analytical and numerical methods.

**Sub-projects**:
- **2-DOF Robot**: Analytical IK solution with closed-form equations
- **3-DOF Robot**: Numerical IK using Jacobian matrix and pseudo-inverse

**Key Technologies**: NumPy, Jacobian computation, Keyboard control

---

### 4. Motion Planning with Potential Fields

**Aim**: Navigate a 2D robot to a target while avoiding obstacles using potential fields.

**Features**:
- Attractive force toward target
- Repulsive forces from obstacles (12 distance sensors)
- Real-time obstacle avoidance
- Smooth trajectory generation

**Key Technologies**: NumPy, Potential Field Method

---

### 5. Path Following

**Aim**: Implement trajectory following for different robot types.

**Sub-projects**:
- **Cuboid Path Following**: Object follows a Bezier spline with correct orientation
- **Ackermann Car**: GUI-controlled vehicle with realistic steering geometry

**Key Technologies**: Bezier splines, PySide6, Ackermann kinematics

---

### 6. OMPL Path Planning

**Aim**: Plan collision-free paths in labyrinth environments using OMPL.

**Features**:
- **Holonomic Planning**: Free translation and rotation (Pose2D state space)
- **Non-holonomic Planning**: Dubins kinematics with constrained turning radius
- Multiple planning algorithms (RRT, RRTConnect, RRT*)
- Path visualization and execution

**Key Technologies**: OMPL, RRT algorithms, Dubins paths

---

## Technologies & Dependencies

| Component | Technology | Version |
|-----------|------------|---------|
| **GUI** | PySide6 (Qt for Python) | 6.x |
| **Simulation** | CoppeliaSim Edu | V4.4+ |
| **API** | ZeroMQ Remote API | Latest |
| **Scientific Computing** | NumPy | 1.19+ |
| **Motion Planning** | OMPL | Latest |
| **Keyboard Control** | pynput / keyboard | Latest |
| **Transformations** | transformations library | Latest |
| **Data Serialization** | CBOR | Latest |

---

## Installation

### Prerequisites

1. **Install CoppeliaSim Edu** from the [official website](https://www.coppeliarobotics.com/downloads)

2. **Install Python dependencies**:
```bash
# Core dependencies
pip install numpy coppeliasim-zmqremoteapi-client

# For GUI applications
pip install PySide6

# For keyboard control
pip install pynput keyboard

# For transformations and serialization
pip install transformations cbor

# For OMPL path planning
pip install ompl
