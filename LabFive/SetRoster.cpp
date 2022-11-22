//Davonte Littleton
//CSIII
//30 September 2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include<set>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::set;


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_))
        //courses_(move(org.courses_))
    {}

    // force generation of default copy constructor
    Student(const Student& org) = default;

    string print() const { return firstName_ + ' ' + lastName_; }

    // needed for unique() and for remove()
    friend bool operator== (Student left, Student right) {
        return left.lastName_ == right.lastName_ &&
            left.firstName_ == right.firstName_;
    }

    // needed for sort()

    friend bool operator< (Student left, Student right) {
        return left.lastName_ < right.lastName_ ||
            (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
    }


private:
    string firstName_;
    string lastName_;
};

// reading a set from a fileName
void readRoster(set<Student>& roster, string fileName);
// printing a set out
void printRoster(const set<Student>& roster);

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }

    set<Student> courseStudents;

    //For loop that takes in all the values for each
    //file provided
    for (int i = 1; i < argc - 1; ++i) readRoster(courseStudents, argv[i]);

    // reading in dropouts
    set<Student> dropouts;
    readRoster(dropouts, argv[argc - 1]);


    //For loop to get rid of dropouts 
    for (auto dropoutCheck = dropouts.cbegin();
        dropoutCheck != dropouts.cend(); ++dropoutCheck) {
        //Searches for the dropouts within the set
        auto dropoutSearch = courseStudents.find(*dropoutCheck);
        //If it finds a name of a dropout, it erases it 
        if (dropoutSearch != courseStudents.end()) 
            courseStudents.erase(*dropoutCheck);
    }

    //Prints out set with only students who have one course
    cout << "Currently Enrolled Students" << endl;
    printRoster(courseStudents);

}


void readRoster(set<Student>& roster, string fileName) {
    //Variables initialize to set up student & course names
    ifstream course(fileName);
    string first, last, studentName;

    //While loop that collects all the student names
    while (course >> first >> last) {
        Student studentInsert(first, last);
        roster.insert(studentInsert);
    }
    //Closes the file
    course.close();
}

// printing a list out and to call on for the set
void printRoster(const set<Student>& roster) {
    //Prints out students and their courses until 
    //The function has gone through the entire set
    for (const auto& student : roster) cout << student.print() << endl;
}
