#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 20000))

sock.sendall("test")

data = sock.recv(4096)

print data

sock.close()
