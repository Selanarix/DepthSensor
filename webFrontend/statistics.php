<?php
include("helper/parseSensorData.php");

$type = "";
if(isset($_GET["type"]) )
	$type = $_GET["type"];
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="Depth Seonsor Project">
    <meta name="author" content="Maximilian Hempe und Jonathan Schubert">
	
	<script src="javascript/jquery.js"></script>
	<script src="javascript/imgAutoReload.js" type="text/javascript"></script>

	
    <title>Depth Sensor Project Page</title>

    <!-- Bootstrap core CSS -->
    <link href="pageData/style/dist/css/bootstrap.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="pageData/style/custom.css" rel="stylesheet">
  </head>
<!-- NAVBAR
================================================== -->
  <body>
	<?php include "pageData/include/naviBar.incl"; ?>


    <div class="container">

      
			<?php
				foreach($_SESSION['data'] as $sensorData)
				{
					if($sensorData["sensorType"] == $type || empty($type) )
					{
						echo '<div class="statistic">';
						echo '<img class="img-responsive autoreloadGraph" id="'.$sensorData["id"].'" src="dynPicProcessing/renderGraph.php?sensor='.$sensorData["id"].'" />'."\r\n";
						echo '</div> <!--close statistic -->';
					}
				}
			?>
	
		<?php include "pageData/include/footer.incl"; ?>

    </div><!-- /.container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="javascript/jquery.js"></script>
    <script src="pageData/style/dist/js/bootstrap.min.js"></script>
  </body>
</html>
