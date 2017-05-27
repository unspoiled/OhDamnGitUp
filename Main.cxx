/*
 * 	Main.cxx
 *  Created on: 25Apr.,2017
 *  Authors: Levi Faid Zac Gardner Duncan Cowie Caleb Mitchell
 * 	
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



string CheckAlarm(int delay, bool del){
	ifstream inxml ("/var/www/html/alarm.xml");
	ofstream outxml;
	string line;
	char switcher;
	string repeat;
	string weekday;
	struct tm * alarmTime;
	time_t rawtime;
	time_t localTime;
	double adiff;
	bool timeCheck = false;
	string returnValue;
	streampos delLocation;
	
	time(&rawtime);
	alarmTime = localtime(&rawtime);
	localTime = mktime(localtime(&rawtime));
	mktime(alarmTime);
	if (inxml.is_open()){
		inxml.ignore(200,'>');	//ignore xml header
		inxml.ignore(200,'>');	//ignore alarms start
		while(inxml.good() && inxml.peek() != EOF){			
			getline(inxml,line,'>');//get alarm(s) header	
			inxml.ignore(200,'>');
			if(line.find("/Alarms")==line.npos){
				getline(inxml,line,'<');
				alarmTime->tm_hour = stoi(line.substr(0,2),nullptr, 10);
				alarmTime->tm_min = stoi(line.substr(2,2),nullptr, 10);
				alarmTime->tm_sec = 0;
				adiff=mktime(alarmTime)-localTime;
				if((0<adiff)&(adiff<delay)){
								timeCheck = true;
				}				
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');				
				if(timeCheck&&del){
					delLocation = inxml.tellg();
					cout<<delLocation;
				}
				getline(inxml,line,'<');				
				switcher=line[0];		//repeat type	
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				getline(inxml,line,'<');
				weekday=line;			//days on				
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');				
				getline(inxml,line,'<');
				returnValue=line;		//tone
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				
				if(!del){
				switch(switcher){
					case 'd':	
							if(timeCheck){
							return returnValue+='d';			
						}			
					break;
					
					case 'w':						
						if(weekday.npos!=weekday.find(to_string(alarmTime->tm_wday))){
							if(timeCheck){
								return returnValue+'w';
							}
						}
					break;
					
					case 'n':											
							if(timeCheck){
								return returnValue+'n';
							}
					break;
				}
			}
			}
		}		
		inxml.close();		
	}
	else{
		cout<< "XML error: alarm.xml not found\n";
	}
	if(del){
		inxml.open("/var/www/html/alarm.xml");
		outxml.open("/var/www/html/alarm");
		if (outxml.is_open()&&inxml.is_open()){
			while(inxml.good() && inxml.peek() != EOF){
				if(inxml.tellg()==delLocation){
					outxml.put('o');
					inxml.ignore(1,'n');
				}else{
					outxml.put(inxml.get());
				}
			}
			outxml.close();		
			inxml.close();
		inxml.open("/var/www/html/alarm");
		outxml.open("/var/www/html/alarm.xml");
		if (outxml.is_open()&&inxml.is_open()){
			while(inxml.good() && inxml.peek() != EOF){
					outxml.put(inxml.get());
				}
			}else{
				cout<< "XML error:something went wrong, sorry\n";			
			}
		}else{
			cout<< "XML error: alarm.xml not found\n";
		}
	}
	
	
	return "0";
}

void alarm(string reptone){
	// This is a quick and dirty way to play an alarm.
	// NOTE: Make sure your sound is turned up enough.
	//       To stop the alarm, use "system("pkill loop_sound");"
	//       or just "pkill loop_sound" in the terminal.
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);					
	string sound = "sounds/sound"+reptone.substr(0,1)+".wav";
	cout<<sound<<"\n";
	string command = "scripts/loop_sound "+sound+" &";
	system(command.c_str());	
	if(reptone.find('n')!=reptone.npos){
		cout<<"ok\n\n";
		CheckAlarm(15, true);
	}
	ConVar.wait_for(lockr, chrono::seconds(60));
	system("pkill loop_sound");
	return;
}

void CheckerThread(){
	int delay = 60;
	mutex mtx;
	string reptone;
	unique_lock<std::mutex> lockr(mtx);
	while(true){
		reptone=CheckAlarm(delay, false);
		cout<<reptone;
		if("0"!=reptone){
			if(delay<15){
				alarm(reptone);
				delay=60;				
			} else {
				delay=delay/2;
			}
		} else {
			ConVar.wait_for(lockr, chrono::seconds(delay));
		}
	}
	return;
}

int main(int argc, char **argv){
	mutex mtx;
	unique_lock<std::mutex> lockr(mtx);
	thread checkT(CheckerThread);
	while(true){
		ConVar.wait_for(lockr, chrono::seconds(40));
		//get port to listen on for interupt from the server client
		//ConVar.notify_all();
	}
	checkT.join();
	return 0;
}
