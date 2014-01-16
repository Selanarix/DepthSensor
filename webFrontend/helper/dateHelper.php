<?php
function diff_time($differenz)
{  
   $woche  = floor($differenz / (7*3600*24));
   $tag  = floor($differenz / (3600*24));
   $std  = floor($differenz / 3600 % 24);
   $min  = floor($differenz / 60 % 60);
   $sek  = floor($differenz % 60);

   return array("sek"=>$sek,"min"=>$min,"hour"=>$std,"day"=>$tag,"week"=>$woche);
}

function getFormatedTimeDiff($old,$new)
{
    $res =  diff_time($new-$old);
    if($res["week"] > 0)
        return $res["week"]." week/s";
    else if($res["day"] > 0)
        return $res["day"]." day/s";    
    else if($res["hour"] > 0)
        return $res["hour"]." hour/s";    
    else if($res["min"] > 0)
        return $res["min"]." min/s";
    else
        return $res["sec"]." sec/s";
    
}
 
?>
