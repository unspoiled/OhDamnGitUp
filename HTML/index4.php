<?php

if( isset($_POST['submit'])){
	unset($_POST['submit']);
	$ssid = htmlentities($_POST['val1']);
	$pwd = htmlentities($_POST['val2']);
	$info = "wpa-ssid '".$ssid."'\nwpa-psk '".$pwd."'";
	echo($info);
	if(file_exists('/etc/network/interfaces2')){
		exec("mv /etc/network/interfaces /etc/network/interfaces3");
		exec("mv /etc/network/interfaces2 /etc/network/interfaces");
	}
	exec("printf 'ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\nupdate_config=1\n\nnetwork={\n\tssid=\"". $ssid ."\"\n\tpsk=\"" . $pwd . "\"\n\tkey_mgmt=WPA-PSK\n}\n' > /etc/wpa_supplicant/wpa_supplicant.conf");
	
	sleep(3);
	exec("sudo reboot"); 	
	echo exec("echo damnson");	
	}


?>
