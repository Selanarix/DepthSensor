setInterval(
    function()
    {
		$.get("helper/parseSensorData.php?pathToFolder=../sensorData/");
		$('.autoreloadImg').each (
            function(index) 
            {
              $(this).attr("src", "dynPicProcessing/renderGraph.php?sensor="+$(this).attr('id')+"&"+new Date().getTime());
            }
        );
       
     }
, 2000);
