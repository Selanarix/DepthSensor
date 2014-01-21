<?php
session_start();

if ( !isset($_SESSION['data']) )
	exit();
$sensorDataSets = $_SESSION['data'];

if ( !isset($_GET["sensor"]))
    exit();
$sensorId = $_GET["sensor"];

if ( !isset($sensorDataSets[$sensorId]))
    exit();

generateTankImage(120, 40, 120, 10, $sensorDataSets[$sensorId]);


//Finish programm completely
exit();

function generateTankImage($tonW, $tonE, $tonH, $prim, $sensorDataSet)
{
    $percent = 0;
    //Test if sensor has an height. Otherwise can not calculate percentage
    if($sensorDataSet["tankHeight"] < 1)
        return false;
    else
    {
        $newestSensorValue = $sensorDataSet["measurements"][count($sensorDataSet["measurements"])-1];
        $percent = $newestSensorValue['value']*100.0/$sensorDataSet["tankHeight"] ;
    }    
    //Solve overflow error
    if ($percent > 100)
        $percent = 100;
    
    //Create our basic image stream 
    $image = imagecreate($tonW+2*$prim, $tonE+$tonH+2*$prim);
     
    // Transparent Background
    imagealphablending($image, false);
    $transparency = imagecolorallocatealpha($image, 0, 0, 0, 127);
    imagefill($image, 0, 0, $transparency);
    imagesavealpha($image, true);

    //Set the background color
    $blue  = imagecolorallocate($image, 0, 0, 255);
    $black = imagecolorallocate($image, 0, 0, 0);
    $red = imagecolorallocate($image, 255, 0, 0);
    $font = 'arial.ttf';

    $percent = 100-$percent;

    if($percent < 100)
    {
        imagefilledellipse($image, $prim+$tonW/2, $prim+$tonE/2 + $tonH, $tonW, $tonE, $blue);
        imagefilledrectangle($image, $prim, ($prim+$tonE/2)+($percent/100)*$tonH, $prim+$tonW, $prim+$tonH+$tonE/2, $blue);
        imagefilledellipse($image, $prim+$tonW/2, ($prim+$tonE/2)+($percent/100)*$tonH, $tonW, $tonE, $blue);
    }

    imageellipse($image, $prim+$tonW/2, ($prim+$tonE/2)+($percent/100)*$tonH, $tonW, $tonE, $black);
    imageellipse($image, $prim+$tonW/2, $prim+$tonE/2 + $tonH, $tonW, $tonE, $black);
    imageellipse($image, $prim+$tonW/2, $prim+$tonE/2 , $tonW, $tonE, $black);
    imageline   ($image, $prim , $prim+$tonH+$tonE/2, $prim, $prim+$tonE/2, $black);
    imageline   ($image, $prim+$tonW, $prim+$tonH+$tonE/2, $prim+$tonW, $prim+$tonE/2, $black);
    imagettftext($image, 25, 0, $prim + $tonW/2 - 20, $prim + $tonH/2+30, $red, $font, (100-$percent)."%");

    header('Content-Type: image/png');
    //save the image as a png and output 
    imagepng($image);
     
    //Clear up memory used
    imagedestroy($image);
}
?>
