#!/usr/local/bin/python

from serial import Serial
import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D
import time
import math


# Object that has properties of (x,y,z)
class Point:
    def __init__(self, x, y=0.0, z=0.0):
        self.x = x
        self.y = y
        self.z = z


# Make a list of points
class PointCollection:
    def __init__(self, points_to_keep):
        self.points_to_keep = points_to_keep
        self.points = []

    # Add points to the list
    def add_point(self, point):
        if len(self.points) >= self.points_to_keep:
            self.points.pop(0)  # Remove the oldest point
        self.points.append(point)

    # Go through all the points and return all x values
    def get_x_values(self):
        return [p.x for p in self.points]

    # Go through all the points and return all y values
    def get_y_values(self):
        return [p.y for p in self.points]

    # Go through all the points and return all z values
    def get_z_values(self):
        return [p.z for p in self.points]

NUM_POINTS_TO_KEEP = 900 # Number of points to keep on the scatter plot
MAX_DIST = 30  # Maximum distance (in), to remove outliers

#  Initialize stuff
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


# Convert spherical to cartesian coordinates
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
while True:
    (dist, theta, phi) = read_serial()
    if dist:
        (x, y, z) = spherical_to_cartesian(dist, theta, phi)
        if y < MAX_DIST:
            points.add_point(Point(x, y, z))
            print('Radius: {0:0.3f}\tTheta: {1:0.3f}\tPhi: {2:0.3f}\tx:{3:0.3f}\ty:{4:0.3f}\tz:{5:0.3f}'.format(dist, theta, phi, x, y, z))
            # Plot our scan
            ax.cla()  # Clear figure
            ax.scatter(points.get_x_values(), points.get_y_values(), points.get_z_values())
            pyplot.draw()  # Redraw the figure
            pyplot.pause(0.001)  # Wait for it to render
        else:
            print('Out of range: Radius: {0:0.3f}\tTheta: {1:0.3f}\tPhi: {2:0.3f}\tx:{3:0.3f}\ty:{4:0.3f}\tz:{5:0.3f}'.format(dist, theta, phi, x, y, z))
