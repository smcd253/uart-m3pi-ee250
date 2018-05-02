"""EE 250L Lab 07 Skeleton Code

Run rpi_pub_and_sub.py on your Raspberry Pi."""

import paho.mqtt.client as mqtt
import time

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to topics of interest here
    # client.subscribe("anrg-pi9/defaultCallback")
    client.subscribe("m3pi-mqtt-ee250")
    client.message_callback_add("m3pi-mqtt-ee250", LEDThread)


#Custom callbacks need to be structured with three args like on_message()
def LEDThread(client, userdata, message):
    #the third argument is 'message' here unlike 'msg' in on_message 
    data = str(message.payload)
    # print(data)

# simple PID control based on deviation from middle based on 'x'

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


def main_meth(client, x):

    #setup the keyboard event listener
     # start to listen on a separate thread
    print(x)
    #this section is covered in publisher_and_subscriber_example.py

    forward = bytearray([1, 3])
    backward = bytearray([1, 4])
    right = bytearray([1, 5])
    left = bytearray([1, 6])
    stop = bytearray([1, 7])

    h = 1 # PID execution frequency
    #def pid_controller(y, yc, h=1, Ti=1, Td=1, Kp=1, u0=0, e0=0)
    ctrl = pid_controller(x, 0, h, 1, 1, 1, 0, 0)
    control = int(next(ctrl)) 
    print(control)
    #grab last element of pid_controller() generator
    corr = 1 # correction constant

    # decide left/right
    # if control < 0 --> m3pi is drifting right of line, 
    # use control variable to increase speed of left motor
    if (control > 0): # right
        right.append(corr * control)
        print(right)
        client.publish("m3pi-mqtt-ee250", right)
        right = bytearray([1, 5])
    elif (control < 0): #left
        control = control * (-1) # cannot pass negative values into bytearray            
        left.append(corr * control)
        print(left)
        client.publish("m3pi-mqtt-ee250", left)
        left = bytearray([1, 6])
    else: #centered
        print(stop)
        client.publish("m3pi-mqtt-ee250", stop)