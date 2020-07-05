import sqlite3
from sqlite3 import Error

def create_connection(db_file):
    conn=None
    try:
        conn = sqlite3.connect(db_file)
    except Error as e:
        print(e)
    return conn

def creat_table(conn,creat_table_sql):
    try:
        c=conn.cursor()
        c.execute(creat_table_sql)
    except Error as e:
        print(e)

def insert2table(conn, data, sql_req):
    cur=conn.cursor()
    cur.execute(sql_req, data)
    return cur.lastrowid


req_Temperature_table="""CREATE TABLE IF NOT EXISTS Temperature(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             temperature integer,
                             time text,
                             );"""
req_insert_Temperature_value='''INSERT INTO Temperature(temperature,time)
                                VALUES(?,?) '''


req_Humidity_table="""CREATE TABLE IF NOT EXISTS Humidity(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             humidity integer,
                             time text,
                             );"""
req_insert_Humidity_value='''INSERT INTO Humidity(humidity,time)
                                VALUES(?,?) '''


req_Co2_table="""CREATE TABLE IF NOT EXISTS Co2(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             co2 integer,
                             time text,
                             );"""
req_insert_Co2_value='''INSERT INTO Co2(co2,time)
                                VALUES(?,?) '''


req_Gaz_table="""CREATE TABLE IF NOT EXISTS Gaz(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             gaz integer,
                             time text,
                             );"""
req_insert_Gaz_value='''INSERT INTO Gaz(gaz,time)
                                VALUES(?,?) '''

req_Ph_table="""CREATE TABLE IF NOT EXISTS Ph(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             ph integer,
                             time text,
                             );"""
req_insert_Ph_value='''INSERT INTO Ph(ph,time)
                                VALUES(?,?,?,?,?) '''


req_SoilMoisture_table="""CREATE TABLE IF NOT EXISTS SoilMoisture(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             soilmoisture integer,
                             time text,
                             );"""
req_insert_SoilMoisture_value='''INSERT INTO SoilMoisture(soilmoisture,time)
                                VALUES(?,?) '''









req_Waterlevel_table="""CREATE TABLE IF NOT EXISTS Waterlevel(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             waterlevel integer,
                             date text
                             );"""
req_insert_Waterlevel_value='''INSERT INTO Waterlevel(waterlevel,date,day,month,year)
                                VALUES(?,?,?,?,?) '''


req_Waterflow_table="""CREATE TABLE IF NOT EXISTS Waterflow(
                             id integer PRIMARY KEY AUTOINCREMENT,
                             flowrate integer,
                             time text
                             );"""
req_insert_Waterflow_value='''INSERT INTO Waterflow(flowrate,time)
                                VALUES(?,?) '''












