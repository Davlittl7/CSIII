//Davonte Littleton
//CSIII
//7 October 2022

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include<algorithm>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples {
    double weight; // oz
    string color;  // red or green
    void print() const { cout << color << ", " << weight << endl; }
};

int main() {
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input crate size: ";
    int size;
    cin >> size;

    vector <Apples> crate(size);

    // assign random weight and color to apples in the crate
    // replace with generate()
    std::generate(crate.begin(), crate.end(), [minWeight, maxWeight] {Apples i;
    i.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight); 
    i.color = rand() % 2 == 1 ? "green" : "red"; return i; });
  
    // for_each() possibly
    cout << "all apples" << endl;
    std::for_each(crate.begin(), crate.end(), [](const Apples& i) {i.print(); });


    cout << "Enter weight to find: ";
    double toFind;
    cin >> toFind;

    // count_if()
    //Counts num of apples greater than the toFind var captured in lambda
    int cnt = std::count_if(crate.begin(), crate.end(), [toFind](Apples i)
        {return i.weight > toFind; });

    cout << "There are " << cnt << " apples heavier than "
        << toFind << " oz" << endl;

    // find_if()
    auto weightMet = [toFind](Apples i) {return i.weight > toFind; };
    auto weightPos = std::find_if(crate.begin(), crate.end(), weightMet);
    if (weightPos != crate.end()) {
        cout << "at positions: ";
        do {
            cout << weightPos - crate.begin() << " ";
            weightPos = std::find_if(++weightPos, crate.end(), weightMet);
        } while (weightPos != crate.end());
    }
    cout << endl;

    // max_element()
    auto heavyWeight = std::max_element(crate.begin(), crate.end(),
        [](Apples i, Apples iTwo) {return i.weight < iTwo.weight; });
    cout << "Heaviest apple weighs: " << heavyWeight->weight << " oz" << endl;
    

    // for_each() or accumulate()
    double sum = 0;
    std::for_each(crate.begin(), crate.end(), [&sum](const Apples& i) 
        {sum += i.weight; });
    cout << "Total apple weight is: " << sum << " oz" << endl;


    // transform();
    cout << "How much should they grow: ";
    double toGrow;
    cin >> toGrow;
    
    //Transform that modifies the original crate to grow the apples
    std::transform(crate.begin(), crate.end(), crate.begin(),
       [toGrow](Apples i) {Apples iTwo; iTwo.weight = i.weight + toGrow; 
            iTwo.color = i.color; return iTwo; });

    // remove_if()
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;

    // removing small apples
    // nested loops, replace with a single loop modification idiom
    crate.erase(std::remove_if(crate.begin(), crate.end(), [minAccept](Apples i) 
        {return i.weight < minAccept; }), crate.end());


    // bubble sort, replace with sort()
    std::sort(crate.begin(), crate.end(), [](Apples i, Apples iTwo)
        {return i.weight < iTwo.weight; });


    // for_each() possibly
    cout << "sorted remaining apples" << endl;
    std::for_each(crate.begin(), crate.end(), [](const Apples& i) {i.print(); });

}
