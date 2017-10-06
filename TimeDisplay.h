/*
 * TimeDisplay.h
 *
 * Authors: Levi Faid Zac Gardner Duncan Cowan Caleb Mitchell
 */

#ifndef TIMEDISPLAY_H_
#define TIMEDISPLAY_H_

extern "C" {
	#include <unistd.h>
}
#include <thread>
#include <atomic>
#include <ctime>
#include <iostream>
#include "display/lcd.h"

class TimeDisplay {
	public:
		TimeDisplay();
		void join();
	private:
		LCD *lcd;
		std::thread TimeDisplayThread;
		std::atomic_bool running;
		void run();
};

#endif /* TIMEDISPLAY_H_ */
