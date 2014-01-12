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

function graphDefineData($sensorDataSets)
{	
	$DataSet = new pData;
	
	foreach ($sensorDataSets as $sensorSet)
	{	
		$values[] = 0;
		$time[] = 0;
		foreach ($sensorSet["depthMeasurements"] as $set)
		{
			
			array_push($values, $set["value"]);
			array_push($time, $set["time"]);
		}
		//print_r($values);
		//print_r($time);
		$nameOfVal = "Values".$i;
		
		$DataSet->AddPoint($value,$nameOfVal);
		$DataSet->AddPoint($time,"Timestamp");
		$DataSet->SetSerieName("Timestamp","Time");
		$DataSet->SetSerieName($nameOfVal,"Depth");
		
		//Set Timestemp as X-Axis and Value to Y-Axis
		$DataSet->SetAbsciseLabelSerie("Timestamp");
		$DataSet->setXAxisName("Time");
		$DataSet->setXAxisFormat("H:i");
		
		$DataSet->setYAxisName(0, "Depth");
		$DataSet->setYAxisUnit(0, "cm");
		
		$DataSet->AddAllSeries();
	}
	
	return $DataSet;
}

function graphInitGraph($DataSet)
{
	// Initialise the graph
	 $Test = new pChart(700,230);
	 $Test->setFixedScale(0,50);
	 $Test->setFontProperties("Fonts/tahoma.ttf",8);
	 $Test->setGraphArea(50,30,585,200);
	 $Test->drawFilledRoundedRectangle(7,7,693,223,5,240,240,240);
	 $Test->drawRoundedRectangle(5,5,695,225,5,230,230,230);
	 $Test->drawGraphArea(255,255,255,TRUE);
	 $Test->drawScale($DataSet->GetData(),$DataSet->GetDataDescription(),SCALE_NORMAL,150,150,150,TRUE,0,2);
	 $Test->drawGrid(4,TRUE,230,230,230,50);
	
	return $Test;
}

function graphRender($DataSet, $Test)
{
	$Test->drawLineGraph($DataSet->GetData(),$DataSet->GetDataDescription());  
	$Test->drawPlotGraph($DataSet->GetData(),$DataSet->GetDataDescription(),3,2,255,255,255); 
	
	// Finish the graph  
	$Test->setFontProperties("Fonts/tahoma.ttf",8);  
	$Test->drawLegend(45,35,$DataSet->GetDataDescription(),255,255,255);  
	$Test->setFontProperties("Fonts/tahoma.ttf",10);  
	$Test->drawTitle(60,22,"My pretty graph",50,50,50,585);  
	$Test->Render("Naked.png"); 
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
    
	<!--creating the graph -->
	<?php
	$DataSet = new pData;
	$Graph = new pChart(700,230);
	
	$DataSet = graphDefineData($sensorDataSets);
	$Graph = graphInitGraph($DataSet);
	graphRender($DataSet, $Test);
	
	
	?>
	
	<p><?php echo htmlPrint_r($sensorDataSets);?></p>
	<!--<?php 	phpinfo(); 	?>-->
</body>
</html>
