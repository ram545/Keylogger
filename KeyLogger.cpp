#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iterator>
#include<regex>
using namespace std;

int main(){
	fstream DeviceData;
	string temp,HandlerTag="H: Handlers=",EVTag="B: EV=",KeyBrdEVTag="EV=120013",event,filePath;
	int HandlerTagLen,EVTagLen,KeyBrdEVTagLen,i;
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

		for(i=0;i<HandlerEVData.size();i++){
			if(HandlerEVData[i].length()>=KeyBrdEVTagLen){
				if(HandlerEVData[i].substr(0,KeyBrdEVTagLen).compare(0,KeyBrdEVTagLen,KeyBrdEVTag)==0){
					temp = HandlerEVData[i-1];
					//cout << temp << endl;
					break;
				}
			}
		}

		regex r("event[0-9]");
		smatch m;
		regex_search(temp,m,r);

		for(auto x:m){
			event=x;
		}
		
		filePath = "dev/input/"+event;
		cout << filePath << endl;
	}
	else{
		cout << "File Cannot be Opened" << endl;
	}
	HandlerEVData.clear();
	DeviceData.close();
}