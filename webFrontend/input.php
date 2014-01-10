<?php
define("FILE_PREFIX","sensorData");
define("DEFAULT_HISTORY_SIZE",10);
define("LOG_FILE_NAME","sensorInput.log");

/**
* Extracts GET parameter for new sensor value and tests if 
* value is valid. 
*/
function parseGET() 
{
    $dataSet = array();
    if(!(isset($_GET["sensor"]) && isset($_GET["value"])))
	    exit("No val");

    $dataSet["sensorID"] = $_GET["sensor"];
    $dataSet["sensorValue"] = $_GET["value"]; 
    $dataSet["time"] = time();

    if(!(is_numeric($dataSet["sensorValue"]) && is_numeric($dataSet["sensorID"])))
	    return false;
    if( $dataSet["sensorValue"] < 0 ||  $dataSet["sensorValue"] > 100000)
	    return false;
    if($dataSet["sensorID"] < 0 || $dataSet["sensorID"] >6)
	    return false;

    return $dataSet; 
}

function updateSensorXML($dataSet)
{
    $filename = FILE_PREFIX.$dataSet["sensorID"].".xml";

    $xml = new DOMDocument();
    //Create file if not exist 
    if(!is_file($filename))
    {
        $root = $xml->createElement("measurements");
        $idAttribute = $xml->createAttribute('sensorID');
        $idAttribute->value = $dataSet["sensorID"];  
        $sizeOfHistoryAttribute = $xml->createAttribute('sizeOfHistory');
        $sizeOfHistoryAttribute->value = DEFAULT_HISTORY_SIZE;  
  
  
        $root->appendChild($idAttribute);
        $root->appendChild($sizeOfHistoryAttribute);      
        $xml->appendChild($root);

        $xml->formatOutput = true;

        $xml->save($filename) or logger("Error");
    }
    else    //Load XML
        $xml->load($filename);

}

function logger($level, $message)
{
    //TODO: loggn
    exit(date("d.m.Y H:i:s",time())." : ".$level." -> ".$message);
}


if(($dataSet = parseGET()) === false)
    logger("ERROR","Could not parse GET request. Parameters malformed");

updateSensorXML($dataSet);



/*
$fp=fopen(FILE_PREFIX.$sensor.".txt","w") or die();
$sizeOfArray=count($lines);
for($i = 0; $i < $sizeOfArray; $i++)
{
	if(($sizeOfArray - $i) > 10)
		continue;
	fwrite($fp,$lines[$i]);
}
fclose($fp);*/
?>
