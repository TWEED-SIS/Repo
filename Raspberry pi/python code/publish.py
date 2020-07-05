import paho.mqtt.publish as publish
import random
import time
mqtt_server= "10.0.2.15"
mqtt_topic1= "sensor/Ph"
mqtt_topic2= "sensor/Distance"
mqtt_topic3= "sensor/dht/Temperature"

while(1):
  #  publish.single(mqtt_topic1,random.randint(1,21),hostname=mqtt_server)
   # time.sleep(5)
  #  publish.single(mqtt_topic2,random.randint(55,88),hostname=mqtt_server)
   # time.sleep(5)
    publish.single(mqtt_topic3,random.randint(11,102),hostname=mqtt_server)
    time.sleep(5)

