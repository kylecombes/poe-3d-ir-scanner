#!/usr/local/bin/python

import re
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

    # try:
    #     cmd_id = int(input("Please enter a command ID (1 - read potentiometer, 2 - read the button: "))
    #     if int(cmd_id) > 2 or int(cmd_id) < 1:
    #         print("Values other than 1 or 2 are ignored.")
    #     else:
    #         cxn.write([int(cmd_id)])
    #         while cxn.inWaiting() < 1:
    #             pass
    #         result = cxn.readline();
    #         print(result)
    # except ValueError:
    #     print("You must enter an integer value between 1 and 2.")
