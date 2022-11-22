//Davonte Littleton
//CSIII
//30 September 2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::map;


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_))
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




// reading a map from a fileName
void readRoster(map<Student, list<string>>& roster, string fileName);
// printing a map out
void printRoster(const map<Student, list<string>>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }

    //map of courses of students
    map<Student, list<string>> courseStudents;


    for (int i = 1; i < argc - 1; ++i) readRoster(courseStudents, argv[i]);
    

    //Prints all of the students and what courses they took
    cout << "All students" << endl;
    cout << "First name, Last Name, and Courses Enrolled" << endl;

    //Prints everything that was taken in by readRoster
    printRoster(courseStudents);
    cout << endl;

    //Initializing a map for dropouts and a new map 
    //For when the dropouts are removed
    map<Student, list<string>> dropouts;
    map<Student, list<string>> sortedCourseStudents;

    //Takes in all the names from the dropouts
    readRoster(dropouts, argv[argc - 1]);

    //Nested for loop designed to remove dropouts 
    for (auto e = courseStudents.cbegin(); e != courseStudents.cend(); ++e) {
        bool matchCheck = false;
        for (auto dropoutCheck = dropouts.cbegin(); dropoutCheck != dropouts.cend();
            ++dropoutCheck) //Checks for dropouts in courseStudents
            if (dropoutCheck->first.print() == e->first.print()) matchCheck = true;
        if (!matchCheck) sortedCourseStudents.insert(std::make_pair(e->first, e->second));
    }
    
    //After the sorting and removal of dropouts
    cout << "All students sorted w/ all dropouts removed" << endl;
    printRoster(sortedCourseStudents);
   
}


void readRoster(map<Student, list<string>>& roster, string fileName) {
    //Variables to read file, take in names and the course name
    ifstream course(fileName);
    string first, last, nameOfCourse = fileName;
    size_t txtFinder = nameOfCourse.find('.');
    nameOfCourse.erase(txtFinder);
    
    //While loop that continues until there are no more names
    while (course >> first >> last) {
        //Student obj to help see if that person is already in the map
        Student rosterFinder(first, last);

        //searches for the student's name
        auto studentSearch = roster.find(rosterFinder);

        //If-else that pushes back just a course if student is already there
        //Or pushes back the course & student obj if they are not there
        if(studentSearch != roster.end()) {
            studentSearch->second.push_back(nameOfCourse);
        }
        else {
            list<string> newCourse({ nameOfCourse });
            roster.insert(std::make_pair(rosterFinder, newCourse));
        }
    }
    
    //Closes the file
    course.close();
}

// prints out the contents of the map
void printRoster(const map<Student, list<string>>& roster) {
    //for loop that runs until it goes thru the entire map
    for (auto rosterPrint = roster.cbegin();
        rosterPrint != roster.cend(); ++rosterPrint) {
        cout << (rosterPrint->first).print() << ": "; //prints student's name
        //for loop that prints out all courses for current student
        for (auto coursePrint : rosterPrint->second) cout << coursePrint << " ";
        cout << endl;
    }
}
