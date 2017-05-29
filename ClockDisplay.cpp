/*
 * Updates the display with the current time every second.
 *
 *  Author: Duncan Cowan
 */

#include "ClockDisplay.h"

ClockDisplay::ClockDisplay() {
	running = true;
	clockDisplayThread = std::thread(&ClockDisplay::run, this);
}

void ClockDisplay::run() {
	if(display_init() < 0)
		std::cerr << display_error() << std::endl;

	struct tm ltm;
	bool colonVisible = true;
	while(running) {
		time_t now = time(0);
		localtime_r(&now, &ltm);

		int hour = ltm.tm_hour;
		int minute = ltm.tm_min;
		std::string time = (hour < 10 ? "0" : "") + std::to_string(hour) +
				(colonVisible ? ":" : " ") +
				(minute < 10 ? "0" : "") + std::to_string(minute);

		// Debugging
		//std::cout << time.c_str() << std::endl;

		if(display_string(time.c_str(), time.length(), SCROLL_NONE, 0, 0) < 0)
			std::cerr << display_error() << std::endl;

		colonVisible = !colonVisible;

		usleep(1000000);
	}
}

void ClockDisplay::stop() {
	running = false;
	display_close();
}

void ClockDisplay::join() {
	if(clockDisplayThread.joinable())
		clockDisplayThread.join();
}
