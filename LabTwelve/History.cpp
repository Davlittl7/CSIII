//Davonte Littleton 
//CSIII
//2 December 2022

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::vector; using std::string;
using std::cout; using std::cin; using std::endl;

// receiver
class Document {
public:

	Document(const vector <string>& lines = {}) : lines_(lines) {}

	// actions
	void insert(int line, const string& str) {
		const int index = line - 1;
		if (index <= lines_.size())
			lines_.insert(lines_.begin() + index, str);
		else
			cout << "line out of range" << endl;
	}

	string remove(int line) {
		const int index = line - 1;
		string deletedLine = "";
		if (index < lines_.size()) {
			deletedLine = lines_[index];
			lines_.erase(lines_.begin() + index);
		}
		else
			cout << "line out of range" << endl;
		return deletedLine;
	}

	void show() {
		for (int i = 0; i < lines_.size(); ++i)
			cout << i + 1 << ". " << lines_[i] << endl;
	}

private:
	vector<string> lines_;
};

// abstract command
class Command {
public:
	Command(Document* doc) : doc_(doc) {}
	virtual void execute() = 0;
	virtual void unexecute() = 0;
	virtual void showCommand() = 0; //pure virtual utilized for history.
	virtual ~Command() {}
protected:
	Document* doc_;
};

// two concrete commands
class InsertCommand : public Command {
public:
	InsertCommand(Document* doc, int line, const string& str) :
		Command(doc), line_(line), str_(str) {}
	void execute() override { doc_->insert(line_, str_); }
	void unexecute() override { doc_->remove(line_); }
	void showCommand() override { cout << "Line " << line_ << " inserted string: " << str_; }
private:
	int line_;
	string str_;
};


class EraseCommand : public Command {
public:
	EraseCommand(Document* doc, int line) :
		Command(doc), line_(line), str_("") {}
	void execute() override { str_ = doc_->remove(line_); }
	void unexecute() override { doc_->insert(line_, str_); }
	void showCommand() override { cout << "Line " << line_ << " erased!"; }
private:
	int line_;
	string str_;
};

// client
class DocumentWithHistory {
public:
	DocumentWithHistory(const vector<string>& text = {}) : doc_(text) {}

	void insert(int line, string str) {
		Command* com = new InsertCommand(&doc_, line, str);
		com->execute();
		doneCommands_.push(com);
	}

	void erase(int line) {
		Command* com = new EraseCommand(&doc_, line);
		com->execute();
		doneCommands_.push(com);
	}

	void undo() {
		if (doneCommands_.size() != 0) {
			doneCommands_.top()->unexecute();
			doneCommands_.pop();
		}
		else
			cout << "no commands to undo" << endl;
		
	}

	void show() { doc_.show(); }

	void history() {
		std::stack<Command*> historyStack;
		int historyCount = 1;
		//Goes thru private stack
		while (!doneCommands_.empty()) {
			//histTmp takes doneCommand from top and 
			//then we remove the top from doneCommands stack
			Command* histTmp = doneCommands_.top();
			doneCommands_.pop();
			cout << historyCount << ". "; histTmp->showCommand(); cout << endl;			
			historyStack.push(histTmp);
			++historyCount;
		}

		//Goes through stack in history method 
		//Puts back lines
		while (!historyStack.empty()) {
			Command* histTmp = historyStack.top();
			historyStack.pop();
			doneCommands_.push(histTmp);
		}
	}

	void redo(int redoneCommand) {
		std::stack<Command*> redoTmp = doneCommands_;

		//Ensures that redo won't be run if there is nothing in doneCommands
		//Or if client puts too high of a number
		if (doneCommands_.size() == 0) {
			cout << "Nothing to redo!" << endl;
			return;
		}
		if (redoneCommand > doneCommands_.size()) {
			cout << "Invalid input." << endl;
			return;
		}

		//Pops until it reaches the destination it needs to be at.
		if (redoneCommand != 1) {
			for (int i = 0; i < redoneCommand - 1; ++i) {
				redoTmp.pop();
			}
		}

		//Pushes this redone command into the history and then executes it
		Command* redoComm = redoTmp.top();
		doneCommands_.push(redoComm);
		redoComm->execute();
	}

	//Clears all the history for when a checkpoint is made
	void clearHistory() { while (!doneCommands_.empty()) doneCommands_.pop(); }

	//Methods for creating a checkpoint and rolling back to that very point
	class Memento* createMemento() const;
	void rollback(class Memento* mem);
private:
	Document doc_;
	std::stack<Command*> doneCommands_;
};

//Class that helps for creating checkpoints and rolling back
class Memento {
public:
	Memento(const Document& doc) : doc_(doc) {}
	const Document& getState() const { return doc_; }
private:
	const Document doc_;
};

// copying the Document itself
Memento* DocumentWithHistory::createMemento() const { return new Memento(doc_); }

// copying back
void DocumentWithHistory::rollback(Memento* mem) { doc_ = mem->getState(); }

// invoker
int main() {
	DocumentWithHistory his({
	  "Lorem Ipsum is simply dummy text of the printing and typesetting",
	  "industry. Lorem Ipsum has been the industry's standard dummy text",
	  "ever since the 1500s, when an unknown printer took a galley of",
	  "type and scrambled it to make a type specimen book. It has",
	  "survived five centuries." });


	char option;
	//Do-while that runs until client enters an invalid option
	do {
		static Memento* checkpoint = nullptr;


		//Showcases the text from above"
		his.show();
		cout << endl;

		cout << "Enter option (i)nsert line, (e)rase line, (u)ndo last command, (c)heckpoint, roll(b)ack, "
			<< endl << "(h)istory, and (r)edo command (use any other char to quit): ";
		cin >> option;

		int line; string str;


		//Switch statement that holds all of of the options
		switch (option) {
		case 'i':
			cout << "line number to insert: ";
			cin >> line;
			cout << "line to insert: ";
			cin.get(); // removes end-of-line character so getline() works correctly
			getline(cin, str);
			his.insert(line, str);
			break;

		case 'e':
			cout << "line number to remove: ";
			cin >> line;
			his.erase(line);
			break;
		case 'u':
			his.undo();
			break;
		case 'c':
			checkpoint = his.createMemento();
			his.clearHistory();
			break;
		case 'b':
			if (checkpoint != nullptr) {
				his.rollback(checkpoint);
				delete checkpoint; checkpoint = nullptr;
			}
			break;
		case 'h':
			his.history();
			//line between history and text following it
			cout << "____________________________" << endl;
			break;
		case 'r':
			int redoLine;
			//Shows history so that the client can have more 
			//clarity on the decision
			his.history();
			cout << endl;
			//Takes in value for which line they choose to redo
			cout << "Which line would you like to redo: "; cin >> redoLine;
			his.redo(redoLine);
			break;
		}

	} while (option == 'i' || option == 'e' || option == 'u' || option == 'c' 
		|| option == 'b'|| option == 'h' || option == 'r');
}
