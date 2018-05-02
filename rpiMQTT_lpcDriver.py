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
    client.subscribe("anrg-pi2/intersection")
    client.message_callback_add("anrg-pi2/intersection", intersection_callback)

def intersection_callback(client, userdata, message):
    data = str(message.payload, "utf-8")
    if data == "right":
        client.publish("m3pi-mqtt-ee250", right_90)
    elif data == "left":
        client.publish("m3pi-mqtt-ee250", left_90)

#Custom callbacks need to be structured with three args like on_message()
def LEDThread(client, userdata, message):
    #the third argument is 'message' here unlike 'msg' in on_message 
    data = str(message.payload, "utf-8")
    print(data)

if __name__ == '__main__':
    #this section is covered in publisher_and_subscriber_example.py
    client = mqtt.Client()
    # client.on_message = on_message
    client.on_connect = on_connect
    client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
    client.loop_start()

    forward = bytearray([1, 3])
    backward = bytearray([1, 4])
    right_90 = bytearray([1, 5])
    left_90 = bytearray([1, 6])

    while True:
        client.publish("m3pi-mqtt-ee250", forward)
        time.sleep(1)
        client.publish("m3pi-mqtt-ee250", backward)
        time.sleep(1)
        client.publish("m3pi-mqtt-ee250", right_90)
        time.sleep(1)
        client.publish("m3pi-mqtt-ee250", left_90)


