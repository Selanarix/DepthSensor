setInterval(
    function()
    {
        console.log("log");
        $('#autoreloadImg').each (
            function(index) 
            {
                console.log("sensor id"+$(this).attr('id'));
                alert("b");
              //  $(this).attr("src", "dynPicProcessing/renderGraph.php?sensor="+$(this).attr('id')+"&"+new Date().getTime());
            }
        );
     }
, 2000);
