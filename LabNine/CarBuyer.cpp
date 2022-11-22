//Davonte Littleton
//CSIII
//4 November 2022

#include <iostream>
#include<array>
#include<algorithm>
#include<iterator>
#include <vector>
#include <cstdlib>
#include "CarFactory.hpp"

using std::vector;
using std::cout; using std::endl;

class CarLot {
public:
    CarLot();

    // if a car is bought, requests a new one
    Car* buyCar(Car* boughtCar) {
        //Boolean to find bought car
        bool foundBoughtCar = false;

        //Searches for the car that must be replaced in this for loop
        for (int i = 0; i < sizeOfLot; ++i) {
            //If-statement stops running once foundBoughtCar is true
            //Makes sure the right car is replaced
            if (cars4sale_[i] == boughtCar && foundBoughtCar == false) {
                foundBoughtCar = true;
                cars4sale_[i] = factories_[rand() % factories_.size()]->requestCar();
            }
        }
        return boughtCar;
    }

    Car* nextCar() {
        //Counts amount of time nextCar has been called 
        static int carRunThrough = 0;
        if (carRunThrough == sizeOfLot) carRunThrough = 0;
        return cars4sale_[carRunThrough++];
    }

    int lotSize() { return sizeOfLot; }

private:
    //Static int for the amount of cars the lot can hold 
    const static int sizeOfLot = 10;
    vector<Car*> cars4sale_;
    vector<CarFactory*> factories_;
};


CarLot::CarLot() {
    // creates 2 Ford factories and 2 Toyota factories 
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());


    //Adds new cars to cars4sale randomly picked from the factories 
    for (int i = 0; i < sizeOfLot; ++i)
        cars4sale_.push_back(factories_[rand() % factories_.size()]->requestCar());

}



CarLot* carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id) {
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    //Randomly choosing the model that they want from toyota
    std::array<std::string, 5> models = { "Corolla", "Camry", "Prius", "4Runner", "Yaris" };
    std::string modelWanted = models[rand() % models.size()];

    cout << "Jill Toyoter " << id << " wants a: Toyota " <<
        modelWanted << endl;

    bool foundCar = false;
    //For loop to make a lotSize number of attempts
    for (int i = 0; i < carLotPtr->lotSize(); ++i) {
        //Changes the car they test drive
        Car* toBuy = carLotPtr->nextCar();
        cout << "Jill Toyoter " << id << endl;
        cout << "test driving "
            << toBuy->getMake() << " "
            << toBuy->getModel();
        //If-else statement to ensure car matches what they want
        if (toBuy->getMake() == "Toyota" && toBuy->getModel() == modelWanted) {
            foundCar = true;
            cout << ": love it! buying it!" << endl;
            carLotPtr->buyCar(toBuy);
            i = carLotPtr->lotSize();
            cout << "Jill Toyoter " << id << " found the car they wanted!" << endl;
        }
        else cout << ": did not like it!" << endl;
    }
    //Tells user they did not find the car if foundCar is false
    if (!foundCar) cout << "Jill Toyoter couldn't find desired car" << endl;
    cout << endl;
}

// test-drives a car
// buys it if Ford
void fordLover(int id) {
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    //Randomly chooses ford model
    std::array<std::string, 4> models = { "Focus", "Mustang", "Explorer", "F-150" };
    std::string modelWanted = models[rand() % models.size()];


    cout << "Jack Fordman " << id << " wants a: Ford " <<
        modelWanted << endl;

    bool foundCar = false;
    //For loop to make a lotSize number of attempts
    for (int i = 0; i < carLotPtr->lotSize(); ++i) {
        //Changes the car they test drive
        Car* toBuy = carLotPtr->nextCar();
        cout << "Jack Fordman " << id << endl;
        cout << " test driving "
            << toBuy->getMake() << " "
            << toBuy->getModel();

        //If-else statement to check if car is exactly what they want or not
        if (toBuy->getMake() == "Ford" && toBuy->getModel() == modelWanted) {
            foundCar = true;
            cout << ": love it! buying it!" << endl;
            carLotPtr->buyCar(toBuy);
            i = carLotPtr->lotSize();
            cout << "Jack Fordman " << id << " found the car they wanted!" << endl;
        }
        else cout << ": did not like it!" << endl;
    }
    //Outputs if they did not find their car if foundCar is false
    if (!foundCar) cout << "Jack Fordman couldn't find desired car" << endl;
    cout << endl;
}



int main() {

    srand(time(nullptr));   
    //Takes numOfBuyers and outputs and calls the functions 
    //that amount of times
    const int numOfBuyers = 3;
    for (int i = 0; i < numOfBuyers; ++i) {
        rand() % 2 == 0 ? toyotaLover(i + 1) : fordLover(i + 1);
    }
}
