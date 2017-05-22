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
	string repeat;
	string weekday;
	int tone;
	struct tm * alarmTime;
	time_t rawtime;
	struct tm * currentTime;
	struct tm * delayTime;
	time (&rawtime);
	currentTime = localtime(&rawtime);
	delayTime = localtime(&rawtime);
	mktime(currentTime);
	if(delay==3600){
		delayTime->tm_hour++;
	}else{
		delayTime->tm_min+=delay/60;
		delayTime->tm_sec+=delay%60;
	}
	mktime(delayTime);
	
	if (inxml.is_open()){
		inxml.ignore(200,'>');
		inxml.ignore(200,'>');
		while( getline (inxml,line,'>')){
			inxml.ignore(200,'>');
			getline(inxml,line,'<');
			if(line!="</Alarms"){
				
				alarmTime->tm_hour = stoi(line.substr(0,1),nullptr, 10);
				alarmTime->tm_min = stoi(line.substr(2,3),nullptr, 10);
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
				
				switch(repeat[0]){
					case d;
						if(difftime( mktime(alarmTime), mktime(currentTime))>difftime(mktime(difftime), mktime(currentTime))){
							return true;
						}
					break;
					case w:
						if(weekday.find(to_string(currentTime->tm_wday))){
							if(difftime( mktime(alarmTime), mktime(currentTime))>difftime(mktime(difftime), mktime(currentTime))){
								return true;
						}
						}
					break;
					case n;
						if(difftime( mktime(alarmTime), mktime(currentTime))>difftime(mktime(difftime), mktime(currentTime))){
							return true;
						}
					break;
					case o:
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

	
	return false;
}

void alarm(){
	
	return;
}


void CheckerThread(){
	int delay = 3600;
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);
	cout<< "Hello World\n";
	//while(true){
		if(CheckAlarm(delay)){
			if(delay<60){
				alarm();
				delay=3600;
			} else {
				delay=delay/2;
			}
		}else{
			ConVar.wait_for(lockr, chrono::seconds(10));
		}
	//}
	cout<<"Goodbye World\n";
	return;
}

int main(int argc, char **argv){
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);
	thread checkT(CheckerThread);
	//while(true){
		ConVar.wait_for(lockr, chrono::seconds(5));
		//waitforhtml
		//makehtml
		//waitforupdate
		ConVar.notify_all();
	//}
	checkT.join();
	cout<< "Hello From The Other Sides\n";
	return 0;
}
