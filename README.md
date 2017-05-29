# GitUp Alarm Clock

In a world saturated by devices with the "Jack of all trades, master of none" mentality, we strive to make a reliable alarm clock that has only one purpose: to be an alarm clock. With an elegant web interface, large LED display, and cutting edge, robust alarm algorithms, this is the last alarm clock you will ever need.




### Under The Hood

There are 3 main parts to our service; 
- The software that checks for alarms and makes noise
- The server that displays the infomation to the user when they are editing alarm times and tones
- The the hardware which includes a pi a speaker and a led display array that shows various infomation

The following is the story of the creation of each part.

#### Chapter one, The Software

In order to sound an alarm we have made a c++11 excutable that does very little of everything in the best possible way.
To start with the program starts a thread of our alarm checker, a function that spents most of it's life waiting between checks,
it waits for a while then calls a function that sees if there is an alarm in the next x seconds, then if there is checks if there
is one in the next x/2 seconds. This continues until the point in time in which very few seconds remain then it will call the
alarm sound function. I'm using the c++11 standard library thread inorder to use threads and using condition varibles for my
waiting. This means I can easily stop the program from waiting from where ever we want to which will be handy when we update this
program to use much longer wait times, so that we can have an interupt come from the server to tell the program the alarm data has
been updated. In the current iteration of the software the wait time is set to a minute so that the server updating is unlikely 
to be an issue unless you want to have an alarm go off in the same minute to make it. In testing we have found that while this wait
time is much shorter then I would ideally want it hasn't had any effects on performance, but still I want it to use less inorder to
stop the pi malfunctioning over time. And that's my story.
build commands
g++ -Wall -c -std=gnu++11 "%f"
g++ -Wall -std=gnu++11 -o "%e" "%f" -pthread

#### Chapter two, The Server

The web server is run through an apache2 and php5 services run on the pi. on first boot up, the clock will be in hot spot mode, where
the user can connect through another device and access a webpage using the current ip of the hotspot or the domain name used "gitup.local"
where a ssid and password can be supplied to the clock for internet access and accurate time keeping. upon doing this the clock reboots
and trys to connect to the internet, if it fails to connect with the supplied information it will switch back to the hotspot. if 
connection is successful it can be accessed on the connbected wifi point through the same domain name "gitup.local" note that this will
only work on devices that support bonjour mdns dection which includes apples products, other wise the needed ip address to connect to by
typing it into the url bar will display on the clocks screen. once the site has been accessed alarms can be set from here 
for weekly repeats one off alarms or daily alarms. alarms are currently set in 24 hour time.

#### Chapter three, The Hardware

Because our alarm clock is it's own standalone device, we had to develop both software and hardware for it. The hardware consists of a 
speaker for the alarm sounds to play through, a 24x8 led array to display the time (along with any other information), and a 
Raspberry Pi Zero W that controls everything. To enable audio playback through the speaker, GPIO pin 18 on the Raspberry Pi is set up
to output the audio as a PWM (Pulse Width Modulation) signal. This signal is then sent through a basic low-pass filter to filter out 
all the high frequencies (above about 20kH) that would otherwise make the audio sound terrible, and could possibly damage the speaker.
To further protect the speaker, a 10uF capacitor is put between the output of the low-pass filter and the speaker to stop any DC current
from passing through and potentially damaging the speaker. For the display, a grid of 24x8 leds were used. This 24x8 grid was then split 
into 4 sections, each a grid of 6x8 leds. Two 16-channel constant current led drivers were in conjunction with a multiplexing method to 
drive all 4 sections in parallel, where each constant current led driver was used to provide data to two sections at a time, with all
4 sections being multiplexed separately in parallel. Controlling the led drivers and the multiplexing is an ATMega328P with the 
Arduino bootloader running custom firmware. This firmware both controls the leds and processes commands sent from the Raspberry Pi 
via the I2C bus. The Raspberry Pi Zero W was chosen as the brains of our alarm clock because of its wireless capabilities, small 
form factor, ease of use for development, and the abundance of GPIO that could be used for hardware buttons or other add ons in the 
future.
