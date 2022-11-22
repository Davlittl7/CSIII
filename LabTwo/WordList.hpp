#include<iostream>
#include<string>

#ifndef WORD_LIST_HPP_ //Include Guards
#define WORD_LIST_HPP_

using std::string;

class WordOccurrence {
public:
    WordOccurrence(const string& word = "", int num = 1): word_(word), num_(num) {};
    // < overloaded operator to allow sort to function with class
    bool operator<(const WordOccurrence& compObj);
    bool matchWord(const string&); // returns true if word matches stored
    void increment(); // increments number of occurrences

    //Accessor and Mutator Funcs for word and num
    string getWord() const; 
    void setWord(string);
    int getNum() const;
    void setNum(int);
private:
    string word_;
    int num_;
};

class WordList {
public:
    //WordList constructor
    WordList(int size = 0): size_(size), wordArray_(new WordOccurrence[size]) {};
    //Copy Constructor 
    WordList(const WordList& rhs) : size_(rhs.size_), wordArray_(new WordOccurrence[rhs.size_]) {
        for (int i = 0; i < size_; ++i) wordArray_[i] = rhs.wordArray_[i];
    } 
    ~WordList(); //Destructor
    WordList& operator=(WordList); //Overloaded Assignment 
    // implement comparison as friend
    friend bool equal(const WordList&, const WordList&); 
    friend void swap(WordList&, WordList&); //swap used for copy-and-swap
    void addWord(const string&); //Adds words into wordArray attribute
    void print(); //Prints out word and amt of occurrences
private:
    WordOccurrence* wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};

#endif
