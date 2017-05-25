/*
 * alarmMain.c++
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
#include <fstream>
#include <string>

using namespace std;
condition_variable ConVar;

bool CheckAlarm(int delay){
	ifstream inxml ("/var/www/html/alarm.xml");
	string line;
	int i=0;
	char switcher;
	string repeat;
	string weekday;
	int tone;
	struct tm * alarmTime;
	time_t rawtime;
	time_t localTime;
	double adiff;
	time(&rawtime);
	alarmTime = localtime(&rawtime);
	localTime = mktime(localtime(&rawtime));
	mktime(alarmTime);
	if (inxml.is_open()){
		inxml.ignore(200,'>');
		inxml.ignore(200,'>');
		while( getline (inxml,line,'>')){			
			inxml.ignore(200,'>');
			getline(inxml,line,'<');			
			if(line!="</Alarms"){
				alarmTime->tm_hour = stoi(line.substr(0,2),nullptr, 10);
				alarmTime->tm_min = stoi(line.substr(2,2),nullptr, 10);
				alarmTime->tm_sec = 0;
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');				
				getline(inxml,line,'<');				
				repeat=line;
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				getline(inxml,line,'<');
				weekday=line;								
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');				
				getline(inxml,line,'<');
				tone=stoi(line,nullptr,10);
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				i++;	
				inxml.ignore(200,'>');
				switcher=repeat[0];
				switch(switcher){
					case 'd':		
							adiff=mktime(alarmTime)-localTime;
							cout<<adiff<<"\n";							
							if((0<adiff)&(adiff<delay)){
								return true;
						}
					break;
					case 'w':
						if(7>weekday.find(to_string(alarmTime->tm_wday))){
							adiff=mktime(alarmTime)-localTime;
								cout<<adiff<<"\n";
								
							if((0<adiff)&(adiff<delay)){
								return true;
							}
						}
					break;
					case 'n':
							adiff=mktime(alarmTime)-localTime;
							cout<<adiff<<"\n";								
							if((0<adiff)&(adiff<delay)){
								return true;
							}
					break;
					case 'o':
						return false;
					break;
					default:
						return false;
				}
			}
		}		
		inxml.close();
		
	}
	else{
		cout<< "fail";
	}
	cout<<tone;
	
	return false;
}

void alarm(){
	// This is a quick and dirty way to play an alarm.
	// NOTE: Make sure your sound is turned up enough.
	//       To stop the alarm, use "system("pkill loop_sound");"
	//       or just "pkill loop_sound" in the terminal.
	string sound = "sounds/beep.wav";
	string command = "scripts/loop_sound "+sound+" &";
    
	system(command.c_str());
	
	return;
}


void CheckerThread(){
	int delay = 3600;
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);
	cout<< "Hello World\n";
	while(true){
		if(CheckAlarm(delay)){
			if(delay<15){
				alarm();
				delay=3600;
				
			ConVar.wait_for(lockr, chrono::seconds(60));
			} else {
				delay=delay/2;
				cout<<"True tho\n";
			}
		} else {
			cout<<"false thou\n";
			ConVar.wait_for(lockr, chrono::seconds(delay));
		}
	}
	cout<<"Goodbye World\n";
	return;
}

int main(int argc, char **argv){
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);
	thread checkT(CheckerThread);
	while(true){
		ConVar.wait_for(lockr, chrono::seconds(40));
		//waitforhtml
		//makehtml
		//waitforupdate
		ConVar.notify_all();
	}
	checkT.join();
	cout<< "Hello From The Other Sides\n";
	return 0;
}
