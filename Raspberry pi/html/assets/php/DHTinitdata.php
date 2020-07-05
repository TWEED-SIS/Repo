<?php
session_start(); 
$labels=array();
$Temperature=array();
$Humidity=array();



$con=new SQLite3("/home/pi/Desktop/Project/Data/Sensordata.db");
$result=$con->query('SELECT * FROM Dht ORDER BY id DESC LIMIT 1');
while($row=$result->fetchArray())
{
$lastrowid=$row['id'];
//echo $lastrowid;
}
$_SESSION['lastrowid']=$lastrowid; 

$res=$con->query('SELECT * FROM (SELECT * FROM Dht ORDER BY id DESC LIMIT 20) ORDER BY id ASC');
while($row=$res->fetchArray()){
	array_push($Temperature,$row['temperature']);
	array_push($labels,$row['time']);
    array_push($Humidity,$row['humidity']);
    $gaugetemp=$row['temperature'];
	$gaugehum=$row['humidity'];

}
$myobj=array("temperature"=>$Temperature,"labels"=>$labels,"humidity"=>$Humidity,"gaugetemperature"=>$gaugetemp,"gaugehumidity"=>$gaugehum);
$myjson=json_encode($myobj);
echo $myjson;
?>
