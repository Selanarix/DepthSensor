
<?php 
include("parseSensorData.php");
echo'<?xml version="1.0" encoding="utf-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN"
    "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
	<script src="jquery.js" ></script>
    <script>
        setInterval(
            function()
            {
                $.get("parseSensorData.php");
				$("#sensor0").attr("src", "renderGraph.php?sensor=0&"+new Date().getTime());
            },2000);
    </script>

 <title>Cister fill level monitor</title>
</head>
<body>
    <h1>Cister fill level monitor</h1>
    <p>There are <?php echo count($_SESSION['data'])?> sensors under monitoring</p>
    
	<img id="sensor0" src="renderGraph.php?sensor=0" />
	<p><?php echo htmlPrint_r($sensorDataSets);?></p>
	<!--<?php 	phpinfo(); 	?>-->
</body>
</html>
