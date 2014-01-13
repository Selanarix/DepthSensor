<?php
define("FILE_PREFIX","sensorData/sensorData");
define("DEFAULT_HISTORY_SIZE",10);
define("LOG_FILE_NAME","log/sensorInput.log");

/**
* Extracts GET parameter for new sensor value and tests if 
* value is valid. 
*/
function parseGET() 
{
    $dataSet = array();
    if(!(isset($_GET["sensor"]) && isset($_GET["value"])))
	    logger("ERROR", "Could not find any value in URL");

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

function setUpDefaultXML($dataSet, $xml)
{
    //Create root element
    $sensor = $xml->createElement("sensor");
    //Attach sensor id attribute to rooot
    $idAttribute = $xml->createAttribute('sensorID');
    $idAttribute->value = $dataSet["sensorID"];  
    $sensor->appendChild($idAttribute);
   
    //Create configuration element
    $configuration = $xml->createElement("configuration");

    $name = $xml->createElement("NameForID");  
    $name->appendChild($xml->createTextNode($dataSet["sensorID"]));
    $configuration->appendChild($name);

    $desc = $xml->createElement("TankDescription");  
    $desc->appendChild($xml->createTextNode("Tank with id: ".$dataSet["sensorID"]));
    $configuration->appendChild($desc);
    
    $sizeOfHistory = $xml->createElement("SizeOfHistory"); 
    $sizeOfHistory->appendChild($xml->createTextNode(DEFAULT_HISTORY_SIZE));
    $configuration->appendChild($sizeOfHistory);

    $height = $xml->createElement("TankHeight"); 
    $height->appendChild($xml->createTextNode('0'));
    $configuration->appendChild($height);

    $floorArea = $xml->createElement("floorArea");
    $floorArea->appendChild($xml->createTextNode('0'));
    $configuration->appendChild($floorArea);

    $colorOfGraph = $xml->createElement("ColorOfGraph");
    $colorOfGraph->appendChild($xml->createTextNode('red'));
    $configuration->appendChild($colorOfGraph);

    $measurements = $xml->createElement("measurements");

    $sensor->appendChild($configuration);
    $sensor->appendChild($measurements);      
    $xml->appendChild($sensor); //Add root to document
}

function openOrCreateXML($xml,$dataSet,$filename)
{
    //Create file if not exist 
    if(!is_file($filename))
        $xml = setUpDefaultXML($dataSet,$xml);
    else    //Load XML
      return $xml->load($filename);            
}

function addMeasurement($xml, $dataSet)
{
    $historySizeList =  $xml->getElementsByTagName("SizeOfHistory");
    if($historySizeList->length < 1)
        return false;

    $size = $historySizeList->item(0)->nodeValue;
    if(!is_numeric($size) || $size < 0)
        return false;
 
    $measurementsList =  $xml->getElementsByTagName("measurements");
    if($measurementsList->length < 1)
        return false;
    $measurementsNode = $measurementsList->item(0);
    
    //Create new node and add
    $newMeasurement = $xml->createElement("measurement");
    $newMeasurement->appendChild($xml->createTextNode($dataSet["sensorValue"]));

    $timeAttribute = $xml->createAttribute('measurementTime');
    $timeAttribute->value = $dataSet["time"];  
    $newMeasurement->appendChild($timeAttribute);

    $measurementsNode->appendChild($newMeasurement);

    //Delete oldes measurements if history size is reached
    $measurementList = $xml->getElementsByTagName("measurement");
    $index = 0;
    //echo $measurementList->length;
    while($measurementList->length - $index > $size)
    {
        $measurementsNode->removeChild($measurementList->item($index));
        $index++;
    }
}

function logger($level, $message)
{
    $fp = fopen(LOG_FILE_NAME,"a") or die();
    fwrite($fp,date("d.m.Y H:i:s",time())." : ".$level." -> ".$message."\n\r");
    fclose($fp);
	exit();
   // exit(date("d.m.Y H:i:s",time())." : ".$level." -> ".$message); //For debug
}

if(($dataSet = parseGET()) === false)
    logger("ERROR","Could not parse GET request. Parameters malformed");

$filename = FILE_PREFIX.$dataSet["sensorID"].".xml";
$xml = new DOMDocument('1.0', 'utf-8');

if((openOrCreateXML($xml,$dataSet,$filename)) === false)
    logger("ERROR","Was not able to open or create XML file for sensor");

if((addMeasurement($xml,$dataSet)) === false)
    logger("ERROR","Was not able to add new measurement");

$xml->formatOutput = true;
$xml->preserveWhiteSpace = false;

if($xml->save($filename) === false)
    logger("ERROR", "Was not able to save altered xml data");

exit(); //Do not send response
?>
