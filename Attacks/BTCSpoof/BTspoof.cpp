#include "/root/Desktop/Attacks/myTools/myWeapons.h"
#include <iostream>
#include <map>
#include <vector>
#include <regex>

/*
	Should act as a dos for wifi & implement spoofing using btrcap 
	users can only select one option
	optionally allow to target both client & router
	
*/
int generatePrompt(){
	int result = 0;
	bool choosing = true;
	while (choosing){
		std::cout << "SELECT: \n" <<
				"1) Disconnect \n" <<
				"2) SPOOF\n" << 
				": ";
		std::string response = "";
		getline(std::cin,response);
		std::string value = grabEx(response,"\\d");
		result = convert<std::string,int>(value);
		if ((result == 1) || (result == 2)){
			choosing = false;
		} else {
			std::cout << "Invalid response\n";
		}
	}
	return result;
}
void deDos(){
	std::vector<std::string> allTargets;
	bool aquiringValues = true;
	std::string interface = "eth0";
	std::string container = "";
	int numberOfTargets = 0;
	std::cout << "Amount of Targets:	";
	getline(std::cin,container);
	numberOfTargets = convert<std::string,int>(container);
	std::cout << "Enter all targets: \n";
	
	while (aquiringValues){
		if (allTargets.size() == numberOfTargets){
			aquiringValues = false;
		} else {
	    		container = "";
	    		std::cout << "[+]: ";
	    		getline(std::cin,container);
	    		allTargets.push_back(container);
		}
	} 
	disconnect("eth0",allTargets);
}
int main(){
	deDos();
	//std::string interface = "eth0";
	//std::string initialization = "bettercap -iface " + interface;
	//std::string probeActivation = "net.probe on";
	//system(cString(initialization));
	//Slee
	//system(cString(probeActivation));
	return 0;
}
