//Davonte Littleton
//CSIII
//28 October 2022

#include<iostream>
#include<fstream>
#include<string>

using std::string; using std::endl;

class Logger {
public: 
	//Static instance of logger class
	static Logger& instance() {
		static Logger l;
		return l;
	}
	void report(const string& newStr) {
		newLine.open("log.txt", std::fstream::out | std::fstream::app);
		newLine << newStr << endl;
		newLine.close();
	}

	Logger(const Logger&) = delete; //Copy constructor
	Logger& operator=(const Logger&) = delete; //Assignment operator
private: 
	std::ofstream newLine;
	Logger() {}; //Default constructor
	
};
