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


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_)),
        courses_(move(org.courses_))
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
    
    //Method to push newly added courses into the courses attribute
    void pushCourse(string newCourse) { courses_.push_back(newCourse); }

    //Method for printing students with all of their courses
    void printStudents() const {
        cout << (*this).print() << ": ";
        for (auto coursesItr = courses_.begin(); 
            coursesItr != courses_.end(); ++coursesItr) {
            cout << *coursesItr << " ";
        }
        cout << endl;
    }
private:
    list<string> courses_;
    string firstName_;
    string lastName_;
};




// reading a list from a fileName
void readRoster(list<Student>& roster, string fileName);
// printing a list out
void printRoster(const list<Student>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }

    //List of students that hold student names and their courses
    list<Student> courseStudents;

    //For loop that takes in all the values for each
    //file provided
    for (int i = 1; i < argc - 1; ++i) readRoster(courseStudents, argv[i]);

    
    //Prints all of the students and what courses they took
    cout << "All students" << endl;
    cout << "First name, Last Name, and Courses Enrolled" << endl;

    //Prints all of the students
    printRoster(courseStudents);

    cout << endl;


    // reading in dropouts
    list<Student> dropouts;
    readRoster(dropouts, argv[argc - 1]);

    list<Student> sortedCourseStudents;  // master list of students

    //For loop to look through dropouts, and only add
    //Non-dropouts to the sortedCourseStudents
    for (const auto& courseCheck : courseStudents) {
        bool matchCheck = false;
        for (const auto& dropoutCheck : dropouts) {
            //Checks for if there are any dropouts in the list
            if (dropoutCheck.print() == courseCheck.print()) matchCheck = true;
        }
        //Adds all of the non-dropout students
        if (!matchCheck) sortedCourseStudents.push_back(courseCheck);
    }
    
    //Sorts the new list of students
    sortedCourseStudents.sort();
    
    //After the sorting and removal of dropouts
    cout << "All students sorted w/ all dropouts removed" << endl;
    printRoster(sortedCourseStudents);
}


void readRoster(list<Student>& roster, string fileName) {
    //Variables initialize to set up student & course names
    ifstream course(fileName);
    string first, last, nameOfCourse = fileName;
    size_t txtFinder = nameOfCourse.find('.');
    nameOfCourse.erase(txtFinder);

    bool checkCourses; 
    
    //While loop that collects all the student names
    while (course >> first >> last) {
        string studentName = first + ' ' + last;
        checkCourses = false;

        for (auto& rosterCheck : roster) { //Runs through entire list
            //If the student is already in the list it just pushes back course
            if (rosterCheck.print() == studentName) {
                rosterCheck.pushCourse(nameOfCourse);
                checkCourses = true;
            }
        }
        //If they aren't in the list yet this adds them 
        //and their first course
        if (!checkCourses) {
            Student tmp(first, last);
            tmp.pushCourse(nameOfCourse);
            roster.push_back(move(tmp));
        }
    }
    //Closes the file
    course.close();
}

// printing a list out and to call on for the list
void printRoster(const list<Student>& roster) {
    //Prints out students and their courses until 
    //The function has gone through the entire list
    for (const auto& student : roster) student.printStudents();
}
