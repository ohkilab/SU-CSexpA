<?php

$files = glob("*.jpg");

foreach($files as $file){
  echo "<img src=\"$file\" width=\"100\">\n";
}

?>