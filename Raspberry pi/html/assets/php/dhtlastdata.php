<?php
session_start(); 

if($_SESSION['lastrowid']!=null)
{
	$lastrowid=$_SESSION['lastrowid'];
}
else
$lastrowid=0;


$con=new SQLite3("/home/pi/Desktop/Project/Data/Sensordata.db");
$result=$con->query('SELECT * FROM Dht ORDER BY id DESC LIMIT 1');
while($row=$result->fetchArray())
{
$lastnewid=$row['id'];
$lasttemperature=$row['temperature'];
$lasthumidity=$row['humidity'];
$lastdate=$row['time'];
}

if($lastnewid!=$lastrowid)
{	
$myobj->temp=$lasttemperature;
$myobj->humi=$lasthumidity;
$myobj->date=$lastdate;
$_SESSION['lastrowid']=$lastnewid;
}else{
$myobj->temp=null;
$myobj->date=null;
$myobj->humi=null;
}
$myjson=json_encode($myobj);
echo $myjson;

?>
