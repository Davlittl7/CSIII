//Davonte Littleton
//CSIII
//7 October 2022

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include<algorithm>
#include<functional>
#include<numeric>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Peaches {
    double weight; // oz
    bool ripe;  // ripe or not
    void print() const { cout << (ripe ? "ripe" : "green") << ", " << weight << endl; }
};

class entity {
public:
    entity(double peachWeight): maxWeight_(peachWeight) {}
    double operator() (double jamSum, const Peaches& p) {
        if (p.weight < maxWeight_) {
            return jamSum += p.weight;
        }
        return jamSum;
    }
private:
    double maxWeight_;
};


int main() {
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input basket size: ";
    int size;
    cin >> size;

    vector <Peaches> basket(size);

    // assign random weight and ripeness peaches in the basket
    // replace with generate()
    std::generate(basket.begin(), basket.end(), [minWeight, maxWeight] {Peaches p;
    p.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
    p.ripe = rand() % 2; return p; });

    // for_each() possibly
    cout << "all peaches" << endl;
    std::for_each(basket.begin(), basket.end(), [](const Peaches &p) {p.print(); });

    // moving all the ripe peaches from basket to peck
    // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    deque<Peaches> peck;

    //If not ripe, it will remain in the basket
    //Inserts ripe peaches into peck
    //If true, it doesn't copy into peck
    std::remove_copy_if(basket.begin(), basket.end(), std::back_inserter(peck),
        [](Peaches p) {return !p.ripe; });

    //Removes anything from basket that remove_if returns as true
    //In this case, it removes any peach that is ripe
    basket.erase(std::remove_if(basket.begin(), basket.end(), 
        [](Peaches p) {return p.ripe; }), basket.end());

    // for_each() possibly
    cout << "peaches remainng in the basket" << endl;
    std::for_each(basket.begin(), basket.end(), [](const Peaches& p) {p.print(); });
    cout << endl;

    // for_each() possibly
    cout << "peaches moved to the peck" << endl;
    std::for_each(peck.begin(), peck.end(), [](const Peaches &p) {p.print(); });

    // prints every "space" peach in the peck
    // 
    const int space = 3;
    cout << "\nevery " << space << "\'d peach in the peck" << endl;

    // replace with advance()/next()/distance()
    // no iterator arithmetic
    auto it = peck.cbegin(); int i = 0;

    //Begins the advancement
    std::advance(it, space);

    //Checks before starting the do-while if it is at the end
    if (it != peck.cend()) {
        //Do-while that prints space'd peach and stops if there
        //aren't enough spaces
        do
        {
            it->print();
            if (std::distance(it, peck.cend()) < space)
                it = peck.cend();
            else 
                std::advance(it, space);
        } while (it != peck.cend());
    }

    // putting all small ripe peaches in a jam
    // use a binder to create a functor with configurable max weight
    // accumulate() or count_if() then remove_if()
    
    const double weightToJam = 10.0;
    double jamWeight = 0;
    
    //Uses functor and bind to have a free parameter for max weight
    jamWeight = std::accumulate(peck.begin(), peck.end(), jamWeight,
        std::bind(entity(weightToJam), std::placeholders::_1, std::placeholders::_2));

    //Removes peaches that were turned to jam
    peck.erase(std::remove_if(peck.begin(), peck.end(), 
        [weightToJam](const Peaches p) {return p.weight < weightToJam; }), peck.end());
    
    //Outputs the Jam's weight
    cout << "Weight of jam is: " << jamWeight << endl;

}
