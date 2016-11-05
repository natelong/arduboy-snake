import serial, time

ser = serial.Serial(port='/dev/cu.usbmodem1411', baudrate=1200)
ser.close()
time.sleep(2)
