var frquencyDevider = 0;

$.fn.exists = function () {
    return this.length !== 0;
}

setInterval(
    function()
    {
        if(frquencyDevider >= 5 || $('.autoreloadGraph').length > 0)
		{
            $.get("helper/parseSensorData.php?pathToFolder=../sensorData/",function() 
            {
		        $('.autoreloadGraph').each (
                    function(index) 
                    {
                      $(this).attr("src", "dynPicProcessing/renderGraph.php?sensor="+$(this).attr('id')+"&"+new Date().getTime());
                    }
                );

                if(frquencyDevider >= 5)
                {
                    $('.autoreloadTank').each (
                    function(index) 
                    {
                      $(this).attr("src", "dynPicProcessing/tankImgCreate.php?sensor="+$(this).attr('id')+"&"+new Date().getTime());
                    });                
                    frquencyDevider = 0;
                }
            });
        }
        frquencyDevider ++;
     }
, 2000);
