<?php
$files = readdir(".");
$dataFiles = array();
foreach($files as $file)
	if(strpos($a,'sensorData') !== false)
		array_push($dataFiles,$file);


$sensorData = array(array());
foreach($dataFiles as $inputSource)
{
	$dataSet = array();
	$dataSet = files($inputSource);
	array_push($sensorData,$dataSet);
}
print_r($sensorData);
?>
