
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
	ifstream inxml ("/var/www/html/alarm.xml");
	ofstream outphp ("/var/www/html/alarm.php");
	string line;
	int i=0;
	if (inxml.is_open() && outphp.is_open()){
		inxml.ignore(200,'>');
		inxml.ignore(200,'>');

		while( getline (inxml,line,'>')){
			inxml.ignore(200,'>');
			getline(inxml,line,'<');
			if(line!="</Alarms"){
				outphp << "<form action='index2.php'>\n ";
				outphp << "Time:" << line;
				cout <<"tes: "<<line;
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				getline(inxml,line,'<');
				outphp << " Repeat: " << line;
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				getline(inxml,line,'<');
				outphp << " weekday: " << line;
				outphp << " tone: 1 \n";
				outphp << "<input type='submit' name='submit1' id='submit1' value='delete'></input>\n";
				outphp << "<input type='hidden' name='val1' id='val1' value='"<<i<<"'></input>\n";
				outphp << "</form>\n";
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				inxml.ignore(200,'>');
				i++;	
				inxml.ignore(200,'>');
			}
		}		
		inxml.close();
		outphp.close();
		
	}
	else{
		cout<< "fail";
	}

}
