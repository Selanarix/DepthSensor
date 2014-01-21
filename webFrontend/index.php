<?php
include("helper/parseSensorData.php");
include("helper/dateHelper.php");

function printPressureSensorInfo($sensorData)
{
					echo '<li class="list-group-item">';
                    echo '<span class="badge">'.$sensorData["floorArea"] * end($sensorData["measurements"])["value"]." ".$sensorData["sensorUnit"].'&#179;</span>';
					echo 'Füllstand</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.getFormatedTimeDiff(end($sensorData["measurements"])["time"],time()).'</span>';
					echo 'Alter der Messung</li>';
					echo '<li class="list-group-item">';                    
                    echo '<span class="badge">'.$sensorData["floorArea"] * $sensorData["tankHeight"]." ".$sensorData["sensorUnit"].'&#179;</span>';
					echo 'Kapazität</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["tankHeight"]." ".$sensorData["sensorUnit"].'</span>';
					echo 'Tankhöhe</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["floorArea"]." ".$sensorData["sensorUnit"].'&#178;</span>';				
					echo 'Grundfläche</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["sensorType"].'</span>';
					echo 'Sensortyp</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["sensorUnit"].'</span>';
					echo 'Masseinheit</li>';
}

function printTemperaturSensorInfo($sensorData)
{
				echo '<li class="list-group-item">';
                    echo '<span class="badge">'.end($sensorData["measurements"])["value"]." ".$sensorData["sensorUnit"].'</span>';
					echo 'Temperatur</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.getFormatedTimeDiff(end($sensorData["measurements"])["time"],time()).'</span>';
					echo 'Alter der Messung</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["sensorType"].'</span>';
					echo 'Sensortyp</li>';
					echo '<li class="list-group-item">';
					echo '<span class="badge">'.$sensorData["sensorUnit"].'</span>';
					echo 'Masseinheit</li>';
}

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


    



    <!-- Marketing messaging and featurettes
    ================================================== -->
    <!-- Wrap the rest of the page in another container to center all the content. -->

    <div class="container marketing">

      <!-- columns of text for each Sensor -->
      <div class="row">
		<?php
				foreach($_SESSION['data'] as $sensorData)
				{
                    
					echo '<div class="col-lg-3">';
					echo '<h2>'.$sensorData["name"].'</h2>';
					echo '<p>'.$sensorData["description"].'</p>';
					echo '<ul class="list-group">';
					
                    if($sensorData["sensorType"] == "Pressure")
				        printPressureSensorInfo($sensorData);
					else if($sensorData["sensorType"] == "Temperature")
                        printTemperaturSensorInfo($sensorData);
                    else
                    {
    				    echo '<li class="list-group-item">';
					    echo '<span class="badge">'.$sensorData["sensorType"].'</span>';
					    echo 'Sensortyp</li>';
                    }
					echo '</ul>';
					echo '<p><a class="btn btn-default" href="debug.php" role="button">View details &raquo;</a></p>';
					
					echo '</div>';
				}
		?>
      </div><!-- /.row -->

      <?php include "pageData/include/footer.incl"; ?>

    </div><!-- /.container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
	<script src="javascript/jquery.js"></script>
    <script src="pageData/style/dist/js/bootstrap.min.js"></script>
  </body>
</html>
