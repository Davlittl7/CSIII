//Davonte Littleton
//CSIII
//18 November 2022

#include<iostream>
#include <string>
#include <set>
#include<vector>

using std::cout; using std::endl; using std::set;

enum class DrinkType { small, medium, large };

//Component
class Drink {
public:
    //Drink constructor that can make any size drink
    Drink(DrinkType type = DrinkType::small, int price = 5):
        type_(type), price_(price) {
        switch (type_) {
        case DrinkType::large:
            name_ = "large coffee with";
            break;
        case DrinkType::medium:
            name_ = "medium coffee with";
            break;
        default:
            name_ = "small coffee with";
            break;
        }
    }

    //Accessor and mutator functions
    virtual int getPrice() const { return price_; }
    virtual std::string getName() const { return name_; }

    virtual void setPrice(int newPrice) { price_ = newPrice; }
    virtual void setName(std::string newName) { name_ = newName; }
private:
    std::string name_;
    int price_;
    DrinkType type_;
};


//Sugar, Foam, and Cream are decorators
class Sugar : public Drink {
public:
    Sugar(const Drink* coffeeDrink) : coffeeDrink_(coffeeDrink) {}

    //Adding on to price and name of the drink
    int getPrice() const override { return coffeeDrink_->getPrice() + 1; }
    std::string getName() const override { 
        return coffeeDrink_->getName() + " sugar,"; 
    }
private: 
    const Drink* coffeeDrink_;
};

class Foam : public Drink {
public:
    Foam(const Drink* coffeeDrink): coffeeDrink_(coffeeDrink) {}

    //Adding on to price and name of the drink
    int getPrice() const override { return coffeeDrink_->getPrice() + 3; }
    std::string getName() const override { 
        return coffeeDrink_->getName() + " foam,";
    }
private:
    const Drink* coffeeDrink_;
};

class Cream : public Drink {
public: 
    Cream(const Drink* coffeeDrink): coffeeDrink_(coffeeDrink) {}

    //Adding on to price and name of the drink
    int getPrice() const override { return coffeeDrink_->getPrice() + 2; }
    std::string getName() const override { 
        return coffeeDrink_->getName() + " cream,"; 
    }
private: 
    const Drink* coffeeDrink_;
};


//Customer is the observer
class Customer {
public:
    Customer() {};
    Customer(class Barista* bar);
    virtual void notify() const = 0;
};

//Classes for chain of responsibility and 
//Barista is the subject for observer design pattern

class Barista {
public:
    //Functions for chain of responsibility
    Barista(Barista* successor = nullptr): successor_(successor) {}

    virtual void handleDrink(std::string order) {
        if (successor_ != nullptr) successor_->handleDrink(order);
        else cout << "No one can make your drink!" << endl;
    }

    //Register, deregister, and notify for observer implementation

    //adds a new customer to the set
    void registerCustomer(Customer* cust) {customers_.insert(cust);}

    //removes a customer from the set
    void deregisterCustomer(Customer* cust) { customers_.erase(cust); }
 
    void notifyCustomers() const {
        for (const auto& cust : customers_) cust->notify();
    }
    
    
    std::string getCustomerName() const { return customerName_; }
    std::string getNameOfDrink() const { return nameOfDrink_; }

    //Notifies all the customers within the set
    void finishDrink(const std::string drinkName, const std::string customerName) {
        nameOfDrink_ = drinkName;
        customerName_ = customerName;
        notifyCustomers();
    }

    size_t getCustomerSize() const { return customers_.size(); }

private:
    std::string nameOfDrink_;
    std::string customerName_;
    Barista* successor_;
    set<Customer*> customers_;
};

//Junior and Senior barista along with Manager for chain of responsibility
class JuniorBarista : public Barista {
public:
    JuniorBarista(Barista *successor = nullptr) : Barista(successor) {}
    void handleDrink(std::string order) override {
        size_t findSugar, findCream, findFoam;
        //Looking for each ingredient, to determine if this barista can make
        //the drink.
        findFoam = order.find("foam"); 
        findCream = order.find("cream");
        findSugar = order.find("sugar");
        if (findFoam == std::string::npos && findCream == std::string::npos
            && findSugar == std::string::npos) 
            cout << "the junior barista will make your: " << order;
        else Barista::handleDrink(order);
       
    }
};

class SeniorBarista : public Barista {
public:
    SeniorBarista(Barista* successor = nullptr) : Barista(successor) {}
    void handleDrink(std::string order) override {
        //Looking for only foam, as that is the only ingredient
        //the senior barista can't handle.
        size_t findFoam = order.find("foam");
        if (findFoam == std::string::npos) 
            cout << "the senior barista will make your: " << order;
        else Barista::handleDrink(order);
        
    }

};

class Manager : public Barista {
public: 
    Manager(Barista* successor = nullptr) : Barista(successor) {}
    void handleDrink(std::string order) override {
        //If it gets to the manager, they will simply make any drink 
        //w/ no issues.
        cout << "the manager will make your: " << order;
    }
};


//Concrete Observer
class ConcreteCustomer: public Customer {
public:
    ConcreteCustomer() : customerName_(""), customerOrder_(nullptr), drinkMaker_(nullptr) {}
    ConcreteCustomer(const std::string& customerName, Barista* bar, Drink* coffee): 
    customerName_(customerName), drinkMaker_(bar), customerOrder_(coffee) {}

    void notify() const override;

    std::string getDrinkName() const { return customerOrder_->getName(); }
    std::string getCustomerName() const { return customerName_; }
    void setCustomerName(std::string newCustomerName) { customerName_ = newCustomerName; }

    void setCustomerDrink(Drink* newOrder) { customerOrder_ = newOrder; }

private:
    std::string customerName_;
    Drink* customerOrder_;
    Barista* drinkMaker_;
};

//Function definitions from Customer and Concrete Customer

Customer::Customer(Barista* bar) { bar->registerCustomer(this); }

void ConcreteCustomer::notify() const {
    if (customerOrder_ != nullptr) {
        //Makes sure that the drink and customer's name match
        if (drinkMaker_->getNameOfDrink() == customerOrder_->getName() &&
            customerName_ == drinkMaker_->getCustomerName()) {
            cout << "Alright " << customerName_ << " ";
            drinkMaker_->handleDrink(customerOrder_->getName());
            cout << " and your total is: $" << customerOrder_->getPrice() << endl;
        }
    }
}
