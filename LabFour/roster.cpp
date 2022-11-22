//Davonte Littleton
//CSIII
//23 September 2022


#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;

// reading a list from a fileName
void readRoster(list<list<string>>& roster, string fileName);

// printing a list out
void printRoster(const list<list<string>>& roster);

int main(int argc, char* argv[]) {

    //checks for correct input of arguments
    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last"
            << endl; exit(1);
    }

    // vector of courses of students
    list <list<string>> courseStudents;

    for (int i = 1; i < argc - 1; ++i) readRoster(courseStudents, argv[i]);

    cout << "All students" << endl;
    cout << "First name, Last Name, and Courses Enrolled" << endl;
    printRoster(courseStudents);
    


    //Nested list to store a sorted list of students w/ dropouts removed
    list<list<string>> sortedCourseStudents;

    //Nested list to take in all of the dropouts
    list<list<string>> dropouts;
    readRoster(dropouts, argv[argc - 1]);

    //Nested for loop that pushed back any lists that don't match
    //the names within the dropout nested list
    for (const auto& courseRoster: courseStudents) {
        bool matchCheck = false;
        for (const auto& dropoutCheck : dropouts) {
            if (courseRoster.front() == dropoutCheck.front()) 
                matchCheck = true; //If true, indicates student is dropout
        }
        //If matchCheck is true, then it won't add the dropouts
        if (!matchCheck) sortedCourseStudents.push_back(courseRoster);
    }

    //Sorts the new student container with dropouts removed.
    sortedCourseStudents.sort();

    //After the sorting and removal of dropouts
    //Outputs the new list of students
    cout << "All students sorted w/ all dropouts removed" << endl;
    printRoster(sortedCourseStudents);
  
}

// reading in a file of names into a list of strings
void readRoster(list<list<string>>& roster, string fileName) {
    list<string> x; //list of strings to push back into roster
   
    ifstream course(fileName); //opens up the file to work with

    //Initializing strings to push back into list of string and 
    //setting up the courseName and erasing the .txt
    string first, last, nameOfCourse = fileName;
    size_t txtFinder = nameOfCourse.find('.');
    nameOfCourse.erase(txtFinder);
    
    //While loop to collect names
    while (course >> first >> last) {
        //Sets up student name to push back
        string studentName = first + ' ' + last + ":";
        
        //for loop to go through each list that is nested inside
        bool checkCourses = false;
        for (auto& listCheck : roster) {
            //Checks to see if the student's name is already in the list
            if (listCheck.front() == studentName && !checkCourses) {
                listCheck.push_back(nameOfCourse);
                checkCourses = true;
            }
        }
        //If statement that sets up a new student if they are not 
        //in the list already.
        if (!checkCourses) {
            x.push_back(move(studentName));
            x.push_back(nameOfCourse);
            roster.push_back(move(x));
        }
    }
    //closes the file
    course.close();
}

// printing a list out
void printRoster(const list<list<string>>& roster) {
    //Sets up an iterator to go through the nested list
    
    //Nested for loop to get inside both lists to the string variable
    for (auto rosterItr = roster.begin(); 
        rosterItr != roster.end(); ++rosterItr) {
        for (const auto& rosterPrint : *rosterItr) {
            //outputs the string values in each list
            cout << rosterPrint << " ";
        }
        //Endl to separate the lists from each other
        cout << endl;
    }

    cout << endl;

}