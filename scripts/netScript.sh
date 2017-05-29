#!/bin/sh -e

echo "run"
echo ! ping -q -c5 google.com >null
if [ ! -f /etc/network/interfaces3 ]
then
	IPTABLES=/sbin/iptables

	# Create internet chain
	# This is used to authenticate users who have already signed up
	#$IPTABLES -N internet -t mangle
	
	# First send all traffic via newly created internet chain
	# At the prerouting NAT stage this will DNAT them to the local
	# webserver for them to signup if they aren't authorised
	# Packets for unauthorised users are marked for dropping later
	#$IPTABLES -t mangle -A PREROUTING -j internet
	
	#$IPTABLES -t mangle -A internet -j MARK --set-mark 99
	
	# Redirects web requests from Unauthorised users to internet webserver
	#$IPTABLES -t nat -A PREROUTING -m mark --mark 99 -p tcp --dport 80 -j DNAT --to-destination 192.168.42.1
	
	# Now that we've got to the forward filter, drop all packets
	# marked 99 - these are unknown users. We can't drop them earlier
	# as there's no filter table
	#$IPTABLES -t filter -A FORWARD -m mark --mark 99 -j DROP
	
	# Do the same for the INPUT chain to stop people accessing the web through Squid
	#$IPTABLES -t filter -A INPUT -p tcp --dport 80 -j ACCEPT
	#$IPTABLES -t filter -A INPUT -p udp --dport 53 -j ACCEPT
	#$IPTABLES -t filter -A INPUT -m mark --mark 99 -j DROP
	#/sbin/iptables -t nat -A PREROUTING -p tcp --dport 80 -j DNAT --to-destination 192.168.42.1
	echo "no"
else
	sleep 40
	echo "yesy"
	if  ! ping -q -c5 google.com >null 
	then
		echo "umm"
		mv /etc/network/interfaces /etc/network/interfaces2
		mv /etc/network/interfaces3 /etc/network/interfaces
		if [ ! -f /var/www/html/index3.php ]
		then
			echo "wha"
			mv /var/www/html/index.php /var/www/html/index3.php
			mv /var/www/html/index5.php /var/www/html/index.php
		fi
		sudo reboot
	else
		if [ ! -f /var/www/html/index5.php ]
		then
		echo "yups"
		mv /var/www/html/index.php /var/www/html/index5.php
		mv /var/www/html/index3.php /var/www/html/index.php
		fi	
fi


fi
exit 0;
