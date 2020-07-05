import sqlite3
from sqlite3 import Error
from sqlite3_methodes import *


database_file="Data/Sensordata.db"
conn=create_connection(database_file)

if conn is not None:
    creat_table(conn,req_Temperature_table)
    creat_table(conn,req_Humidity_table)
    creat_table(conn,req_SoilMoisture_table)
    creat_table(conn,req_Waterflow_table)
    creat_table(conn,req_Ph_table)
    creat_table(conn,req_Waterlevel_table)
    creat_table(conn,req_Co2_table)
    creat_table(conn,req_Gaz_table)
    print(" database connection  tab created")
else:
    print("Error ! cannot creat database connection")
