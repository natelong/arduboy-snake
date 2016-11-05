import serial, time, sys

def usage():
    print "Usage: python <path>/reset.py <port>"

if len(sys.argv) != 2:
    usage()
    exit()

port = sys.argv[1]
try:
    ser = serial.Serial(port=port, baudrate=1200)
except serial.SerialException:
    print "Couldn't connect to device at", port
    exit(1)
else:
    ser.close()
    time.sleep(2)
