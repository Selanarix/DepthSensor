<?php
if(!(isset($_GET["sensor"]) && isset($_GET["value"])))
	exit("No val");

$sensor = $_GET["sensor"];
$value = $_GET["value"]; 

if(!(is_numeric($sensor) && is_numeric($value)))
	exit();
if($value < 0 || $value > 100000)
	exit();
if($sensor < 0 || $sensor >6)
	exit();

if(!is_file("sensorData".$sensor.".txt"))
{
	$fp=fopen("sensorData".$sensor.".txt","w") or die();
	fclose($fp);
}
$lines = file("sensorData".$sensor.".txt");
array_push($lines,$value."\n");

$fp=fopen("sensorData".$sensor.".txt","w") or die();
$sizeOfArray=count($lines);
for($i = 0; $i < $sizeOfArray; $i++)
{
	if(($sizeOfArray - $i) > 10)
		continue;
	fwrite($fp,$lines[$i]);
}
fclose($fp);
?>
