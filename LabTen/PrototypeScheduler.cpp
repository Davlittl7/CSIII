//Davonte Littleton
//CSIII
//11 November 2022


#include<iostream>
#include<cstdlib>
#include<string>
#include<queue>
#include<deque>
#include<algorithm>

using std::cout; using std::endl; using std::string;
using std::deque;

//Forward class declaration 
class State;
class Process {
public:
	Process();

	//Behaviors
	void dispatch(); 
	void unblock(); 
	void suspend(); 
	void block(); 
	string report(); 

	
	//Assign a number to each process 
	//uses a static var to increase w/ each invocation for identifier
	void assignProcessNumber() { 
		static int ID = 1;
		processNumber_ = ID++; 
	}

	//Returns the process's number/ID
	//Accessor function
	int getProcessNumber() const { return processNumber_; }

	//Clone function for prototype design pattern
	Process* clone() { return new Process(); }

	void changeState(State* state) { state_ = state; }
private:
	int processNumber_;
	State* state_;
};

class State {
public: 
	//Virtual behavior functions w/bodies so that they 
	//do not have to be implemented by every derived class
	virtual void dispatch(Process*) {}
	virtual void unblock(Process*) {}
	virtual void suspend(Process*) {}
	virtual void block(Process*) {}

	//Pure virtual function that must be implemented in every derived class
	virtual string report() = 0;

	void changeState(Process* p, State* s) { p->changeState(s); }
};

class Ready : public State {
	//Dispatch only behavior implemented 
public:
	//Singleton implementation
	static State* instance() {
		static State* onlyInstance = new Ready;
		return onlyInstance;
	}

	//State transition func prototype
	void dispatch(Process*) override;

	string report() override { return "Ready"; }
private:
	//Private constructor, copy constructor, and assignment operator
	//To forbid all instatiations
	Ready() {};
	Ready(const Ready&) = delete;
	Ready& operator=(const Ready&) = delete;

};

class Running : public State {
	//Block, suspend, and exit are the behaviors implemented
public:
	//Singleton implementation
	static State* instance() {
		static State* onlyInstance = new Running;
		return onlyInstance;
	}
	
	//State transition func prototypes
	void block(Process*) override;
	void suspend(Process*) override;

	string report() override { return "Running"; }
private:
	//Private constructor, copy constructor, and assignment operator
	//To forbid all instatiations
	Running() {};
	Running(const Running&)  = delete;
	Running& operator=(const Running&) = delete;
};

class Blocked : public State {
	//Unblock only 
public:
	//Classic singleton
	static State* instance() {
		static State* onlyInstance = new Blocked;
		return onlyInstance;
	}

	//State transition func prototype
	void unblock(Process*) override;

	string report() override { return "Blocked"; }
private:
	//Private constructor, copy constructor, and assignment operator
	//To forbid all instatiations
	Blocked() {};
	Blocked(const Blocked&) = delete;
	Blocked& operator=(const Blocked&) = delete;
};

//State transition functions implementation
void Ready::dispatch(Process* p) {
	changeState(p, Running::instance());
}

void Running::block(Process* p) {
	changeState(p, Blocked::instance());
}

void Running::suspend(Process* p) {
	changeState(p, Ready::instance());

}

void Blocked::unblock(Process* p) {
	changeState(p, Ready::instance());
}

//Process behavior implementations
void Process::dispatch() { state_->dispatch(this); }
void Process::unblock() {  state_->unblock(this); }
void Process::block() { state_->block(this); }
void Process::suspend() { state_->suspend(this); }
string Process::report() { return state_->report(); }


//Process constructor that starts every 
//Process as ready
Process::Process() { 
	state_ = Ready::instance(); 
	assignProcessNumber();
};



int main() {
	srand(time(nullptr));
	//Number of processes to be put into ready queue
	int numProcesses = 4;

	//Creating a prototype process and ready and block queues
	//and making a ptr to our process prototype
	Process prototypeProcess;
	Process* processPtr = &prototypeProcess;
	deque<Process*> readyQueue, blockedQueue;

	//Integers that are used for the random choices 
	//needed for the program
	int runStateDecision, blockStateDecision;
	
	//Pushing back the first prototype Process and then cloning 3 more
	readyQueue.push_back(processPtr);
	for (int i = 0; i < numProcesses - 1; ++i) {
		readyQueue.push_back(prototypeProcess.clone());
	}

	//For loop to show that each process is numbered and that they 
	//have been created
	for (int i = 0; i < readyQueue.size(); ++i) {
		cout << "Process " << readyQueue[i]->getProcessNumber() <<
			" has been created" << endl;
		cout << "Process " << readyQueue[i]->getProcessNumber() <<
			"'s state is " << readyQueue[i]->report() << endl;
		cout << endl;
	}

	
	cout << "---------------Scheduler is now running---------------" << endl;

	//While loop that runs until both ready and blockqueue have nothing in them
	while (!readyQueue.empty() || !blockedQueue.empty()) {
		//1: Suspend 2: Block 3: Exit
		runStateDecision = (rand() % 3) + 1;

		//50% chance of unblocking or staying blocked
		blockStateDecision = (rand() % 2) + 1;

		//Will run as long as readyQueue has at least one element
		if(!readyQueue.empty()) {
			cout << "Dispatching..." << endl;
			readyQueue.front()->dispatch();
			cout << "Process " << readyQueue.front()->getProcessNumber() <<
				"'s state is " << readyQueue.front()->report() << endl;
		}
		else cout << "Ready Queue is empty!" << endl;

		//If process is suspended
		if (!readyQueue.empty() && runStateDecision == 1) {
			cout << "Suspending..." << endl;
			readyQueue.front()->suspend();
			cout << "Process " << readyQueue.front()->getProcessNumber() <<
				"'s state is " << readyQueue.front()->report() << endl;

			//Pushes back the value at the front 
			readyQueue.push_back(readyQueue.front());

			//Deletes the process from the front as it has been pushed to back
			readyQueue.pop_front();
		}



		//If process is blocked
		if (!readyQueue.empty() && runStateDecision == 2) {
			cout << "Blocking..." << endl;
			readyQueue.front()->block();
			cout << "Process " << readyQueue.front()->getProcessNumber() <<
				"'s state is " << readyQueue.front()->report() << endl;

			//Puts element in front into the back of the blockedQueue
			blockedQueue.push_back(readyQueue.front());

			//Takes the element that was blocked out of readyQueue
			readyQueue.pop_front();
		}

		//If process exits
		if (!readyQueue.empty() && runStateDecision == 3) {
			cout << "Exiting..." << endl;
			cout << "Process " << readyQueue.front()->getProcessNumber() 
				<< " has exited" << endl;
			readyQueue.pop_front();
		}

		//If there is a blocked process 
		//1: nothing happens 2: unblocked
		cout << endl;
		if (!blockedQueue.empty() && blockStateDecision == 2) {
			cout << "Unblocking..." << endl;
			blockedQueue.front()->unblock();
			cout << "Process " << blockedQueue.front()->getProcessNumber() <<
				"'s state is " << blockedQueue.front()->report() << endl;

			//Puts element in front into the back of the blockedQueue
			readyQueue.push_back(blockedQueue.front());

			//Takes the element that was blocked out of readyQueue
			blockedQueue.pop_front();
		}
		cout << endl;
	}
	cout << "All processes have exited!" << endl;
	cout << "---------------Scheduler is done---------------" << endl;


	
}
