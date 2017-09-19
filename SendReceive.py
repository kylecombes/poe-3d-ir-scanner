#!/usr/local/bin/python

from serial import Serial, SerialException
import time

# The Serial constructor will take a different first argument on 
# Windows. The first argument on Windows will likely be of the form
# 'COMX' where 'X' is a number like 3,4,5 etc.
# Eg.cxn = Serial('COM5', baudrate=9600
#
# NOTE: You won't be able to program your Arduino or run the Serial 
# Monitor while the Python script is running. 
cxn = Serial('/dev/ttyACM0', baudrate=9600)


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
            if len(res) == 2:
                return res[0], res[1]
        except UnicodeDecodeError:
            pass
    return False, False


while True:
    (radius, theta) = read_serial()
    if radius:
        print('Radius: {}\tAngle: {}'.format(radius, theta))
    time.sleep(0.3)
