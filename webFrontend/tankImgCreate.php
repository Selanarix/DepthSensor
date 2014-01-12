<?php
generateTankImage(200, 50, 200, 10, 15, "testTankImg1.png");

function generateTankImage($tonW, $tonE, $tonH, $prim, $percent, $filename)
{
    //Create our basic image stream 
    //125px width, 125px height
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
    imagettftext($image, 30, 0, $prim + $tonW/2 - 35, $prim + $tonH/2+30, $red, $font, (100-$percent)."%");

    //save the image as a png and output 
    imagepng($image, $filename);
     
    //Clear up memory used
    imagedestroy($image);
}
?>

<html>
<head>
    <title>Test img generation</title>
</head>
<body>
    <img src="testTankImg1.png" />
</body>
</html>
