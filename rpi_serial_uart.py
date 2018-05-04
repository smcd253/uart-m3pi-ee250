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
    time.sleep(1)
    print("sending synch")
    A = bytes('A')
    ser.write(A)
    ser.write('\n')
    # rcv = readLine(ser)
    # print "received:", rcv