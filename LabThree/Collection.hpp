//Davonte Littleton
//CSIII
//16 September 2022

#include<iostream>
#include<cstdlib>
#include "List.hpp"

#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

//Forward Class Declaration
template<typename T>
class Collection; 

//Forward Function Declaration
template<typename T> 
bool equal(const Collection<T>&, const Collection<T>&);

template<typename T>
class Collection {
public:
	Collection(): head(nullptr) {}
	void add(const T&);
	void remove(const T&);
	void print();
	T last() { return head->getData(); }; //returns last value inserted

	//Friend fucnction to compare two collections/lists
	friend bool equal<T>(const Collection&, const Collection&);
private:
	node<T>* head;

};

//Func to add an item into collection
template<typename T>
void Collection<T>::add(const T& newItem) {
	//Sets a new tmp node
	node<T>* tmp = new node<T>;

	//Uses tmp to implement the new item and have head's value be after
	tmp->setData(newItem);  
	tmp->setNext(head);

	//Sets original head equal to the tmp node
	head = tmp;


}

//Func to remove all instances of an item in collection
template<typename T>
void Collection<T>::remove(const T& removeVal) {
	if (head == nullptr) return; //returns if list is empty
	
	node<T>* removeStart = head;
	//removes any values equivalent to removeVal from the top of the list
	while (head != nullptr && head->getData() == removeVal) {
		head = head->getNext();
		delete removeStart;
		removeStart = head;
	}

	
	//Nodes for traversing through list
	node<T>* tmp = head; node<T>* revTmp = nullptr;

	//While loop that goes through entire list until
	while (tmp != nullptr) {
		//Sets a node to the next val of tmp
		node<T>* nxtTmp = tmp->getNext();
		//If statement to check if current tmp data is equal to removeVal
		if (tmp->getData() == removeVal) {
			if (revTmp != nullptr) revTmp->setNext(nxtTmp);
			delete tmp; //removes value from list
		}
		if (tmp != nullptr) revTmp = tmp;
		tmp = nxtTmp;
	}

	delete tmp;
}
    
//Func to print all values in collection
template<typename T>
void Collection<T>::print() {
	node<T>* tmp = head;

	//Checks to see if collection and outputs empty if so
	if (head == nullptr) std::cout << "The list is empty!" << std::endl;

	//Outputs all the values currently in the linked list
	while (tmp != nullptr) {
		//Retrieves value and then moves to next item in list
		std::cout << tmp->getData() << std::endl;

		//Then moves on to the next val until tmp == nullptr
		tmp = tmp->getNext();
	}

	delete tmp;
}


//Func comparing two collections for equality
template <typename T>
bool equal(const Collection<T>& a, const Collection<T>& b) {
	//Two tmp values to go through each collection
	node<T>* aTmp = a.head;
	node<T>* bTmp = b.head;

	//While loop that goes until one of the lists are finished 
	while (aTmp != nullptr && bTmp != nullptr) {
		if (aTmp->getData() != bTmp->getData()) {
			return false;
		}
		//Moves aTmp & bTmp forward to look to the next val in list
		aTmp = aTmp->getNext();
		bTmp = bTmp->getNext();
	}

	//If both collections/lists get past while loop, equal is true
	delete aTmp;
	delete bTmp;
	return true; 
}


#endif 

