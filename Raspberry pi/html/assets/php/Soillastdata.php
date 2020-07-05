<?php
session_start(); 

if($_SESSION['lastrowidsoilmoisture']!=null)
{
	$lastrowidsoilhum=$_SESSION['lastrowidsoilmoisture'];
}else
{
$lastrowidsoilhum=0;
}
if($_SESSION['lastrowidph']!=null)
{
	$lastrowidph=$_SESSION['lastrowidph'];
}else
{
$lastrowidph=0;
}



$con=new SQLite3("/home/pi/Desktop/Project/Data/Sensordata.db");
$result=$con->query('SELECT * FROM SoilMoisture ORDER BY id DESC LIMIT 1');
while($row=$result->fetchArray())
{
$lasthumid=$row['id'];
$lasthumidity=$row['soilmoisture'];
$lastdatehum=$row['time'];
}
$result1=$con->query('SELECT * FROM Ph ORDER BY id DESC LIMIT 1');
while($row=$result1->fetchArray())
{
$lastphid=$row['id'];
$lastph=$row['ph'];
$lastdateph=$row['time'];
}


if($lasthumid!=$lastrowidsoilhum)
{
$myobj->humi=$lasthumidity;
$myobj->datehum=$lastdatehum;
$_SESSION['lastrowidsoilmoisture']=$lasthumid;
}else{
$myobj->humi=null;
$myobj->datehum=null;
}

if($lastphid!=$lastrowidph)
{
$myobj->ph=$lastph;
$myobj->dateph=$lastdateph;
$_SESSION['lastrowidph']=$lastphid;
}else{
$myobj->ph=null;
$myobj->dateph=null;
}


$myjson=json_encode($myobj);
echo $myjson;



?>
