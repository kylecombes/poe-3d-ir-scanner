#!/usr/local/bin/python

from serial import Serial
import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D
import time
import math


class Point:
    def __init__(self, x, y=0.0, z=0.0):
        self.x = x
        self.y = y
        self.z = z


class PointCollection:
    def __init__(self, points_to_keep):
        self.points_to_keep = points_to_keep
        self.points = []

    def add_point(self, point):
        if len(self.points) >= self.points_to_keep:
            self.points.pop(0)  # Remove the oldest point
        # print('Adding point ({0:0.3f},{1:0.3f})'.format(point.x, point.y))
        self.points.append(point)

    def get_x_values(self):
        return [p.x for p in self.points]

    def get_y_values(self):
        return [p.y for p in self.points]

    def get_z_values(self):
        return [p.z for p in self.points]

NUM_POINTS_TO_KEEP = 1600
MAX_DIST = 100  # Maximum distance (in), to remove outliers

# Initialize stuff
cxn = Serial('/dev/ttyACM0', baudrate=9600)
points = PointCollection(NUM_POINTS_TO_KEEP)


def read_serial():
    """
        Attempt to read the radius (distance) and angle from the Arduino.
        Returns a tuple in the format (radius, theta) if successful, or False otherwise.
    """
    while cxn.inWaiting() < 1:
        pass
    data = cxn.readline()
    if data:
        try:
            data = data.decode('UTF-8')
            res = data.split('\t')
            if len(res) == 3:
                return float(res[0]), float(res[1]), float(res[2])
        except UnicodeDecodeError:
            pass
        except ValueError:
            pass  # Error casting to float
    return False, False, False


def spherical_to_cartesian(radius, theta, phi):
    theta = math.radians(theta)
    phi = math.radians(phi)
    x = radius*math.sin(phi)*math.cos(theta)
    y = radius*math.sin(phi)*math.sin(theta)
    z = radius*math.cos(phi)
    return x, y, z


fig = pyplot.figure()
# pyplot.ion()
fig.show()  # Show the figure
ax = fig.add_subplot(111, projection='3d')  # Set up 3D plot
# ax.xlabel('x (in)')  # Label x-axis
# ax.ylabel('y (in)')  # Label y-axis
# ax.ylabel('z (in)')  # Label z-axis
while True:
    (dist, theta, phi) = read_serial()
    if dist and dist < MAX_DIST:
        (x, y, z) = spherical_to_cartesian(dist, theta, phi)
        points.add_point(Point(x, y, z))
        print('Radius: {0:0.3f}\tAngle: {1}\tx:{2:0.3f}\ty:{3:0.3f}\tz:{3:0.3f}'.format(dist, theta, phi, x, y, z))
        # Plot our scan
        ax.cla()  # Clear figure
        ax.scatter(points.get_x_values(), points.get_y_values(), points.get_z_values())
        pyplot.draw()  # Redraw the figure
        pyplot.pause(0.001)  # Wait for it to render
    time.sleep(0.3)  # Might be unnecessary
