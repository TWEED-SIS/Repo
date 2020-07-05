import paho.mqtt.client as mqtt
from mqtt_methodes import *

client=mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message
client.connect("10.0.2.15",1883,60)
client.loop_forever()
		
