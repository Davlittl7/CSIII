//Davonte Littleton
//CSIII
//28 October 2022

#include<iostream>
#include<string>
#include<fstream>
#include "Logger.hpp"

using std::cout; using std::endl;

void testLogger();

int main() {
	string input = "Hello, World!";
	Logger::instance().report(input);
	Logger::instance().report("Sup");


	std::ifstream fout;
	string receiveLine;
	fout.open("log.txt");

	testLogger();
	while (!fout.eof()) {
		getline(fout, receiveLine);
		cout << receiveLine << endl;
	};

	fout.close();
}

void testLogger() {
	Logger::instance().report("Hello");
}
