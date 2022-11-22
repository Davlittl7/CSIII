//Davonte Littleton
//CSIII
//9 September 2022

#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
#include "WordList.hpp"

using std::cout; using std::endl;

int main(int argc, char *argv[]) {
	WordList wordSort;
	std::ifstream fileOpener;
	std::string wordCollector;
	fileOpener.open(argv[1]);
	if(argc == 2) {
		//Goes until the end of the file
		while (fileOpener >> wordCollector) {
			std::string refinedWord;
			//Removes any potential punctuation from the word
			for (int i = 0; i < wordCollector.size(); ++i) {
				char charCheck = wordCollector[i];
				if (isalnum(charCheck)) //Checks for alphanumeric in char
					refinedWord.append(sizeof(charCheck), charCheck);
			}
			//Adds word into obj wordSort
			wordSort.addWord(refinedWord);
		}
		//Prints all the words out in order by rarity
		wordSort.print();
	}
	else { //Error message if a file is not used with a.out as an argument
		cout << "Error! Please input a file along with your execution!";
	}
	fileOpener.close();
}

