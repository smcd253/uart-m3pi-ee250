import serial
import time

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
    ser.write('0') # direction
    ser.write('0') # speed hundreds
    ser.write('5') # speed tens
    ser.write('0') # speed ones

    
    # c = ser.readline()
    # print "received: " + c
    time.sleep(1)