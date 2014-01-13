
<?php
include("helper/parseSensorData.php");
echo'<?xml version="1.0" encoding="utf-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN"
    "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
  <script src="javascript/jquery.js"></script>
  <script src="javascript/imgAutoReload.js" type="text/javascript"></script>
  <title>Cister fill level monitor</title>
</head>
<body>
    <h1>Cister fill level monitor</h1>
    <p>There are <?php echo count($_SESSION['data'])?> sensors under monitoring</p>
    <?php
        foreach($_SESSION['data'] as $sensorData)
            echo '<img class="autoreloadImg" id="'.$sensorData["id"].'" src="dynPicProcessing/renderGraph.php?sensor='.$sensorData["id"].'" />'."\r\n";
    ?>
</body>
</html>
