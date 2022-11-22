#include<iostream>
#include<string>
#include<algorithm>
#include "WordList.hpp"


//Compares two words to see if they are the same
bool WordOccurrence::matchWord(const string& compWord)
{
	
	if (word_ == compWord)
		return true;
	else
		return false;

	//return word_ == compWord ? true : false;
}



//Method implemented to help with sorting words by rarity
bool WordOccurrence::operator< (const WordOccurrence& compObj) {
	return compObj.getNum() > this->getNum();
}



//Increments the num of the word's appearance
void WordOccurrence::increment()
{
	++num_;
}

//Accessor Func that returns word (mainly for output in print method)
string WordOccurrence::getWord() const {
	return word_;
}

//Mutator func that sets a newWord for addWord func
void WordOccurrence::setWord(string x) {
	word_ = x;
}

//Accessor func that returns num_ (mainly for output in print method)
int WordOccurrence::getNum() const {
	return num_;
}

//Sets num of wordOccurrence within array (used for addWord func)
void WordOccurrence::setNum(int x) {
	num_ = x;
}



//Overloaded Assignment Operator
WordList& WordList::operator=(WordList rhs) {
	swap(*this, rhs);
	return *this;
}


//Destructor
WordList::~WordList() {
	delete[] wordArray_;
}

//Method that adds a new Word into wordOccurrence array
void WordList::addWord(const string& newWord) {
	
	//Looks to see if the word is already in the dynamic array
	for (int i = 0; i < size_; ++i) {
		if (wordArray_[i].matchWord(newWord)) {
			wordArray_[i].increment();
			return;
		}
	}

	//Sets up increased size and tmp dynamic array
	int newSize = size_ + 1;
	WordOccurrence* tmp = new WordOccurrence[newSize];

	//Copies all the old words into a new dynamic tmp WordOccurrence arr
	for (int i = 0; i < size_; ++i) {
		tmp[i] = wordArray_[i];
		tmp[i].setNum(wordArray_[i].getNum());
	}

	//Sets last element of arr to the newWord, increments, and then deallocates
	size_ = newSize;
	tmp[newSize - 1].setWord(newWord);
	delete[] wordArray_;

	//Set wordArray_ equal to the new array tmp
	wordArray_ = tmp;

	
}

//Outputs word and the amt of times it has appeared
void WordList::print() {
	//Sorts the words in wordArray_ by their rarity
	std::sort(wordArray_, wordArray_ + size_); 
	
	//Prints out all the words and the amount of that word
	for (int i = 0; i < size_; ++i) {
		std::cout << wordArray_[i].getWord() << " " << wordArray_[i].getNum() << std::endl;
	}


}

//Compares two WordList objects
bool equal(const WordList& first, const WordList& second)
{
	//Compares sizes first as if these aren't equal, then the WordLists aren't
	if (first.size_ != second.size_) {
		return false;
	}
	else {
		for (int i = 0; i < first.size_; ++i) {
			//Compares to see if the words they have are the same
			if (first.wordArray_[i].getWord() != second.wordArray_[i].getWord()) 
				return false;
			//Compares to see if they have the same count for each of the words
			if (first.wordArray_[i].getNum() != second.wordArray_[i].getNum()) 
				return false;
		}
	}
	return true;
	
}

//Friend swap function used for copy-and-swap with overloaded assignment
void swap(WordList& first, WordList& second) {
	//Swaps the two attribute values with each other
	std::swap(first.size_, second.size_);
	std::swap(first.wordArray_, second.wordArray_);
}

