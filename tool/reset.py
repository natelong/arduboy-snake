import serial, time, sys

def usage():
    print "Usage: python <path>/reset.py <port>"

if len(sys.argv) != 2:
    usage()
    exit()

port = sys.argv[1]
ser = serial.Serial(port=port, baudrate=1200)
ser.close()
time.sleep(2)
