/*
 * alarmMain.cc

 *
 *  Created on: 25Apr.,2017
 *      Author: Levi
 */

#include <cstdlib>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>


std::condition_variable ConVar;


void CheckerThread(){
	int delay = 3600;
	std::mutex mtx;
	std::unique_lock<std::mutex> lockr(mtx);
	while(true){
		if(CheckAlarm(delay)){
			if(delay<60){
				//alarm()
				delay=3600
			}else{
				delay=delay/2;
			}
		}else{
			ConVar.wait_for(lockr,std::chrono::seconds(delay))
		}
	}
	return;
}

void alarm(){
	return;
}

bool CheckAlarm(int delay){


	return false;
}


int main(){
	std::thread checkT(CheckerThread)
	while(true){
		//waitforhtml
		//makehtml
		//waitforupdate
		lak.notify_all();
	}
	checkT.join();
	return 0
}
