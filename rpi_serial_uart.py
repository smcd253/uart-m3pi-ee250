import serial
import time

# port = "/dev/ttyS0"    # Raspberry Pi 3
port = "/dev/serial0"    # Raspberry Pi 3

def readLine(port):
    s = ""
    while True:
        ch = port.read()
        s += ch
        if ch == '\r':
            return s

ser = serial.Serial(port, baudrate = 9600)
print("starting")
while True:
    ser.write('0') # fwd
    ser.write('100') # speed
    
    # c = ser.readline()
    # print "received: " + c
    time.sleep(1)