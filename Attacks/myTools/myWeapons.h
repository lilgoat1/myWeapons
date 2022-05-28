#ifndef MYWEAPONS_H_EXISTS
#define MYWEAPONS_H_EXISTS
#include <string>
#include <tuple>
#include <vector>

const char* cString(std::string value);

template<class INPUT, class OUTPUT>
OUTPUT convert(INPUT value);

std::string getConsoleOutput(const char* command);
std::string parseIpAddress(std::string response);
std::tuple<std::string,std::string> getIpValues();
void runSpoof();
std::string createEther();
int getSeed();
void writeSeed(int value);
void changeMacAddress();
std::string grabEx(std::string expression, std::string constraint);
void disconnect(std::string interface, std::vector<std::string> targets);
void sleep(int seconds);
void writeToCap(std::string fileName, std::vector<std::string> content);
std::vector<std::string> readFromCap(std::string fileName);
#include "myWeapons.cpp"
#endif
