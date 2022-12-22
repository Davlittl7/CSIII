//Davonte Littleton
//CSIII
//9 December 2022

#include <set>
#include <map>
#include <iostream>
#include <string>
#include <algorithm>

using std::cout; using std::endl;
using std::map; using std::set;
using std::string;

class Observer;
class Subject;

class EventRegistry {
public:
    static void registerObserver(string, Observer*);
    static void deregisterObserver(string, Observer*);
    static void handleMessage(string);

private:
    static map<string, set<Observer*>> observerMap_;
};

// initialize the static map
map<string, set<Observer*>> EventRegistry::observerMap_;

class Observer {
public:
    Observer(const string& name) : name_(name) {}
    string getName() const { return name_; }

    void subscribe(string s) { EventRegistry::registerObserver(s, this); }
    void unsubscribe(string s) { EventRegistry::deregisterObserver(s, this); }
    void handleMessage(string);
private:
    string name_;
};

class Subject {
public:
    Subject(const string& name) : name_(name) {}
    string getName() const { return name_; }
    void generateMessage() {
        cout << name_ << " sent a message" << endl;
        EventRegistry::handleMessage(name_);
    }
private:
    string name_;
};

void EventRegistry::registerObserver(string s, Observer* o) {
    observerMap_[s].insert(o);
    cout << o->getName() << " subscribed to " << s << endl;
}

void EventRegistry::deregisterObserver(string s, Observer* o) {
    observerMap_[s].erase(o);
    cout << o->getName() << " unsubscribed from " << s << endl;
}

void EventRegistry::handleMessage(string s) {
    for (auto e : observerMap_[s])
        e->handleMessage(s);
}

void Observer::handleMessage(string s) {
    cout << name_ << " received message from " << s << endl;
}


int main() {
    Observer olaf("Observer Olaf"), olga("Observer Olga");
    Subject  sue("Subject Sue"), steve("Subject Steve");

    olaf.subscribe(sue.getName()); olaf.subscribe(steve.getName());
    olga.subscribe(sue.getName());

    cout << endl;


    sue.generateMessage();
    steve.generateMessage();
    cout << endl;

    olga.unsubscribe(sue.getName());
    sue.generateMessage();
}
