<?php
//$data = simplexml_load_file("photos001.xml");
$data = json_decode(file_get_contents("photos002.json"));
$photos = $data->photos->photo;

foreach($photos as $photo){
  echo $photo->url_z . "\n";
}

?>