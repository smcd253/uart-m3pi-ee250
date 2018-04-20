"""EE 250L Lab 07 Skeleton Code

Run rpi_pub_and_sub.py on your Raspberry Pi."""

import paho.mqtt.client as mqtt
import time

import grovepi
from grovepi import *
from grove_rgb_lcd import *
from grove_dht import Dht

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to topics of interest here
    client.subscribe("anrg-pi9/defaultCallback")
    client.subscribe("anrg-pi9/LEDThread")


# #Custom callbacks need to be structured with three args like on_message()
# def LEDThread(client, userdata, message):
#     #the third argument is 'message' here unlike 'msg' in on_message 
#     data = str(message.payload, "utf-8")
#     if ((data == "TOGGLE") and digitalRead(LED) == 0): #if receive message and LED is off
#         digitalWrite(LED, 1) #turn LED on
#     elif ((data == "TOGGLE") and digitalRead(LED) == 1): #if receive message and LED is on
#         digitalWrite(LED, 0) #turn LED off


if __name__ == '__main__':
    #this section is covered in publisher_and_subscriber_example.py
    client = mqtt.Client()
    # client.on_message = on_message
    client.on_connect = on_connect
    client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
    client.loop_start()

    forward = 0x03;
    backward = 0x04;
    right_90 = 0x05;
    left_90 = 0x06;

    while True:
        client.publish("anrg-pi9/LEDThread", forward)
        time.sleep(1)
        client.publish("anrg-pi9/LEDThread", backward)
        time.sleep(1)
        client.publish("anrg-pi9/LEDThread", right_90)
        time.sleep(1)
        client.publish("anrg-pi9/LEDThread", left_90)


