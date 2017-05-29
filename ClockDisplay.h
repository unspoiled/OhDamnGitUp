/*
 * ClockDisplay.h
 *
 * Author: Duncan Cowan
 */

#ifndef CLOCKDISPLAY_H_
#define CLOCKDISPLAY_H_

extern "C" {
	#include "display/display.h"
	#include <unistd.h>
}
#include <thread>
#include <atomic>
#include <ctime>
#include <iostream>

class ClockDisplay {
	public:
		ClockDisplay();
		void stop();
		void join();
	private:
		std::thread clockDisplayThread;
		std::atomic_bool running;
		void run();
};

#endif /* CLOCKDISPLAY_H_ */
