//Davonte Littleton
//CSIII
//7 October 2022

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include<map>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;
using std::multimap;

enum class Variety { orange, pear, apple };
vector<string> colors = { "red", "green", "yellow" };


int main() {

    //Initializing random seed and multimap for tree
    srand(time(nullptr));
    multimap<Variety, string> tree;

    //Setting up a random size for the multimap
    time_t randSize = (rand() % 100) + 1;
    for (time_t i = 0; i < randSize; ++i) //For loop that makes pair for all the varieties 
        tree.emplace(std::make_pair(static_cast<Variety>(rand() % 3), colors[rand() % 3]));


    //Finally, printing out the colors of the oranges
    cout << "Colors of the oranges: ";
    for (auto it = tree.lower_bound(Variety::orange); 
        it != tree.upper_bound(Variety::orange); ++it) 
        cout << it->second << ", ";
    cout << endl;
}
