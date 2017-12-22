#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import socket
import test_urx
import wsg50

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 10000)
sock.bind(server_address)

#Calling listen() puts the socket into server mode, and accept() waits for an incoming connection.
# Listen for incoming connections
sock.listen(1)

wsg50.connect(test_urx.s)

while True:
    # Wait for a connection
    connection, client_address = sock.accept()

    # accept() returns an open connection between the server and client, along with the address of the client. The connection is actually a different socket on another port (assigned by the kernel). Data is read from the connection with recv() and transmitted with sendall().
    try:
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
            if data:
                mv = data.split(' ')
                print "(from, to) = ", mv
                test_urx.move_piece(int(mv[0]), int(mv[2]), test_urx.s)

                connection.sendall(data)
            else:
                break
    finally:
        # Disconnect gripper
        #wsg50.disconnect(test_urx.s)
        # Clean up the connections
        connection.close()
