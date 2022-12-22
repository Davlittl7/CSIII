//Davonte Littleton
//CSIII 
//9 December 2022

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include<map>
#include <algorithm>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string; using std::map;

enum class company {AirFrance, KLM, Aeroflot, Lufthansa, Delta, United, JetBlue};
//abstract flyweight
class Title {
public:
	virtual string logo() = 0;
};

class AirFrance : public Title {
public:
	string logo() override { return "AIRFRANCE"; }
};

class KLM : public Title {
public:
	string logo() override { return "KLM"; }
};

class Aeroflot : public Title {
public:
	string logo() override { return "AEROFLOT"; }
};

class Lufthansa: public Title {
public:
	string logo() override { return "LUFTHANSA"; }
};

class Delta : public Title {
public:
	string logo() override { return "DELTA"; }
};

class United : public Title {
public:
	string logo() override { return "UNITED"; }
};

class JetBlue : public Title {
public:
	string logo() override { return "JETBLUE"; }
};


//Factory to set a new logo depending on their enum class
class CompanyFactory {
public: 
	static Title* makeTitle(company compTitle) {
		if (titles_.find(compTitle) == titles_.end()) {
			switch (compTitle) {
			case company::AirFrance:
				titles_[compTitle] = new AirFrance; break;
			case company::KLM:
				titles_[compTitle] = new KLM; break;
			case company::Aeroflot:
				titles_[compTitle] = new Aeroflot; break;
			case company::Lufthansa:
				titles_[compTitle] = new Lufthansa; break;
			case company::Delta:
				titles_[compTitle] = new Delta; break;
			case company::United:
				titles_[compTitle] = new United; break;
			case company::JetBlue:
				titles_[compTitle] = new JetBlue; break;
			}
		}
		return titles_[compTitle];
	}
private:
	static map<company, Title*> titles_;
};

map<company, Title*> CompanyFactory::titles_;

// abstract mediator
class Controller {
public:
	virtual void join(class Flight*) = 0;
	virtual void leave(class Flight*) = 0;
	virtual void broadcast() = 0;
	virtual void observe() = 0;
};


// concrete mediator
// tower, runway clearance
class Tower : public Controller {
public:
	void join(class Flight*) override;
	void leave(class Flight*) override;
	void broadcast() override;
	void observe() override;
private:
	std::set<class Flight*> waiting_;
};


// abstract colleague
class Flight {
public:
	Flight(Controller* controller) :controller_(controller), status_(Status::waiting) {
		task_ = rand() % 2 ? Task::taxiing : Task::approaching;
		controller_->join(this);
	}
	void receive(const string& msg) {
		if (msg.find(flightNo_) != string::npos || msg.find("all") != string::npos) {
			if (msg.find("clear") != string::npos) {
				cout << flightNo_ << " roger that, ";
				cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
				status_ = Status::cleared;
				
			}
			else if (msg.find("status") != string::npos) {
				cout << flightNo_
					<< (status_ == Status::waiting ? " waiting to " : " cleared to ")
					<< (task_ == Task::taxiing ? "take off" : "land") << endl;
			}
			else
				cout << "Tower, this is " << flightNo_ << " please repeat." << endl;
		}
	}
	bool isCleared() const { return status_ == Status::cleared; }
	void proceed() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1)); // wait a while    
		//Prints out the new message with their logo in all caps
		cout << (task_ == Task::taxiing ? "...." + logo_->logo() + 
			std::to_string(numberFlight_) + " took off" + "...." : "...." +
			logo_->logo() + std::to_string(numberFlight_) + " landed" + "....") << endl;
		controller_->leave(this);
	}

protected:
	string flightNo_;
	Title* logo_;
	Controller* controller_;
	int numberFlight_;
	enum class Task { taxiing, approaching };  Task task_;
	enum class Status { waiting, cleared };    Status status_;
};

// concrete colleagues
class Airbus : public Flight {
public:
	Airbus(Tower* tower) : Flight(tower) {
		static const std::vector<string> companies =
		{ "AirFrance", "KLM", "Aeroflot", "Lufthansa" };

		//Setup for the flightNo_ name
		numberFlight_ = rand() % 1000;
		flightNo_ = companies[rand() % companies.size()] +
			std::to_string(numberFlight_);

		//If-else-if statements in order to determine logo
		//Looks for specific company name in string
		if (flightNo_.find("AirFrance") != string::npos) 
			logo_ = CompanyFactory::makeTitle(company::AirFrance);
		else if (flightNo_.find("KLM") != string::npos) 
			logo_ = CompanyFactory::makeTitle(company::KLM);
		else if (flightNo_.find("Aeroflot") != string::npos) 
			logo_ = CompanyFactory::makeTitle(company::Aeroflot);
		else if (flightNo_.find("Lufthansa") != string::npos) 
			logo_ = CompanyFactory::makeTitle(company::Lufthansa);
		
		cout << flightNo_ << " requesting "
			<< (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
	}
};

class Boeing : public Flight {
public:
	Boeing(Tower* tower) : Flight(tower) {
		static const std::vector<string> companies =
		{ "Delta", "United", "JetBlue" };
		
		//Setup for the flightNo_ name
		numberFlight_ = rand() % 1000;
		flightNo_ = companies[rand() % companies.size()] +
			std::to_string(numberFlight_);

		//If-else-if statements in order to determine logo
		//Looks for specific company name in string
		if (flightNo_.find("Delta") != string::npos)
			logo_ = CompanyFactory::makeTitle(company::Delta);
		else if (flightNo_.find("United") != string::npos)
			logo_ = CompanyFactory::makeTitle(company::United);
		else if (flightNo_.find("JetBlue") != string::npos)
			logo_ = CompanyFactory::makeTitle(company::JetBlue);

		cout << flightNo_ << " requesting "
			<< (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
	}
};


// member functions for Tower
void Tower::broadcast() {
	cout << "Tower: ";
	string msg;
	getline(cin, msg);
	if (!msg.empty())
		for (auto f : waiting_) f->receive(msg);
}


void Tower::observe() {
	auto findCleared = [](Flight* f) {return f->isCleared(); };

	auto toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);

	while (toProceed != waiting_.end()) { // found a cleared flight
		(*toProceed)->proceed();
		toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
		if (toProceed != waiting_.end())
			cout << "MAYDAY! MAYDAY! MAYDAY! " << endl; // more than a single flight cleared
	}
}


void Tower::join(Flight* f) {
	waiting_.insert(f);
}


void Tower::leave(Flight* f) {
	waiting_.erase(f);
	delete f;
}

int main() {
	srand(time(nullptr));
	Tower jfk;

	new Boeing(&jfk);
	new Airbus(&jfk);
	new Boeing(&jfk);
	new Airbus(&jfk);

	while (true) {
		jfk.broadcast();
		jfk.observe();
		if (rand() % 2) {
			if (rand() % 2)
				new Boeing(&jfk);
			else
				new Airbus(&jfk);
		}
	}
}
