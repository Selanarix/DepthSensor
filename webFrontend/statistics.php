<?php
include("helper/parseSensorData.php");
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
    <link href="pageData/style/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="pageData/style/custom.css" rel="stylesheet">
  </head>
<!-- NAVBAR
================================================== -->
  <body>
	<?php include "pageData/include/naviBar.incl"; ?>

    <!-- Carousel
    ================================================== -->
    <div id="myCarousel" class="carousel slide" data-ride="carousel">
      <!-- Indicators -->
      <!--<ol class="carousel-indicators">
        <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
        <li data-target="#myCarousel" data-slide-to="1"></li>
        <li data-target="#myCarousel" data-slide-to="2"></li>
      </ol>-->
      <div class="carousel-inner">
        <div class="item active">
          <img data-src="holder.js/900x500/auto/#777:#7a7a7a/text:First slide">
          <div class="container">
            <div class="carousel-caption">
			  <h1>Level Statistics</h1>
              <p>This Project gets data of <?php echo count($_SESSION['data'])?> sensor(s) in your watertanks. On this page you find statistics about chronological sequence of water level.</p>
            </div>
          </div>
        </div>
      </div>
    </div><!-- /.carousel -->



    <!-- Marketing messaging and featurettes
    ================================================== -->
    <!-- Wrap the rest of the page in another container to center all the content. -->

    <div class="container marketing">

      <!-- two columns of text below the carousel -->
      
			<?php
				foreach($_SESSION['data'] as $sensorData)
				{
					echo '<div class="row">';
					echo '<img class="autoreloadGraph" id="'.$sensorData["id"].'" src="dynPicProcessing/renderGraph.php?sensor='.$sensorData["id"].'" />'."\r\n";
					echo '</div> <!--close row -->';

				}
			?>
	
		<?php include "pageData/include/footer.incl"; ?>

    </div><!-- /.container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://code.jquery.com/jquery-1.10.2.min.js"></script>
    <script src="pageData/style/dist/js/bootstrap.min.js"></script>
  </body>
</html>
