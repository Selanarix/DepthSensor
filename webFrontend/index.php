<?php
$dataFiles = array();
$sensorData = array();

$handle = opendir('.');
if ($handle === false) 
    exit();
/* This is the correct way to loop over the directory. */
while (false !== ($entry = readdir($handle))) 
{
    if(strpos($entry,'sensorData') !== false)
	{
       echo $entry."<br>";
        $dataSet = array();
	    $dataSet = file($entry);
	    array_push($sensorData,$dataSet);
    }
}	
print_r($sensorData);
?>
