Skip to content
Search or jump to…
Pull requests
Issues
Codespaces
Marketplace
Explore
 
@Davlittl7 
Davlittl7
/
CSIII
Public
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
Settings
CSIII/LabEight/BridgeFigures.cpp
@Davlittl7
Davlittl7 Create BridgeFigures.cpp
Latest commit c4f6c3d 6 minutes ago
 History
 1 contributor
158 lines (135 sloc)  4.25 KB

//Davonte Littleton
//CSIII
//4 November 2022

#include <iostream>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill {
public:
	Fill(char internalChar, char externalChar) : 
		internalChar_(internalChar), externalChar_(externalChar) {}
	virtual char getBorder() = 0;
	virtual char getInternal() = 0;
	virtual ~Fill() {}
protected:
	char internalChar_;
	char externalChar_;
};

// concrete body
class Hollow : public Fill {
public:
	//Only need one parameter for hollow as the other is space
	Hollow(char externalChar) :Fill(' ', externalChar) {}
	char getBorder() override { return externalChar_; }
	char getInternal() override { return internalChar_; }
	~Hollow() override {}
};


// another concrete body
class Filled : public Fill {
public:
	//Two constructors for filled to be initialized to the same character
	//And another for derived classes to utilize
	Filled(char fillCharacter): Fill(fillCharacter, fillCharacter) {}
	Filled(char insideFill, char outsideFill):Fill(insideFill, outsideFill) {}
	char getBorder() override { return externalChar_; }
	char getInternal() override { return internalChar_; }
	~Filled() override {}
};

class FullyFilled : public Filled {
public:
	//Uses second constructor of filled base class
	FullyFilled(char inside, char outside): Filled(inside, outside) {}
	char getBorder() override { return externalChar_; }
	char getInternal() override { return internalChar_; }
	~FullyFilled() override {}
};

class RandomFilled : public Filled {
public: 
	//Uses second constructor of filled base class
	RandomFilled(char insideOrOutside, char outsideOrInside) : 
		Filled(insideOrOutside, outsideOrInside) {}
	char getBorder() override {
		srand(time(nullptr));
		//return based on determineChar
		int determineChar = rand() % 2;
		if (determineChar == 0) return internalChar_;
		else return externalChar_;
	}
	char getInternal() override { 
		srand(time(nullptr));
		//return based on determineChar
		int determineChar = rand() % 2;
		if (determineChar == 0) return externalChar_;
		else return internalChar_;
	}
	~RandomFilled() override {}
};

// abstract handle
class Figure {
public:
	Figure(int size, Fill* fill) : size_(size), fill_(fill) {}
	virtual void draw() = 0;
	virtual ~Figure() {}
protected:
	int size_;
	Fill* fill_;
};

// concrete handle
class Square : public Figure {
public:
	Square(int size, Fill* fill) : Figure(size, fill) {}
	void draw() override;

};

void Square::draw() {
	for (int i = 0; i < size_; ++i) {
		for (int j = 0; j < size_; ++j)
			if (i == 0 || j == 0 || i == size_ - 1 || j == size_ - 1)
				cout << fill_->getBorder();
			else
				cout << fill_->getInternal();
		cout << endl;
	}
}

int main() {

	//Declares two char variables to use for squares
	char fillCharOne, fillCharTwo;
	int size;
	
	//Creates and draws the filled square
	cout << "Enter fill character for filled square: "; cin >> fillCharOne;
	cout << "Enter size: "; cin >> size;
	Figure* userBox = new Square(size, static_cast<Fill*>(new Filled(fillCharOne)));
	userBox->draw();
	cout << endl;

	
	//Frees up userBox after making filled square
	delete userBox;

	//Creates and draws the hollowSquare
	cout << "Enter external character for hollow square: "; cin >> fillCharOne;
	cout << "Enter size: "; cin >> size;
	userBox = new Square(size, static_cast<Fill*>(new Hollow(fillCharOne)));
	userBox->draw();
	cout << endl;

	//Frees up userBox after making hollow square
	delete userBox;

	//Creates and draws the fully filled square
	cout << "Enter internal character for fully filled square: "; 
	cin >> fillCharOne;
	cout << "Enter external character for fully filled square: "; 
	cin >> fillCharTwo;
	cout << "Enter size: "; cin >> size;
	userBox = new Square(size, static_cast<Fill*>(new FullyFilled(fillCharOne, fillCharTwo)));
	userBox->draw();
	cout << endl;

	//Frees up userBox after making fully filled square
	delete userBox;
	
	//Creates and draws the random filled square
	cout << "Enter a character for random filled square: ";
	cin >> fillCharOne;
	cout << "Enter a 2nd character for fully filled square: ";
	cin >> fillCharTwo;
	cout << "Enter size: "; cin >> size;
	userBox = new Square(size, static_cast<Fill*>(new RandomFilled(fillCharOne, fillCharTwo)));
	userBox->draw();
	cout << endl;

	//Frees up userBox after making random filled square
	delete userBox;
}
