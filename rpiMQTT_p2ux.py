"""EE 250L Lab 07 Skeleton Code

Run rpi_pub_and_sub.py on your Raspberry Pi."""

import paho.mqtt.client as mqtt
import video_stream
import time
from multiprocessing import Process, Queue

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))
    # No subscriptions needed. This file is only pulishing


def main_meth(q):
    #setup the keyboard event listener
    #this section is covered in publisher_and_subscriber_example.py
    client = mqtt.Client()
    # client.on_message = on_message
    client.on_connect = on_connect
    client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
    client.loop_start()
    while True:
        print(q.get())
        client.publish("anrg-pi2/position", "HELLO MOTO")
        if type(data) == int:
            if data > 0:
                client.publish("anrg-pi2/position", "Turning Left")
            elif data < 0:
                client.publish("anrg-pi2/position", "Turning Right")
        else:
            client.publish("anrg-pi2/position", data)