#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>

// 12 total chars
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

int getSeed(){
	std::ifstream myFile;
	myFile.open("values.dat");
	std::string value = "";
	getline(myFile,value);
	myFile.close();
	return stoi(value);
}

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
// ! hard coded interface
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
int main(){
	changeMacAddress();
	return 0;
}
