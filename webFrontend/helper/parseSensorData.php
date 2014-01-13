<?php
session_start();

define("FOLDER_OF_XML_DATA","sensorData/");

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
$handle = opendir(FOLDER_OF_XML_DATA);
if ($handle === false) 
    exit();


// parse all files out of this folder 
while (false !== ($entry = readdir($handle))) 
{
    if(strpos($entry,'sensorData') !== false)
	{
        $sensorData = array();
        $sensorData = parseSensorXML(FOLDER_OF_XML_DATA.$entry);
        if($sensorData === false)
           echo "could not read xml file";
        else
           array_push($sensorDataSets,$sensorData);
    }
}

//Store data in the session
$_SESSION['data'] = $sensorDataSets;
?>
