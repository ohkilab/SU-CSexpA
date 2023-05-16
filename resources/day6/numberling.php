<?php

$files = glob("*.jpg");

$i = 0;
foreach($files as $file){
  $to_file = sprintf("%03d.jpg", $i);
  exec("mv $file $to_file");
  $i++;
}

?>
