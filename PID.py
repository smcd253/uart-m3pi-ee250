import serial
import time

port = "/dev/serial0"    # Raspberry Pi 3

ser = serial.Serial(port, baudrate = 9600)
print("Starting Serial")

def pid_controller(y, yc, h, Ti, Td, Kp, u0, e0):
	"""Calculate System Input using a PID Controller

	Arguments:
	y  .. Measured Output of the System
	yc .. Desired Output of the System
	h  .. Sampling Time
	Kp .. Controller Gain Constant
	Ti .. Controller Integration Constant
	Td .. Controller Derivation Constant
	u0 .. Initial state of the integrator
	e0 .. Initial error

	Make sure this function gets called every h seconds!
	"""
	
	# Step variable
	k = 0

	# Initialization
	ui_prev = u0
	e_prev = e0

	while 1:

		# Error between the desired and actual output
		e = yc - y

		# Integration Input
		ui = ui_prev + 1/Ti * h*e
		# Derivation Input
		ud = 1/Td * (e - e_prev)/h

		# Adjust previous values
		e_prev = e
		ui_prev = ui

		# Calculate input for the system
		u = Kp * (e + ui + ud)
		
		k += 1

		yield u


def main_meth(x):
    h = 1 # PID execution frequency
    #def pid_controller(y, yc, h=1, Ti=1, Td=1, Kp=1, u0=0, e0=0)
    ctrl = pid_controller(x, 0, h, 1, 1, 1, 0, 0)
    #grab last element of pid_controller() generator
    
    control = int(next(ctrl)) 
    corr = 1 # correction constant
    _speed = control * corr

    if _speed > 255:
        _speed = 255
    elif _speed < -255:
        _speed = -255
    # decide left/right
    # if control < 0 --> m3pi is drifting right of line, 
    # use control variable to increase speed of left motor
    # 0: FORWARD, 1: REVERSE, 2: RIGHT_STILL, 3: LEFT_STILL, 4: STOP
    if (_speed > 0): # right
        speed = [chr(_speed // 100), chr(_speed // 10), chr(_speed // 1)]        
        ser.write('2')
        ser.write(speed[0])
        ser.write(speed[1])
        ser.write(speed[2])

    elif (_speed < 0): #left
        _speed = _speed * (-1) # cannot pass negative values into bytearray   
        speed = [chr(_speed // 100), chr(_speed // 10), chr(_speed // 1)]
        ser.write('3')
        ser.write(speed[0])
        ser.write(speed[1])
        ser.write(speed[2])         
        
    else: #centered
        ser.write('4')
        ser.write(speed[0])
        ser.write(speed[1])
        ser.write(speed[2]) 
