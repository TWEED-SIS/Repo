<?php
session_start(); 
$lastrowidsm=null;
$con=new SQLite3("/home/pi/Desktop/Project/Data/Sensordata.db");
$result=$con->query('SELECT * FROM SoilMoisture ORDER BY id DESC LIMIT 1');
while($row=$result->fetchArray())
{
$lastrowidsm=$row['id'];
#echo $lastrowid;
}
$_SESSION['lastrowidsoilmoisture']=$lastrowidsm; 

$result1=$con->query('SELECT * FROM Ph ORDER BY id DESC LIMIT 1');
while($row=$result->fetchArray())
{
$lastrowidph=$row['id'];
#echo $lastrowid;
}
$_SESSION['lastrowidph']=$lastrowidph; 



$labels=array();
$SoilHumidity=array();
$Soilph=array();


$res=$con->query('SELECT * FROM (SELECT * FROM SoilMoisture ORDER BY id DESC LIMIT 10) ORDER BY id ASC');
while($row=$res->fetchArray()){
	array_push($labels,$row['time']);
	array_push($SoilHumidity,$row['soilmoisture']);
	$humgg=$row['soilmoisture'];
}
$res=$con->query('SELECT * FROM (SELECT * FROM Ph ORDER BY id DESC LIMIT 10) ORDER BY id ASC');
while($row=$res->fetchArray()){
	array_push($labels,$row['time']);
	array_push($Soilph,$row['ph']);
	$phgg=$row['ph'];
}
sort($labels);


$myobj=array("SoilHumidite"=>$SoilHumidity,"SoilPh"=>$Soilph,"labels"=>$labels,"humiditegauge"=>$humgg,"phgauge"=>$phgg);
$myjson=json_encode($myobj);
echo $myjson;



?>
