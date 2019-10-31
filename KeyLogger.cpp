#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iterator>
#include<regex>
#include <cstring>
#include <linux/input.h>
using namespace std;

string getEventKeyBoard(){
	fstream DeviceData;
	string temp,HandlerTag="H: Handlers=",EVTag="B: EV=",KeyBrdEVTag="EV=120013",event;
	int HandlerTagLen,EVTagLen,KeyBrdEVTagLen,iterator;
	vector<string> HandlerEVData;
	DeviceData.open("/proc/bus/input/devices",ios::in);
	HandlerTagLen=HandlerTag.length();
	EVTagLen=EVTag.length();
	KeyBrdEVTagLen=KeyBrdEVTag.length();
	if(DeviceData){
		do
		{
			getline(DeviceData,temp);
			if(temp.length() > HandlerTagLen){
				if(temp.substr(0,HandlerTagLen).compare(0,HandlerTagLen,HandlerTag)==0)
					HandlerEVData.push_back(temp.substr(HandlerTagLen,temp.length()));
			}
			else{
				if(temp.substr(0,EVTagLen).compare(0,EVTagLen,EVTag)==0)
					HandlerEVData.push_back(temp.substr(3,temp.length()));
			}
		}while(!DeviceData.eof());

		for(iterator=0;iterator<HandlerEVData.size();iterator++){
			if(HandlerEVData[iterator].length()>=KeyBrdEVTagLen){
				if(HandlerEVData[iterator].substr(0,KeyBrdEVTagLen).compare(0,KeyBrdEVTagLen,KeyBrdEVTag)==0){
					temp = HandlerEVData[iterator-1];
					break;
				}
			}
		}

		regex r("event[0-9]");
		smatch m;
		regex_search(temp,m,r);
		event = m[0];
	}
	else{
		cout << "File Cannot be Opened" << endl;
		event="";
	}
	HandlerEVData.clear();
	DeviceData.close();
	return event;
}

void captureKeyBoardData(string path){
	fstream keyStrokeData;
	input_event keyData;
	int iterator,sizeOfEvent=sizeof(keyData);
	char tempData[sizeOfEvent];
	keyStrokeData.open(path,ios::in);
	if(keyStrokeData){
		keyStrokeData.read(tempData,sizeOfEvent);
		memcpy(&keyData,tempData,sizeOfEvent);
		cout << keyData.code << endl;
	}
	else
		cout << "Could not open KeyBoard Data" << endl;

}

int main(){
	string filePath,eventNum;
	eventNum = getEventKeyBoard();
	if(eventNum.length()>0){
		filePath = "/dev/input/" + eventNum;
		captureKeyBoardData(filePath);
	}
	else
		cout << "Could not fetch keyboard event!!" << endl;
}