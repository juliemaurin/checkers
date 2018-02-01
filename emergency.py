import urx
rob = urx.Robot("192.168.1.196")

try:
    while(1):
        rob.stopl(150)
except KeyboardInterrupt:
    print "MANUALY INTERRUPTED"    
finally:
    rob.close()
