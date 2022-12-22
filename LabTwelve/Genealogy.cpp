//Davonte Littleton
//CSIII
//2 December 2022

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl; using std::cin;
using std::string;
using std::vector;

class Person { // component
public:
    Person(string firstName, Person* spouse, Person* father, Person* mother) :
        firstName_(firstName), spouse_(spouse), father_(father), mother_(mother) {}
    const string& getFirstName() { return firstName_; }
    Person* getSpouse() { return spouse_; }
    void setSpouse(Person* spouse) { spouse_ = spouse; }
    Person* getFather() { return father_; }
    Person* getMother() { return mother_; }

    virtual void accept(class PersonVisitor*) = 0;
    virtual string getWholeName() = 0;
    virtual ~Person() {}
private:
    const string firstName_;
    Person* spouse_;
    Person* father_;
    Person* mother_;
};


// leaf
// man has a last name 
class Man : public Person {
public:
    Man(string lastName, string firstName, Person* spouse,
        Person* father, Person* mother) :
        lastName_(lastName),
        Person(firstName, spouse, father, mother) {}
    const string& getLastName() const { return lastName_; }
    void accept(class PersonVisitor* visitor) override;
    string getWholeName() override { return getFirstName() + " " + getLastName(); }
private:
    const string lastName_;
};

// composite
// woman has a list of children
class Woman : public Person {
public:
    Woman(vector<Person*> children,
        string firstName,
        Person* spouse,
        Person* father, Person* mother) :
        children_(children),
        Person(firstName, spouse, father, mother) {}
    const vector<Person*>& getChildren() const { return children_; }
    void setChildren(const vector<Person*>& children) { children_ = children; }
    void accept(class PersonVisitor* visitor) override;
    string getWholeName() override { 
        if (getSpouse() != nullptr)
            return getFirstName() + " " + static_cast<Man*>(getSpouse())->getLastName();
        else if (getFather() != nullptr)
            return getFirstName() + " " + static_cast<Man*> (getFather())->getLastName();
        else
            return getFirstName() + " " + "Doe";
    }
private:
    vector<Person*> children_;
};

// abstract visitor
class PersonVisitor {
public:
    virtual void visit(Man*) = 0;
    virtual void visit(Woman*) = 0;
    virtual ~PersonVisitor() {}
};

// composite methods that define traversal
void Man::accept(PersonVisitor* visitor) {
    visitor->visit(this);
}

void Woman::accept(PersonVisitor* visitor) {
    // children traversal through mother only
    // father's children are not traversed so as not 
    // to traverse them twice: for mother and father

    visitor->visit(this);

    // traversing descendants
    for (auto child : children_)
        child->accept(visitor);
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter : public PersonVisitor {
public:
    void visit(Man* m) override {
        cout << m->getFirstName() << " " << m->getLastName() << endl;
    }
    void visit(Woman* w) override {
        cout << w->getFirstName() << " ";
        if (w->getSpouse() != nullptr)
            cout << static_cast<Man*>(w->getSpouse())->getLastName();
        else if (w->getFather() != nullptr)
            cout << static_cast<Man*> (w->getFather())->getLastName();
        else
            cout << "Doe";
        cout << endl;
    }
};


class ChildrenPrinter : public PersonVisitor {
public:
    void visit(Man* m) override {
        cout << m->getFirstName() << ": ";
        Woman* spouse = static_cast<Woman*>(m->getSpouse());
        if (spouse != nullptr)
            printNames(spouse->getChildren());
        cout << endl;
    }
    void visit(Woman* w) override {
        cout << w->getFirstName() << ": ";
        printNames(w->getChildren());
        cout << endl;
    }
private:
    void printNames(const vector<Person*>& children) {
        for (const auto c : children)
            cout << c->getFirstName() << ", ";
    }
};

//Concrete vistor for whether two people are close relatives or just friends 
class FriendOrRelative : public PersonVisitor {
public: 
    //Constructor that takes in two string values from main
    //and starts the status as false.
    FriendOrRelative(string firstCandidate, string secondCandidate) :
        firstCandidate_(firstCandidate), secondCandidate_(secondCandidate), 
        checkStatus_(false) {}

    //Two visit functions defined below
    void visit(Man* m) override;
    void visit(Woman* w) override;

    //Returns two statements depending on if checkStatus_ is true or false.
    string returnStatus();
private:
    //Names for candidates entered
    string firstCandidate_;
    string secondCandidate_;
  
    //false if friends, true if relatives
    bool checkStatus_;
};

int main() {

    // setting up the genealogical tree      
    // the tree is as follows
    //    
    //
    //       James Smith  <--spouse-->   Mary 
    //	                                  |
    //	                                 children -------------------------
    //	                                  |              |                |
    //	                                  |              |                |
    //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
    //	                                  |
    //	                                 children------------
    //	                                  |                 |
    //                                     |                 |
    //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
    //	       |
    //	     children
    //	       |
    //          |
    //	     Susan


    // first generation
    Man* js = new Man("Smith", "James", nullptr, nullptr, nullptr);
    Woman* ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
    ms->setSpouse(js); js->setSpouse(ms);

    // second generation
    Woman* ps = new Woman({}, "Patricia", nullptr, js, ms);
    Man* wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
    ps->setSpouse(wj); wj->setSpouse(ps);

    vector<Person*> marysKids = { ps,
                       new Man("Smith", "Robert", nullptr, js, ms),
                       new Woman({}, "Linda", nullptr, js, ms) };
    ms->setChildren(marysKids);

    // third generation
    Man* mj = new Man("Johnson", "Michael", nullptr, wj, ps);
    vector<Person*> patsKids = { mj, new Woman({}, "Barbara", nullptr, wj, ps) };
    ps->setChildren(patsKids);

    Woman* jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
    vector<Person*> jensKids = { new Woman({}, "Susan", nullptr, mj ,jj) };

    jj->setSpouse(mj); mj->setSpouse(jj);
    jj->setChildren(jensKids);


    // defining two visitors
    ChildrenPrinter* cp = new ChildrenPrinter;
    NamePrinter* np = new NamePrinter;

    // executing the traversal with the composite
    // and the specific visitor

    
    cout << "\nNAME LIST\n";
    ms->accept(np);
    cout << endl << endl;

    
    cout << "CHILDREN LIST\n";
    ms->accept(cp);
    

    //Takes in the two names inputted
    string firstName, secondName;
    cout << "Enter first candidate: "; std::getline(cin, firstName);
    cout << "Enter second candidate: "; std::getline(cin, secondName);

    //Initializes friend or relative visitor and then calls 
    //accept with ms/Mary Smith
    FriendOrRelative* fp = new FriendOrRelative(firstName, secondName);
    ms->accept(fp);
    cout << fp->returnStatus() << endl;

    //Deallocates fp once finished
    delete fp;
}


//Visit funcs to see if candidates are related or just friends.
void FriendOrRelative::visit(Man* m) {
    //Makes sure grandparents aren't counted as relatives
    if (m->getWholeName() == firstCandidate_ && m->getMother() != nullptr &&
        m->getMother()->getMother() != nullptr
        && m->getMother()->getMother()->getWholeName() == secondCandidate_) {
        checkStatus_ = false; return;
    }
    if (m->getWholeName() == firstCandidate_ && m->getMother()  != nullptr &&
        m->getMother()->getFather() != nullptr
        && m->getMother()->getFather()->getWholeName() == secondCandidate_) {
        checkStatus_ = false; return;
    }

    if (m->getWholeName() == secondCandidate_ && m->getMother() != nullptr &&
        m->getMother()->getMother() != nullptr
        && m->getMother()->getMother()->getWholeName() == firstCandidate_) {
        checkStatus_ = false; return;
    }
    if (m->getWholeName() == secondCandidate_ && m->getMother() != nullptr &&
        m->getMother()->getFather() != nullptr
        && m->getMother()->getFather()->getWholeName() == firstCandidate_) {
        checkStatus_ = false; return;
    }

    //If-else-if that checks for siblings
    if (m->getWholeName() == firstCandidate_ &&
        m->getMother() != nullptr) {
        for (auto childItr : static_cast<Woman*>(m->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (m->getWholeName() == secondCandidate_ &&
        m->getMother() != nullptr) {
        for (auto childItr : static_cast<Woman*>(m->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //if-else-if that checks for aunts, uncles and parents for first candidate
    if (m->getWholeName() == firstCandidate_  
        && m->getMother() && m->getMother()->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(m->getMother()->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (m->getWholeName() == firstCandidate_ && m->getFather() != nullptr 
        && m->getFather()->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(m->getFather()->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //If-else-if that checks for aunts, uncles, and parents for 2nd candidate
    if (m->getWholeName() == secondCandidate_
        && m->getMother() && m->getMother()->getMother() != nullptr) {
        for (auto childItr :
            static_cast<Woman*>(m->getMother()->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (m->getWholeName() == secondCandidate_ && m->getFather() != nullptr
        && m->getFather()->getMother() != nullptr) {
        for (auto childItr :
            static_cast<Woman*>(m->getFather()->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //Ensures that the father is counted as a relative
    if ((m->getWholeName() == firstCandidate_ || 
        m->getWholeName() == secondCandidate_) && m->getSpouse() != nullptr) {
        checkStatus_ = true; return;
    }
    if ((m->getWholeName() == firstCandidate_ && m->getFather() != nullptr 
        && m->getFather()->getWholeName() == secondCandidate_)
        || (m->getWholeName() == secondCandidate_ && m->getFather() != nullptr 
            && m->getFather()->getWholeName() == firstCandidate_)) {
        checkStatus_ = true; return;
    }

    
}

void FriendOrRelative::visit(Woman* w){

    //Makes sure grandparents aren't counted as relatives
    if (w->getWholeName() == firstCandidate_ && w->getMother() != nullptr &&
        w->getMother()->getMother() != nullptr
        && w->getMother()->getMother()->getWholeName() == secondCandidate_) {
        checkStatus_ = false; return;
    }
    if (w->getWholeName() == firstCandidate_ && w->getMother() != nullptr &&
        w->getMother()->getFather() != nullptr
        && w->getMother()->getFather()->getWholeName() == secondCandidate_) {
        checkStatus_ = false; return;
    }

    if (w->getWholeName() == secondCandidate_ && w->getMother() != nullptr &&
        w->getMother()->getMother() != nullptr
        && w->getMother()->getMother()->getWholeName() == firstCandidate_) {
        checkStatus_ = false; return;
    }
    if (w->getWholeName() == secondCandidate_ && w->getMother() != nullptr &&
        w->getMother()->getFather() != nullptr
        && w->getMother()->getFather()->getWholeName() == firstCandidate_) {
        checkStatus_ = false; return;
    }
    
    //Checks for the children
    if (w->getWholeName() == firstCandidate_) {
        for (auto childItr : w->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (w->getWholeName() == secondCandidate_) {
        for (auto childItr : w->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //Checks for siblings
    if (w->getWholeName() == firstCandidate_ && w->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(w->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (w->getWholeName() == secondCandidate_ && 
        w->getMother() != nullptr) {
        for (auto childItr : static_cast<Woman*>(w->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //Checks for aunts, uncles, etc. for first candidate
    if (w->getWholeName() == firstCandidate_ && w->getMother() != nullptr 
        && w->getMother()->getMother() != nullptr) {
        for (auto childItr : static_cast<Woman*>(w->getMother()->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }
    else if (w->getWholeName() == firstCandidate_ && w->getFather() != nullptr 
        && w->getFather()->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(w->getFather()->getMother())->getChildren()) {
            if (childItr->getWholeName() == secondCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //Checks for aunts, uncles, etc. for second candidate
    if (w->getWholeName() == secondCandidate_ && w->getMother() != nullptr 
        && w->getMother()->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(w->getMother()->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    } else if (w->getWholeName() == secondCandidate_ && w->getFather() != nullptr && 
        w->getFather()->getMother() != nullptr) {
        for (auto childItr : 
            static_cast<Woman*>(w->getFather()->getMother())->getChildren()) {
            if (childItr->getWholeName() == firstCandidate_) {
                checkStatus_ = true; return;
            }
        }
    }

    //Makes sure fathers are counted as relatives
    if ((w->getWholeName() == firstCandidate_ && w->getFather() != nullptr 
        && w->getFather()->getWholeName() == secondCandidate_)
        || (w->getWholeName() == secondCandidate_ && w->getFather() != nullptr 
            && w->getFather()->getWholeName() == firstCandidate_)) {
        checkStatus_ = true; return;
    }


}

//Outputs final status to determine if they are relatives
//Or just siblings
string FriendOrRelative::returnStatus()
{
    //Outputs relatives if true, friends if false in this
    //if-else statement 
    if (checkStatus_)
        return firstCandidate_ + " and " + secondCandidate_ +
        " are relatives!";
    else
        return firstCandidate_ + " and " + secondCandidate_ +
        " are just friends.";
}
