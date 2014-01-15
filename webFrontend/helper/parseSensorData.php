<?php
session_start();

define("FOLDER_OF_XML_DATA","sensorData/");

$path = FOLDER_OF_XML_DATA;
if(isset($_GET["pathToFolder"]))
	$path = $_GET["pathToFolder"];

function parseSensorXML($filename)
{
    $dataSet = array();
    
    if(empty($filename) || !file_exists($filename))
        return false;

    $sensor = simplexml_load_file($filename);
    $configTag = $sensor->configuration;

    //parse config part
    $dataSet["floorArea"] = (int)$configTag->floorArea;
    $dataSet["tankHeight"] = (int)$configTag->TankHeight;
    $dataSet["colorGraph"] = (string)$configTag->ColorOfGraph;
    $dataSet["sensorUnit"] = (string)$configTag->SensorUnit;
    $dataSet["sensorType"] = (string)$configTag->SensorType;
    $dataSet["name"] = (string)$configTag->NameForID;
    $dataSet["description"] = (string)$configTag->TankDescription;
    $dataSet["sizeOfHistory"] = (int)$configTag->SizeOfHistory;
    $dataSet["id"] = (int)$sensor["sensorID"];
    $dataSet["depthMeasurements"] = array();

        /* For each <character> node, we echo a separate <name>. */
    foreach ($sensor->measurements->measurement as $measure) 
    {
       $measureSet = array();
       $measureSet["value"] = (int)$measure;
       $measureSet["time"] = (int)$measure["measurementTime"];
       array_push($dataSet["depthMeasurements"],$measureSet);
    }

    return $dataSet;
}

$sensorDataSets = array();

//Try to open dir with xml data
$handle = opendir($path);
if ($handle === false) 
{
/*	$fp = fopen("test.txt","a");
	fwrite($fp,"ERROR Could not open folder for reading\r\n");
	fclose($fp);
*/
    exit();
}

// parse all files out of this folder 
while (false !== ($entry = readdir($handle))) 
{
    if(strpos($entry,'sensorData') !== false)
	{
        $sensorData = array();
        $sensorData = parseSensorXML($path.$entry);
        if($sensorData === false)
           echo "could not read xml file";
        else
           $sensorDataSets[$sensorData["id"]] = $sensorData;
    }
}
/*
$fp = fopen("test.txt","a");
fwrite($fp,"INFO Update session context\r\n");
fclose($fp);
*/
//Store data in the session
$_SESSION['data'] = $sensorDataSets;
?>
