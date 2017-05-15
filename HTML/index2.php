<?php
if(isset($_GET['submit1'])){

	$xml = new DOMDocument();
	$xml->load("alarm.xml");
	$edit = $xml->documentElement;
	$val1 = htmlentities($_GET['val1']);
	$root = $edit->getElementsByTagName('Alarm')->item(intval($val1));
	$edit->removeChild($root);
	$xml->save("alarm.xml");	
	exec("./hello");
	sleep(1);
	header('location: index.php');
}
else if( isset($_GET['submit'])){
				$xml = new DOMDocument();
				$xml->load("alarm.xml");
				$val1 = htmlentities($_GET['val1']);
				$val2 = htmlentities($_GET['val2']);
				$val3 = htmlentities($_GET['val3']);
				$val4 = htmlentities($_GET['val4']);
				
				$root = $xml->createElement('Alarm');
				$root = $xml->getElementsByTagName('Alarms')[0]->appendChild($root);
				
				$title = $xml->createElement('Time');
				$text = $xml->createTextNode("$val1$val2");
				$title = $root->appendChild($title);
				$test = $title->appendChild($text);
				
				$title = $xml->createElement('Repeat');
				$text = $xml->createTextNode($val3);
				$title = $root->appendChild($title);
				$test = $title->appendChild($text);
				
				$title = $xml->createElement('weekday');
				$text = $xml->createTextNode($val4);
				$title = $root->appendChild($title);
				$test = $title->appendChild($text);
				
				$title = $xml->createElement('Tone');
				$text = $xml->createTextNode("1");
				$title = $root->appendChild($title);
				$test = $title->appendChild($text);
				
					
				$title = $root->appendChild($title);
				$xml->save("alarm.xml");

				exec("./hello");
				sleep(1);
				header('location: index.php');
			}


?>
