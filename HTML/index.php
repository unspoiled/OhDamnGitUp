<html>
	<head>
	<title>Assignment</title>
			<link rel="stylesheet" type="text/css" href="style.css">
			<script src="jquery-3.1.0.min.js"></script>
		
	</head>
	<body>
	<div id="header">
	<img src="logo.png"></img>
	</div>
	<div id="main">
	<?php
		include 'alarm.php';
	?>
	<form action="index2.php">
			<select name="val1" id="val1">
	  			<option value="01">01</option>
	  			<option value="02">02</option>
	  			<option value="03">03</option>
	  			<option value="04">04</option>
	  			<option value="05">05</option>
	  			<option value="06">06</option>
	  			<option value="07">07</option>
	  			<option value="08">08</option>
	  			<option value="09">09</option>
	  			<option value="10">10</option>
	  			<option value="11">11</option>
	  			<option value="12">12</option>
	  			<option value="13">13</option>
	  			<option value="14">14</option>
	  			<option value="15">15</option>
	  			<option value="16">16</option>
	  			<option value="17">17</option>
	  			<option value="18">18</option>
	  			<option value="19">19</option>
	  			<option value="20">20</option>
	  			<option value="21">21</option>
	  			<option value="22">22</option>
	  			<option value="23">23</option>
	  			<option value="00">00</option>
			</select>
			<select name="val2" id="val2">
				<option value="00">00</option>
				<option value="01">01</option>
	  			<option value="02">02</option>
	  			<option value="03">03</option>
	  			<option value="04">04</option>
	  			<option value="05">05</option>
	  			<option value="06">06</option>
	  			<option value="07">07</option>
	  			<option value="08">08</option>
	  			<option value="09">09</option>
	  			
	  			
			<?php
   				 for ($i=10; $i<60; $i++)
   				 {
       				 ?>
           			 <option value="<?php echo $i;?>"><?php echo $i;?></option>
       				 <?php
   				 }
			?>
			</select>
			<select id="val3" name="val3">
				<option value="Daily">Daily</option>
				<option value="weekly">weekly</option>
				<option value="no repeat">no repeat</option>
			</select>
			<select id="val4" name="val4">
				<option value="monday">Monday</option>
				<option value="tuesday">Tuesday</option>
				<option value="wendays">Wensday</option>
				<option value="thursday">Thursday</option>
				<option value="friday">Friday</option>
				<option value="saturday">Saturday</option>
				<option value="sunday">Sunday</option>				
			</select>
		<input type="submit" name="submit" value="Add"></input>
	</form>	
		<?php
			if(isset($_GET['submit1'])){
				$xml = new DOMDocument();
				$xml->load("alarm.xml");
				$edit = $xml->documentElement;
				$val1 = htmlentities($_GET['val1']);
				$root = $edit->getElementsByTagName('Alarm')->item(intval($val1));
				$edit->removeChild($root);
				$xml->save("alarm.xml");
				exec('./hello');
				header('Location: index.php');
			}
			
			else if( isset($_GET['submit'])){
				$xml = new DOMDocument();
				$xml->load("alarm.xml");
				$val1 = htmlentities($_GET['val1']);
				$val2 = htmlentities($_GET['val2']);
				$val3 = htmlentities($_GET['val3']);
				
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
				
				$title = $xml->createElement('Tone');
				$text = $xml->createTextNode("1");
				$title = $root->appendChild($title);
				$test = $title->appendChild($text);
				
					
				$title = $root->appendChild($title);
				$xml->save("alarm.xml");
			}
		?>
	</div>
	<div id="footer">
	</div>
	
	</body>
</html>
