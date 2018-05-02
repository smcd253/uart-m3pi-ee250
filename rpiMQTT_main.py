import video_stream
import rpiMQTT_p2ux
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected to server (i.e., broker) with result code "+str(rc))

    #subscribe to topics of interest here
    # client.subscribe("anrg-pi9/defaultCallback")
    client.subscribe("m3pi-mqtt-ee250")

if __name__ == '__main__':
	client = mqtt.Client()
	# client.on_message = on_message
	client.on_connect = on_connect
	client.connect(host="eclipse.usc.edu", port=11000, keepalive=60)
	client.loop_start()
	video_stream.main_meth(client)