"""EE 250L Lab 07 Skeleton Code

Run rpi_pub_and_sub.py on your Raspberry Pi."""

import paho.mqtt.client as mqtt
import time

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to topics of interest here
    # client.subscribe("anrg-pi9/defaultCallback")
    client.subscribe("m3pi-mqtt-ee250/led-thread")
    client.message_callback_add("m3pi-mqtt-ee250/led-thread", LEDThread)


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

    elem = [1, 3]
    forward = bytearray(elem)
    backward = bytearray([1, 4])
    right_90 = "\x01\0x05"
    left_90 = '\x01\0x06'

    while True:
        # client.publish("m3pi-mqtt-ee250/led-thread", forward)
        time.sleep(1)
        # client.publish("m3pi-mqtt-ee250/led-thread", backward)
        # time.sleep(1)
        # client.publish("m3pi-mqtt-ee250/led-thread", right_90)
        # time.sleep(1)
        # client.publish("m3pi-mqtt-ee250/led-thread", left_90)


