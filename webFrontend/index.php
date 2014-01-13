<?php
// Standard inclusions   
include("graph/pChart/pData.class");
include("graph/pChart/pChart.class");


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

function htmlPrint_r($data)
{
    echo '<pre>';
    print_r($data);
    echo '</pre>';
}

$dataFiles = array();
$sensorDataSets = array();

$handle = opendir('.');
if ($handle === false) 
    exit();
/* This is the correct way to loop over the directory. */
while (false !== ($entry = readdir($handle))) 
{
    if(strpos($entry,'sensorData') !== false)
	{
        $sensorData = array();
        $sensorData = parseSensorXML($entry);
        if($sensorData === false)
           echo "could not read xml file";
        else
           array_push($sensorDataSets,$sensorData);
    }
}

//Generate graph images for sensors
$DataSet = new pData;
$Graph = new pChart(700,230);

$DataSet = graphDefineData($sensorDataSets);
$Graph = graphInitGraph($DataSet);
graphRender($DataSet, $Graph);


function graphDefineData($sensorDataSets)
{	
	$DataSet = new pData;
	$i = 0;
	foreach ($sensorDataSets as $sensorSet)
	{	
		$values = array();
		$time = array();
		foreach ($sensorSet["depthMeasurements"] as $set)
		{
			
			array_push($values, $set["value"]);
			array_push($time, $set["time"]);
		}
		//print_r($values);
		//print_r($time);
		$nameOfVal = "Serie".$i;
		$nameOfX = "XLabel".$i;
		
		$DataSet->AddPoint($values,$nameOfVal);
		$DataSet->AddPoint($time,$nameOfX);
		$DataSet->SetAbsciseLabelSerie($nameOfX);
		$DataSet->AddAllSeries();
		$DataSet->RemoveSerie($nameOfX);   
		
		
		$DataSet->SetSerieName("Time".$i,$nameOfX);
		$DataSet->SetSerieName("Depth".$i,$nameOfVal);
		
		//Set Timestemp as X-Axis and Value to Y-Axis
		$DataSet->setXAxisName("Time");
		$DataSet->setXAxisFormat("time");
		
		$DataSet->setYAxisName("Depth");
		$DataSet->setYAxisUnit("cm");
		
        $i++;
	}
	
	return $DataSet;
}

function graphInitGraph($DataSet)
{
	// Initialise the graph
	 $Graph = new pChart(700,230);
	 $Graph->setFixedScale(0,50);
	 $Graph->setFontProperties("graph/Fonts/tahoma.ttf",8);
	 $Graph->setGraphArea(50,30,585,200);
	 $Graph->drawFilledRoundedRectangle(7,7,693,223,5,240,240,240);
	 $Graph->drawRoundedRectangle(5,5,695,225,5,230,230,230);
	 $Graph->drawGraphArea(255,255,255,TRUE);
	 $Graph->drawScale($DataSet->GetData(),$DataSet->GetDataDescription(),SCALE_NORMAL,150,150,150,TRUE,0,2);
	 $Graph->drawGrid(4,TRUE,230,230,230,50);
	
	// Draw the 0 line   
	$Graph->setFontProperties("Fonts/tahoma.ttf",6);   
	$Graph->drawTreshold(0,143,55,72,TRUE,TRUE);   
	return $Graph;
}

function graphRender($DataSet, $Graph)
{
	// Draw the cubic curve graph  
	$Graph->drawFilledLineGraph($DataSet->GetData(),$DataSet->GetDataDescription(),50,TRUE);
	// Finish the graph  
	$Graph->setFontProperties("graph/Fonts/tahoma.ttf",8);  
	//$Graph->drawLegend(45,35,$DataSet->GetDataDescription(),255,255,255);  
	$Graph->setFontProperties("graph/Fonts/tahoma.ttf",10);  
	$Graph->drawTitle(60,22,"Amount of water during time",50,50,50,585);  
	$Graph->Render("Naked.png"); 
}
?>


<?php echo'<?xml version="1.0" encoding="utf-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN"
    "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
 <title>Cister fill level monitor</title>
</head>
<body>
    <h1>Cister fill level monitor</h1>
    <p>There are <?php echo count($sensorDataSets)?> sensors under monitoring</p>
    
	<img src="Naked.png" />
	<p><?php echo htmlPrint_r($sensorDataSets);?></p>
	<!--<?php 	phpinfo(); 	?>-->
</body>
</html>
