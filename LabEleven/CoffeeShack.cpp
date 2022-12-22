//Davonte Littleton
//CSIII
//18 November 2022

#include<iostream>
#include<cstdlib>
#include<vector>
#include "drinks.hpp"

using std::cout; using std::endl; using std::cin;
using std::vector;

int main() {
	const int numOfCustomers = 4;
	srand(time(nullptr));
	//Initializes a coffee size and then prompts the user for an input
	char coffeeSize;
	//Using chain of responsibility to determine who made the drink
	Barista* drinkMaker = new JuniorBarista(new SeniorBarista(new Manager));
	
	vector<ConcreteCustomer> customers;

	//Pushing back generic customers
	for (int i = 0; i < numOfCustomers; ++i) 
		customers.push_back(ConcreteCustomer("", drinkMaker, nullptr));
	
	
	//Counting number of drinks so that we can stop at a certain number
	int numOfDrinks = 0;

	//Indexing to make sure to modify elements at the right locations
	int indexing = 0;

	cout << "We've got " << numOfCustomers << " orders to take everyone!" << endl;
	//While loop that goes until we made a set amount of drinks
	while (numOfDrinks != numOfCustomers) {
		int randBaristaChoice = rand() % 2;
		//If-else for determining if a new order will be placed or if
		//Someone's order will get called.
		if (randBaristaChoice == 0 || numOfDrinks == 0) {
			cout << "Welcome to Davonte's Coffee Shack!" << endl;
			cout << "Can I get you a [l]arge, [m]edium, or a [s]mall coffee?: ";
			cin >> coffeeSize;


			Drink* order = nullptr;
			//Switch case for determining the coffee size based on input
			switch (coffeeSize) {
			case 'l':
				order = new Drink(DrinkType::large, 10);
				break;
			case 'm':
				order = new Drink(DrinkType::medium, 7);
				break;
			case's':
				order = new Drink();
				break;
			default:
				cout << "I need a proper coffee size!" << endl;
				break;
			}

			if (order != nullptr) {
				std::string customerName; //String var for the name of the customer
				char addOn; //character variable for added on ingredients
				do
				{
					//Prompt user for what addOns they would like and take their value
					cout << "Would you like to add [s]ugar, [c]ream, milk [f]oam,"
						<< " or are you [d]one?: "; cin >> addOn;

					//Switch case for each addOn they input, which adds onto final 
					//order string and increases price of order depending on the case
					switch (addOn) {
					case 's':
						order->setName(Sugar(order).getName());
						order->setPrice(Sugar(order).getPrice());
						break;
					case 'c':
						order->setName(Cream(order).getName());
						order->setPrice(Cream(order).getPrice());
						break;
					case 'f':
						order->setName(Foam(order).getName());
						order->setPrice(Foam(order).getPrice());
						break;
					case 'd':
						break;
					default:
						cout << "I'm sorry could you repeat that, "
							<< "I think you said it wrong" << endl;
						break;
					}
				} while (addOn != 'd');


				//Outputting the final total of the drink and who makes it
				cout << "Finally, could I get your name?: "; cin >> customerName;
				customers[indexing].setCustomerName(customerName);
				customers[indexing].setCustomerDrink(order);
				drinkMaker->registerCustomer(&customers[indexing]);

				cout << endl;
				++numOfDrinks;
				++indexing;
			}
		}
		else {
			//Notifies all customers that one of the drinks are finished
			if (drinkMaker->getCustomerSize() != 0 && drinkMaker != nullptr) {
				int randDrinkToFinish = rand() % (drinkMaker->getCustomerSize());
				drinkMaker->finishDrink(customers[randDrinkToFinish].getDrinkName(),
					customers[randDrinkToFinish].getCustomerName());
				drinkMaker->deregisterCustomer(&customers[randDrinkToFinish]);
				cout << endl;
				customers.erase(customers.begin() + randDrinkToFinish);
				--indexing;
			}
		}
	}

}
