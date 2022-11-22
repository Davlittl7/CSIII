//Davonte Littleton
//CSIII
//28 October 2022

#include "hashmap.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;

// template tests hashmap inserts
template <typename Key, typename Value>
void test_insert(hashmap<Key, Value>& hm, Key key, Value value) {
    cout << "Inserting - " << key << ": " << value << " - ";
    pair<const pair<const Key, Value>*, bool> result = hm.insert(make_pair(key, value));

    if (result.second)
        cout << "It has been inserted!" << endl;
    else
        cout << "Can't insert, the key: " << result.first->first
        << " already exists w/ value " << result.first->second << endl;
}

// template tests hashmap erases
template <typename Key, typename Value>
void test_erase(hashmap<Key, Value>& hm, Key key) {
    cout << "Erasing the key: " << key << "..." << endl;
    pair<const pair<const Key, Value>*, bool> result = hm.erase(key);

    if (result.second) {
        cout << "Done! ";
        if (result.first)
            cout << "The next element is - " << result.first->first
            << ": " << result.first->second << endl;
        else cout << "There is no next element" << endl;
    }
    else 
        cout << "It failed! A key value at " << key << " doesn't exist!" << endl;
}


int main() {
    //Declaring a hashmap container of students
    hashmap<int, string> students;

    //Testing Insert 
    //Filling in the student hashmap container 
    cout << "Testing insert function!" << endl;
    test_insert<int, string>(students, 123, "Davonte");
    test_insert<int, string>(students, 456, "Devionne");
    test_insert<int, string>(students, 789, "Khaylyn");

    //Testing to see if it will allow for duplicates
    test_insert<int, string>(students, 123, "Billy");
    cout << endl;

    //Testing overloaded [] (indexing) operator
    cout << "Testing overloaded indexing operator!" << endl;
    cout << "Inserting new students 901 & 867 using []!" << endl;
    students[901] = "Sage";
    students[867] = "Andie";

    //Seeing if these students have been properly inserted
    auto a = students.find(901);
    auto b = students.find(867);

    if (a != nullptr) cout << a->first << " maps to: " << a->second << endl;
    else cout << "No student matches the key specifed for a" << endl;

    if (b != nullptr) cout << b->first << " maps to: " << b->second << endl;
    else cout << "No student matches the key specified for b" << endl;

    //Testing if they access properly key
    cout << "Seeing what is stored in 123 w/ indexing operator" << endl;
    cout << "123 maps to " << students[123] << endl;
    cout << endl;

    //Testing the erase function
    cout << "Testing erase function!" << endl;
    int eraseKey;
    cout << "Enter a student key to expel (will say it doesn't exist if no key found): ";
    cin >> eraseKey;

    //Erase student inputted if it is there and point to next student
    //Will say if a student w/ that key does not exist
    test_erase(students, eraseKey);

    //Uses an insert to double check if the student w/ eraseKey was erased
    cout << "Admitting new student and using key specified above" << endl;
    cout << "This won't work if key already exists " << endl;

    //If insert is successful, then student was successfully deleted 
    //Or eraseKey value wasn't there in the 1st place
    //If insert is unsuccessful, then delete is not working properly
    test_insert<int, string>(students, eraseKey, "Ashley");
    cout << endl;

    //Testing the rehash function
    cout << "Testing rehash function!" << endl;

    //Shouldn't change the size of the student hashmap
    students.rehash(50);
    cout << "Trying to decrease the size of students" << endl;
    cout << "Students current size (should be 101): " << students.getNumBuckets() << endl;
    cout << endl;

    //Should change the size of the student hashmap
    students.rehash(300);
    cout << "Increasing the size of students" << endl;
    cout << "Students current size (should be 300): " << students.getNumBuckets() << endl;
    cout << endl;

    //Testing to see if the students are at the same spots 
    cout << 123 << ": " << students[123] << endl;
    cout << 456 << ": " << students[456] << endl;
    cout << 789 << ": " << students[789] << endl;
    cout << 901 << ": " << students[901] << endl;
    cout << 867 << ": " << students[867] << endl;

    //Only outputs key tied to eraseKey if it doesn't match other keys
    if (eraseKey != 123 && eraseKey != 456 && eraseKey != 789
        && eraseKey != 901 && eraseKey != 867)
        cout << eraseKey << ": " << students[eraseKey] << endl;
    
}
