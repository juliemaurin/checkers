#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import urx
import wsg50
import socket
from time import sleep
# URscript reference : http://www.sysaxes.com/manuels/scriptmanual_en_3.1.pdf

def get_pos(n):
    i = n % 4
    j = n // 4
    dx = 2 * i * delta[0]
    if (j % 2): dx += delta[0]
    dy = j * delta[1]
    return dx, dy

def move_piece(frm, to, s):
    # Get "from" positions
    pos_from_offset = get_pos(frm)
    print "From offset = ", pos_from_offset
    pos_from_high = (start[0] + pos_from_offset[0], start[1] + pos_from_offset[1], height_high, angles[0], angles[1], angles[2])
    pos_from_low = (start[0] + pos_from_offset[0], start[1] + pos_from_offset[1], height_low, angles[0], angles[1], angles[2])

    # Get "to" positions
    pos_to_offset = get_pos(to)
    print "To offset = ", pos_to_offset
    pos_to_high = (start[0] + pos_to_offset[0], start[1] + pos_to_offset[1], height_high, angles[0], angles[1], angles[2])
    pos_to_low = (start[0] + pos_to_offset[0], start[1] + pos_to_offset[1], height_low, angles[0], angles[1], angles[2])

    print "Step 1"
    rob.movel(pos_from_high, a, v)
    print "Step 2"
    rob.movel(pos_from_low, a, v)
    wsg50.grasp(s)
    sleep(0.2)
    #TODO : Close gripper here
    print "Step 3"
    rob.movel(pos_from_high, a, v)

    print "Step 4"
    rob.movel(pos_to_high, a, v)
    print "Step 5"
    rob.movel(pos_to_low, a, v)
    wsg50.release(s)
    sleep(0.2)
    #TODO : Open gripper here
    print "Step 6"
    rob.movel(pos_to_high, a, v)
    sleep(0.1)

def bin_to_move(n):
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

def reset_pos():
    init = (0.63254, -0.20653, height_low, angles[0], angles[1], angles[2])
    rob.movel(init, 2000, 2000)


a = 10
v = 10
wsg50_weight = 1.2

start = [0, 0]
height_high = 0.06
height_low = 0.025
angles = [2.22, 2.22, -0.01]
delta = [0.035, -0.035]

# Connect to the robot
rob = urx.Robot("192.168.1.196")
rob.set_tcp((0, 0, 0.17, 0, 0, 0))
rob.set_payload(wsg50_weight, (0, 0, 0))

# Connect gripper
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#TODO : Edit this
reset_pos()

print "MAKE SURE TO MOVE THE ROBOT TO THE ORIGIN BEFORE START"
_ = raw_input("(Press enter to continue)")

start = rob.getl()[:2]

print start

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
