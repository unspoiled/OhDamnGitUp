/*
 * Updates the display with the current time every second.
 *
 * Authors: Levi Faid Zac Gardner Duncan Cowan Caleb Mitchell
 */

#include "TimeDisplay.h"

TimeDisplay::TimeDisplay() {
	lcd = new LCD(0x3F);

	running = true;
	TimeDisplayThread = std::thread(&TimeDisplay::run, this);
}

void TimeDisplay::run() {
	struct tm ltm;
	bool colonVisible = true;
	while(running) {
		time_t now = time(0);
		localtime_r(&now, &ltm);

		int hour = ltm.tm_hour;
		int minute = ltm.tm_min;
		std::string time = std::string("      ") + 
				(hour < 10 ? std::string("0") : std::string("")) +
				std::to_string(hour) +
				(colonVisible ? std::string(":") : std::string(" ")) +
				(minute < 10 ? std::string("0") : std::string("")) + 
				std::to_string(minute) +
				std::string("      ");

		// Debugging
		//std::cout << time.c_str() << std::endl;

		lcd->clear();
		lcd->print(time.c_str());

		colonVisible = !colonVisible;

		usleep(1000000);
	}
}

void TimeDisplay::join() {
	if(TimeDisplayThread.joinable())
		TimeDisplayThread.join();
}
