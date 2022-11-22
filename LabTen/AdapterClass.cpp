//Davonte Littleton
//CSIII
//11 November 2022

#include <iostream>

using std::cout; using std::endl; using std::cin;

// base interface
class Figure {
public:
	virtual void draw() = 0;
	virtual ~Figure() {}
};

// adaptee/implementer
class LegacyRectangle {
public:
	LegacyRectangle(int topLeftX,
		int topLeftY,
		int bottomRightX,
		int bottomRightY) :
		topLeftX_(topLeftX),
		topLeftY_(topLeftY),
		bottomRightX_(bottomRightX),
		bottomRightY_(bottomRightY) {}

	void oldDraw() {
		for (int i = 0; i < bottomRightY_; ++i) {
			for (int j = 0; j < bottomRightX_; ++j)
				if (i >= topLeftY_ && j >= topLeftX_)
					cout << '*';
				else
					cout << ' ';
			cout << endl;
		}
	}
	// defining top/left and bottom/right coordinates 

	//Sets all private attributes to new values
	void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
		topLeftX_ = topLeftX;
		topLeftY_ = topLeftY;
		bottomRightX_ = bottomRightX;
		bottomRightY_ = bottomRightX;
	}

	//Accessor functions
	int getTopLeftX() const { return topLeftX_; }
	int getTopLeftY() const { return topLeftY_; };
	int getBottomRightX() const { return bottomRightX_; };
	int getBottomRightY() const { return bottomRightY_; };
private:
	int topLeftX_;
	int topLeftY_;
	int bottomRightX_;
	int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter : public Figure,
	private LegacyRectangle {
public:
	SquareAdapter(int size) :
		LegacyRectangle(0, 0, size, size) {};
	void draw() override {
		oldDraw();
	}

	//Retrives the size of the square
	int size() const {
		return getBottomRightX();
	}

	//Changes the square to a newSize provided by the parameter
	void resize(int newSize) {
		move(0, 0, newSize, newSize);
	}
};


int main() {
	int sizeInputted;
	cout << "What size square would you like?: "; cin >> sizeInputted;

	//Creates the square
	SquareAdapter* square = new SquareAdapter(sizeInputted);
	square->draw();

	//Takes in the new size
	cout << "What would you like the new size to be?: "; cin >> sizeInputted;

	//Resizes the newly sized square and then draws it
	square->resize(sizeInputted);
	square->draw();

	cout << "The new size is: " << square->size() << endl;
}
