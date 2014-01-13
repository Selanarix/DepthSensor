<?php
session_start();

// Standard inclusions   
include("pChart/pData.class");
include("pChart/pChart.class");

if ( !isset($_SESSION['data']) )
	exit();

$sensorDataSets = $_SESSION['data'];

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
		$timestamps = array();
		$nameOfVal = "Serie".$i;
		$nameOfX = "XLabel".$i;
		
		foreach ($sensorSet["depthMeasurements"] as $set)
		{
			$DataSet->AddPoint($set["value"],$nameOfVal);
			array_push($timestamps, $set["time"]);
		}
		
		
		$j=0;
		for ($j=0; $j<sizeof($timestamps); $j++)
		{
			$timestamps[$j]=$timestamps[$j]-end($timestamps);			
		}
		
		$DataSet->AddPoint($timestamps,$nameOfX);
		$DataSet->SetAbsciseLabelSerie($nameOfX);
		$DataSet->AddAllSeries();
		$DataSet->RemoveSerie($nameOfX);   
		
		
		$DataSet->SetSerieName("Time".$i,$nameOfX);
		$DataSet->SetSerieName("Depth".$i,$nameOfVal);
		
		//Set Timestemp as X-Axis and Value to Y-Axis
		$DataSet->setXAxisName("Time");
		$DataSet->setXAxisUnit("s");
		
		$DataSet->setYAxisName("Depth");
		$DataSet->setYAxisUnit("cm");
		
        $i++;
	}
	
	return $DataSet;
}

function graphInitGraph($DataSet)
{
	// Initialise the graph
	 $Graph = new pChart(700,250);
	 $Graph->setFixedScale(0,50);
	 $Graph->setFontProperties("tahoma.ttf",8);
	 $Graph->setGraphArea(70,30,600,200);
	 $Graph->drawFilledRoundedRectangle(7,7,693,233,5,240,240,240);
	 $Graph->drawRoundedRectangle(5,5,695,235,5,230,230,230);
	 $Graph->drawGraphArea(255,255,255,TRUE);
	 $Graph->drawScale($DataSet->GetData(),$DataSet->GetDataDescription(),SCALE_NORMAL,150,150,150,TRUE,0,2);
	 $Graph->drawGrid(4,TRUE,230,230,230,50);
	
	// Draw the 0 line   
	$Graph->setFontProperties("tahoma.ttf",6);   
	$Graph->drawTreshold(0,143,55,72,TRUE,TRUE);   
	return $Graph;
}

function graphRender($DataSet, $Graph)
{
	// Draw the cubic curve graph  
	$Graph->drawFilledLineGraph($DataSet->GetData(),$DataSet->GetDataDescription(),50,TRUE);
	//$Graph->drawLineGraph($DataSet->GetData(),$DataSet->GetDataDescription());
	$Graph->drawPlotGraph($DataSet->GetData(),$DataSet->GetDataDescription(),3,2,255,255,255);
	// Finish the graph  
	$Graph->setFontProperties("tahoma.ttf",8);  
	//$Graph->drawLegend(55,45,$DataSet->GetDataDescription(),255,255,255);  
	$Graph->setFontProperties("tahoma.ttf",10);  
	$Graph->drawTitle(60,22,"Amount of water during time",50,50,50,585);  
	//$Graph->Render("Naked.png");
	$Graph->Stroke();
}
?>
