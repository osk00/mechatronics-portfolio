from coppeliasim_zmqremoteapi_client import RemoteAPIClient
from PySide6.QtWidgets import QApplication, QPushButton, QWidget, QVBoxLayout, QLabel
import math
import sys

# --- Ackermann Steering Helper ---
def ackermann_steering_angles(steering_angle, wheel_base, track_width):
    d = track_width / 2.0
    if steering_angle == 0:
        return 0.0, 0.0
    radius = wheel_base / math.tan(steering_angle)
    left_angle = math.atan(wheel_base / (radius - d))
    right_angle = math.atan(wheel_base / (radius + d))
    return left_angle, right_angle

# --- Main GUI App ---
class CarControlGUI(QWidget):
    def __init__(self, sim):
        super().__init__()
        self.sim = sim
        self.initUI()
        self.speed = 0.0
        self.steering_angle = 0.0

        # Car Parameters
        self.max_steering = math.radians(30)
        self.wheel_base = 2.5
        self.track_width = 1.5

        # Get joint handles
        self.steerL = sim.getObject('/steeringLeft')
        self.steerR = sim.getObject('/steeringRight')
        self.wheelL = sim.getObject('/motorLeft')
        self.wheelR = sim.getObject('/motorRight')


        self.update_motors()

    def initUI(self):
        layout = QVBoxLayout()

        self.status = QLabel("Speed: 0.0 | Steering: 0.0")
        layout.addWidget(self.status)

        up = QPushButton("↑ Speed +")
        down = QPushButton("↓ Speed -")
        left = QPushButton("← Steer Left")
        right = QPushButton("→ Steer Right")
        stop = QPushButton("! Stop")

        up.clicked.connect(self.increase_speed)
        down.clicked.connect(self.decrease_speed)
        left.clicked.connect(self.increase_steering)
        right.clicked.connect(self.decrease_steering)
        stop.clicked.connect(self.stop_car)

        layout.addWidget(up)
        layout.addWidget(down)
        layout.addWidget(left)
        layout.addWidget(right)
        layout.addWidget(stop)

        self.setLayout(layout)
        self.setWindowTitle("Ackermann Car Control")

    def update_motors(self):
        # Clamp steering angle
        self.steering_angle = max(-self.max_steering, min(self.steering_angle, self.max_steering))
        left_angle, right_angle = ackermann_steering_angles(self.steering_angle, self.wheel_base, self.track_width)

        # Update joints
        self.sim.setJointTargetPosition(self.steerL, left_angle)
        self.sim.setJointTargetPosition(self.steerR, right_angle)
        self.sim.setJointTargetVelocity(self.wheelL, self.speed)
        self.sim.setJointTargetVelocity(self.wheelR, self.speed)

        self.status.setText(f"Speed: {self.speed:.2f} | Steering: {math.degrees(self.steering_angle):.1f}°")

    def increase_speed(self):
        self.speed += 0.2
        self.update_motors()

    def decrease_speed(self):
        self.speed -= 0.2
        self.update_motors()

    def increase_steering(self):
        self.steering_angle += math.radians(5)
        self.update_motors()

    def decrease_steering(self):
        self.steering_angle -= math.radians(5)
        self.update_motors()

    def stop_car(self):
        self.speed = 0.0
        self.sim.setJointTargetVelocity(self.wheelL, 0.0)
        self.sim.setJointTargetVelocity(self.wheelR, 0.0)

        self.status.setText("! Stopped")

# --- Main ---
if __name__ == '__main__':
    client = RemoteAPIClient()
    sim = client.getObject('sim')
    sim.startSimulation()

    app = QApplication(sys.argv)
    gui = CarControlGUI(sim)
    gui.show()
    sys.exit(app.exec())

    sim.stopSimulation()