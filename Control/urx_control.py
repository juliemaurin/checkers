#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import urx
import socket
from time import sleep
# URscript reference : http://www.sysaxes.com/manuels/scriptmanual_en_3.1.pdf

class Robot:
    def __init__(self):
        self.a = 2 # Tool acceleration (m/s²)
        self.v = 0.1 #(max=3) # Tool speed (m/s)
        self.payload_weight = 1.2

        self.lift_distance = 0.03

        self.angles = [2.22, 2.22, -0.01]

        # Connect to the robot
        self.rob = urx.Robot("192.168.1.196")
        self.rob.set_tcp((0, 0, 0.17, 0, 0, 0))
        self.rob.set_payload(self.payload_weight, (0, 0, 0))

        #TODO : Edit this
        #self.reset_pos()

        print "MAKE SURE TO MOVE THE ROBOT TO THE ORIGIN 1 BEFORE START"
        _ = raw_input("(Press enter to continue)")

        start1 = self.rob.getl()

        print "MAKE SURE TO MOVE THE ROBOT TO THE ORIGIN 2 BEFORE START"
        _ = raw_input("(Press enter to continue)")

        start2 = self.rob.getl()

        self.delta = [(b - a) / 7.0 for a, b in zip(start1, start2)]

        # Cancel angle linearization
        self.delta[3] = 0.0
        self.delta[4] = 0.0
        self.delta[5] = 0.0

        self.start = start1

        print start1
        print start2

        print self.delta
        print self.start

    def get_poses(self, offset):
        low = [start + offset for start, offset in zip(self.start, offset)]
        high = list(low)
        high[2] += self.lift_distance
        return low, high

    def get_pos(self, n):
        i = 2 * (n % 4)
        j = n // 4

        if (j % 2 == 0):
            i += 1

        dx = i * self.delta[0]
        dy = j * self.delta[1]

        height_low = self.delta[2] * (i + j) / 2.0

        rx = self.delta[3] * (i + j) / 2.0
        rz = self.delta[4] * (i + j) / 2.0
        ry = self.delta[5] * (i + j) / 2.0

        return dx, dy, height_low, rx, ry, rz

    def move_piece(self, frm, to, wsg):
        # Get "from" positions
        pos_from_offset = self.get_pos(frm)
        pos_from_low, pos_from_high = self.get_poses(pos_from_offset)

        # Get "to" positions
        pos_to_offset = self.get_pos(to)
        pos_to_low, pos_to_high = self.get_poses(pos_to_offset)

        print "Picking ", frm
        self.rob.movel(pos_from_high, self.a, self.v)
        self.rob.movel(pos_from_low, self.a, self.v)
        wsg.grasp()
        sleep(0.3)
        self.rob.movel(pos_from_high, self.a, self.v)

        print "Placing ", to
        self.rob.movel(pos_to_high, self.a, self.v)
        self.rob.movel(pos_to_low, self.a, self.v)
        wsg.release()
        sleep(0.3)
        self.rob.movel(pos_to_high, self.a, self.v)

    def remove_piece(self, n, wsg):
        # Get "from" positions
        pos_from_offset = self.get_pos(n)
        pos_from_low, pos_from_high = self.get_poses(pos_from_offset)
        print pos_from_low, pos_from_high

        # Get "to" positions
        pos_to_offset = (-2 * self.delta[0], -1 * self.delta[1], self.delta[2], 0.0, 0.0, 0.0)
        pos_to_low, pos_to_high = self.get_poses(pos_to_offset)

        print "Removing ", n
        self.rob.movel(pos_from_high, self.a, self.v)
        self.rob.movel(pos_from_low, self.a, self.v)
        wsg.grasp()
        sleep(0.3)
        self.rob.movel(pos_from_high, self.a, self.v)

        self.rob.movel(pos_to_high, self.a, self.v)
        self.rob.movel(pos_to_low, self.a, self.v)
        wsg.release()
        sleep(0.3)
        self.rob.movel(pos_to_high, self.a, self.v)

    def bin_to_move(self, n):
        # Convert to binary string
        s = format(n, '032b')

        frm = None
        to = None
        for k, v in enumerate(reversed(s)):
            if (v == '1'):
                if (frm is None):
                    frm = k
                elif (to is None):
                    to = k

        return frm, to

if __name__ == "__main__":
    try:
        import sys
        # Connect to the Robot
        print "Connecting to robot"
        ur = Robot()

        import wsg50_control
        # Connect to the gripper
        print "Connecting to grippers"
        wsg = wsg50_control.WSG50()
        wsg.connect()

        for i in range(32):
            ur.move_piece(i, 5, wsg)
            ur.remove_piece(i, wsg)
    except KeyboardInterrupt:
        print "MANUALY INTERRUPTED"
    except:
        print "UNEXPECTED ERROR"
        print sys.exc_info()
    finally:
        ur.rob.stopl()
        ur.rob.close()
        wsg.disconnect()

"""
print "Current tool pose is: ",  rob.getl()
rob.translate((0.1, 0, 0), a, v)  #move tool and keep orientation
rob.stopj(a)

robot.movel(x, y, z, rx, ry, rz), wait=False)
while True :
    sleep(0.1)  #sleep first since the robot may not have processed the command yet
    if robot.is_program_running():
        break

robot.movel(x, y, z, rx, ry, rz), wait=False)
while.robot.getForce() < 50:
    sleep(0.01)
    if not robot.is_program_running():
        break
robot.stopl()

try:
    robot.movel((0,0,0.1,0,0,0), relative=True)
except RobotError, ex:
    print("Robot could not execute move (emergency stop for example), do something", ex)
"""
