#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import socket
import urx_control
import wsg50_control
import sys

try:
    print "Starting game server"
    # Create game server socket
    game_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    game_sock.bind(("127.0.0.1", 10000))

    # Calling listen() puts the socket into server mode,
    # and accept() waits for an incoming connection.
    # Listen for incoming connections
    game_sock.listen(1)

    # Connect to the gripper
    print "Connecting to gripper"
    wsg = wsg50_control.WSG50()
    wsg.connect()


    # Connect to the Robot
    print "Connecting to robot"
    ur = urx_control.Robot()

    while True:
        # Wait for a connection
        print "Waiting for connection"
        connection, client_address = game_sock.accept()

        # accept() returns an open connection between the server and client,
        # along with the address of the client.
        # The connection is actually a different socket on another port
        # (assigned by the kernel).
        # Data is read from the connection with recv()
        # and transmitted with sendall().
        try:
            # Receive the data in small chunks and retransmit it
            while True:
                print "Receiving data"
                data = connection.recv(16)
                if data:
                    mv = data.split(' ')
                    print "Received move : ", mv
                    ur.move_piece(int(mv[0]), int(mv[2]), wsg)

                    index_high = max(int(mv[0]), int(mv[2]))
                    index_low = min(int(mv[0]), int(mv[2]))
                    if (index_high - index_low > 6):
                        # Compute the position of the "taken piece" while jumping
                        print "It's a jump"

                        taken_piece = (index_high + index_low) / 2
                        if (((index_low / 4) % 2) != 0):
                            taken_piece += 1

                        ur.remove_piece(taken_piece, wsg)

                    ur.go_home()
                    print "Sending reply"
                    connection.sendall(data)
                else:
                    break
        finally:
            # Clean up the connections
            print "Cleaning up connection"
            connection.close()

except KeyboardInterrupt:
    print "MANUALY INTERRUPTED"
except:
    print "UNEXPECTED ERROR"
    print sys.exc_info()
finally:
    print "TERMINATING..."
    ur.rob.stopl()
    ur.rob.close()
    wsg.disconnect()
