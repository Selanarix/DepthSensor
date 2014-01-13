<?php
	session_start();

	// pChart inclusions   
	include("pChart/pData.class");
	include("pChart/pChart.class");

	if ( !isset($_SESSION['data']) )
		exit();
	$sensorDataSets = $_SESSION['data'];

	if ( !isset($_GET["sensor"]))
		exit();
	$sensorId = $_GET["sensor"];

	if ( !isset($sensorDataSets[$sensorId]) )
		exit();

	//Generate graph images for sensors
	$DataSet = new pData;
	$Graph = new pChart(700,230);
	
	
	$DataSet = graphDefineData($sensorDataSets[$sensorId]);
	$Graph = graphInitGraph($DataSet);
	graphRender($DataSet, $Graph, $sensorDataSets[$sensorId]["description"]);
	exit();
	
	function graphDefineData($sensorSet)
	{	
		$DataSet = new pData;
		
			$timestamps = array();
			$nameOfVal = "Serie";
			$nameOfX = "XLabel";
			
			foreach ($sensorSet["depthMeasurements"] as $set)
			{
				$DataSet->AddPoint($set["value"],$nameOfVal);
				array_push($timestamps, $set["time"]);
			}
			
			for ($counter=0; $counter<sizeof($timestamps); $counter++)
			{
				//$timestamps[$counter] = $timestamps[$counter] - end($timestamps);
				$timestamps[$counter] = $timestamps[$counter] - time();
			}
			
			$DataSet->AddPoint($timestamps,$nameOfX);
			$DataSet->SetAbsciseLabelSerie($nameOfX);
			$DataSet->AddAllSeries();
			$DataSet->RemoveSerie($nameOfX);   
			
			
			$DataSet->SetSerieName("Time",$nameOfX);
			$DataSet->SetSerieName("Depth",$nameOfVal);
			
			//Set Timestemp as X-Axis and Value to Y-Axis
			$DataSet->setXAxisName("Time");
			$DataSet->setXAxisUnit("s");
			
			$DataSet->setYAxisName("Depth");
			$DataSet->setYAxisUnit("cm");
			
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

	function graphRender($DataSet, $Graph, $statName)
	{
		// Draw the cubic curve graph  
		$Graph->drawFilledLineGraph($DataSet->GetData(),$DataSet->GetDataDescription(),50,TRUE);
		$Graph->drawPlotGraph($DataSet->GetData(),$DataSet->GetDataDescription(),3,2,255,255,255);
		
		// Finish the graph
		$Graph->setFontProperties("tahoma.ttf",8);  
		//$Graph->drawLegend(55,45,$DataSet->GetDataDescription(),255,255,255);  
		$Graph->setFontProperties("tahoma.ttf",10);  
		$Graph->drawTitle(60,22,$statName,50,50,50,585);  
		$Graph->Stroke();
	}
?>
