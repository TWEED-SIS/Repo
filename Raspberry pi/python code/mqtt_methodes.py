import paho.mqtt.client as mqtt
import time
from databaseManager import *
from datetime import datetime
import datetime
temp=0
humi=0

def on_connect(client ,userdata,flag,rc):
    print("connected with result code "+str(rc))
    client.subscribe("Sensor/Temperature")
    client.subscribe("Sensor/Humidité")
    client.subscribe("Sensor/SoilMoisture")
    client.subscribe("Sensor/waterlevel")
    client.subscribe("Sensor/Ph")
    client.subscribe("Sensor/waterflow")
    client.subscribe("Sensor/Co2")
    client.subscribe("Sensor/Gaz")
    date=datetime.datetime.now()
    print (date)


def on_message(client,userdata,msg):
    t = time.localtime()
    timenow=time.strftime("%H:%M:%S", t)
    date=datetime.datetime.now()
    
    
    with conn:
        
        if msg.topic =="Sensor/Temperature":
            print(msg.topic+" "+str(msg.payload))
            values=int(msg.payload)
            data=(values,timenow,day,month,year)
            insert2table(conn,data,req_insert_Temperature_value)
            print(msg.topic+" "+str(msg.payload))
        
        if msg.topic =="Sensor/Humidity":
            print(msg.topic+" "+str(msg.payload))
            values=int(msg.payload)

            data=(values,timenow,day,month,year)
            insert2table(conn,data,req_insert_Humidity_value)
            print(msg.topic+" "+str(msg.payload))
 
        if msg.topic =="Sensor/Co2":
            print(msg.topic+" "+str(msg.payload))
            values=int(msg.payload)

            data=(values,timenow)
            insert2table(conn,data,req_insert_Co2_value)
            print(msg.topic+" "+str(msg.payload))
    
        if msg.topic =="Sensor/Gaz":
            print(msg.topic+" "+str(msg.payload))
            values=int(msg.payload)

            data=(values,timenow)
            insert2table(conn,data,req_insert_GAz_value)
            print(msg.topic+" "+str(msg.payload))
            
        if msg.topic =="Sensor/SoilMoisture":
            print(msg.topic+" "+str(msg.payload))
            values=int(msg.payload)
            data=(values,timenow)
            insert2table(conn,data,req_insert_SoilMoisture_value)
            print(msg.topic+" "+str(msg.payload))
        
        if msg.topic =="Sensor/waterlevel":
            print (timenow)
            values=int(msg.payload)
            data=(values,timenow)
            insert2table(conn,data,req_insert_Waterlevel_value)
            print(msg.topic+" "+str(msg.payload))
            
        if msg.topic =="Sensor/ph":
            print (timenow)
            values=int(msg.payload)
            data=(values,timenow)
            insert2table(conn,data,req_insert_Ph_value)
            print(msg.topic+" "+str(msg.payload))
            
        if msg.topic =="Sensor/waterflow":
            values=msg.payload
            info = values.split(";")
            waterlose=int(info[0])
            flowrate=int(info[1])
            data=(flowrate,waterlose,timenow)
            insert2table(conn,data,req_insert_Waterflow_value)
            print ("flowrate "+str(flowrate))
            print("dht data saved")
        
        
        
    
    
        
        
        



