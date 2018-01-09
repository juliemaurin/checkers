#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import urx
import socket
from time import sleep
# URscript reference : http://www.sysaxes.com/manuels/scriptmanual_en_3.1.pdf

class Robot:
    def __init__(self):
        self.a = 2 # Tool acceleration (m/s²)
        self.v = 3 # Tool speed (m/s)
        self.payload_weight = 1.2

        self.height_high = 0.06
        self.height_low = 0.025
        self.angles = [2.22, 2.22, -0.01]
        self.delta = [0.035, -0.035]

        # Connect to the robot
        self.rob = urx.Robot("192.168.1.196")
        self.rob.set_tcp((0, 0, 0.17, 0, 0, 0))
        self.rob.set_payload(self.payload_weight, (0, 0, 0))

        #TODO : Edit this
        self.reset_pos()

        print "MAKE SURE TO MOVE THE ROBOT TO THE ORIGIN BEFORE START"
        _ = raw_input("(Press enter to continue)")

        self.start = self.rob.getl()[:2]

        print self.start

    def get_pos(self, n):
        i = n % 4
        j = n // 4
        dx = 2 * i * self.delta[0]
        if (j % 2): dx += self.delta[0]
        dy = j * self.delta[1]
        return dx, dy

    def move_piece(self, frm, to, wsg):
        # Get "from" positions
        pos_from_offset = self.get_pos(frm)
        print "From offset = ", pos_from_offset
        pos_from_high = (self.start[0] + pos_from_offset[0], self.start[1] + pos_from_offset[1], self.height_high, self.angles[0], self.angles[1], self.angles[2])
        pos_from_low = (self.start[0] + pos_from_offset[0], self.start[1] + pos_from_offset[1], self.height_low, self.angles[0], self.angles[1], self.angles[2])

        # Get "to" positions
        pos_to_offset = self.get_pos(to)
        print "To offset = ", pos_to_offset
        pos_to_high = (self.start[0] + pos_to_offset[0], self.start[1] + pos_to_offset[1], self.height_high, self.angles[0], self.angles[1], self.angles[2])
        pos_to_low = (self.start[0] + pos_to_offset[0], self.start[1] + pos_to_offset[1], self.height_low, self.angles[0], self.angles[1], self.angles[2])

        self.rob.movel(pos_from_high, self.a, self.v)
        self.rob.movel(pos_from_low, self.a, self.v)
        wsg.grasp()
        sleep(0.2)
        self.rob.movel(pos_from_high, self.a, self.v)

        self.rob.movel(pos_to_high, self.a, self.v)
        self.rob.movel(pos_to_low, self.a, self.v)
        wsg.release()
        sleep(0.2)
        self.rob.movel(pos_to_high, self.a, self.v)

        sleep(0.1)

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

    def reset_pos(self):
        init = (0.63254, -0.20653, self.height_low, self.angles[0], self.angles[1], self.angles[2])
        self.rob.movel(init, self.a, self.v)

if __name__ == "__main__":
    # Connect to the Robot
    print "Connecting to robot..."
    ur = Robot()
    p1 = (0.63254, -0.20653, ur.height_low, ur.angles[0], ur.angles[1], ur.angles[2])
    p2 = (0.63254, -0.20653, ur.height_high, ur.angles[0], ur.angles[1], ur.angles[2])
    p3 = (p1[0] + (0.035 * 6), p1[1] - (0.035 * 1), ur.height_high, ur.angles[0], ur.angles[1], ur.angles[2])
    p4 = (p1[0] + (0.035 * 6), p1[1] - (0.035 * 1), ur.height_low, ur.angles[0], ur.angles[1], ur.angles[2])

    while True:
        ur.rob.movel(p1, ur.a, ur.v)
        ur.rob.movel(p2, ur.a, ur.v)
        ur.rob.movel(p3, ur.a, ur.v)
        ur.rob.movel(p4, ur.a, ur.v)
        ur.rob.movel(p3, ur.a, ur.v)
        ur.rob.movel(p2, ur.a, ur.v)


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
