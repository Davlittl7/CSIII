//Davonte Littleton
//CSIII
//28 October 2022

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl; using std::cin;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game {
public:
    Game() :playersCount_(0), movesCount_(0), playerWon_(noWinner) {}

    // template method
    void playGame(const int playersCount = 0) {
        playersCount_ = playersCount;
        movesCount_ = 0;

        initializeGame();

        for (int i = 0; !endOfGame(); i = (i + 1) % playersCount_) {
            makeMove(i);
            if (i == playersCount_ - 1)
                ++movesCount_;
        }
        printWinner();
    }

    virtual ~Game() {}

protected:
    // primitive operations
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner; } // this is a hook
                    // returns true if winner is decided
    static const int noWinner = -1;

    int playersCount_;
    int movesCount_;
    int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly : public Game {
public:
    // implementing concrete methods
    void initializeGame() {
        playersCount_ = rand() % numPlayers_ + 1; // initialize players
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
            if (chance < movesCount_) playerWon_ = player;
        }
    }

    void printWinner() {
        cout << "Monopoly, player " << playerWon_ << " won in "
            << movesCount_ << " moves." << endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20; // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess : public Game {
public:
    void initializeGame() {
        playersCount_ = numPlayers_; // initalize players
        for (int i = 0; i < numPlayers_; ++i)
            experience_[i] = rand() % maxExperience_ + 1;
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = (rand() % maxMoves_) / experience_[player];
            if (chance < movesCount_) playerWon_ = player;
        }
    }

    void printWinner() {
        cout << "Chess, player " << playerWon_
            << " with experience " << experience_[playerWon_]
            << " won in " << movesCount_ << " moves over"
            << " player with experience " << experience_[playerWon_ == 0 ? 1 : 0]
            << endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 2; // nobody wins before minMoves_
    static const int maxMoves_ = 100; // somebody wins before maxMoves_
    static const int maxExperience_ = 5; // player's experience
                               // the higher, the greater probability of winning
    int experience_[numPlayers_];
};

class Dice : public Game {
public:
    void initializeGame() {
        //Initializes a random seed 
        srand(time(nullptr));
        //Sets player count to 2 (player & computer)
        playersCount_ = numPlayers_;

        //Initializes player and computer rolls array to 0 for each element
        for (int i = 0; i < numOfDice_; ++i) {
            playerRolls_[i] = 0;
            computerRolls_[i] = 0;
        }
        cout << "It's time for the Dice Game!!!" << endl;
    }

    void makeMove(int player) {
        //Static int to count the amount of rounds that have taken place
        static int roundCount = 1;

        //Initializes random value for whether or not the computer will roll
        //If it isn't the first round
        if (roundCount != 1) computerChoice_ = rand() % 2;

        //Sets up the end of the game if the player & computer pass on rolling 
        if (computerChoice_ == 0 && playerChoice_ == 'n') {
            cout << "Both the computer and player have passed!" << endl;
            playerWon_ = player;
        }

        //If statements for if player and computer chooses yes to rolling
        if(playerChoice_ == 'y') playerSum_ = 0;

        if (computerChoice_ == 1) computerSum_ = 0;

        //Checks to see if the end game statement hasn't been met already
        if (playerWon_ != player) {
            //If statement that runs the rolling
            if (movesCount_ > minMoves_) {
                for (int i = 0; i < numOfDice_; ++i) {
                    //Will roll for player as long as playerChoice_ is yes(y)
                    if (playerChoice_ == 'y') {
                        playerRolls_[i] = rand() % 6 + 1;
                        playerSum_ += playerRolls_[i];
                    }

                    //Will roll for computer if choice is equal to 1 or it's the 
                    //first round.
                    if (computerChoice_ == 1) {
                        computerRolls_[i] = rand() % 6 + 1;
                        computerSum_ += computerRolls_[i];
                    }
                }
                //Outputs current round
                cout << "Round " << roundCount << "!" << endl;

                //Outputs the player's current roll if they rolled that round
                //And tells you if they didn't roll by saying they passed
                cout << "The player rolled: ";
                if (playerChoice_ == 'y')
                    for (int i = 0; i < numOfDice_; ++i) cout << playerRolls_[i] << " ";
                else
                    cout << "passed on rolling";
                cout << endl;

                //Outputs the computer's current roll if they rolled that round
                //And tells you if they didn't roll by saying they passed 
                cout << "The computer rolled: ";
                if (computerChoice_ == 1)
                    for (int i = 0; i < numOfDice_; ++i) cout << computerRolls_[i] << " ";
                else
                    cout << "passed on rolling";
                cout << endl;

                //Outputs the player's and computer's current score
                cout << "Player's current score: " << playerSum_ << endl;
                cout << "Computer's current score: " << computerSum_ << endl;


                //The program will ask the player if they want to roll or pass
                if (roundCount != maxRounds_) {
                    cout << "Does the player want to roll again [y/n] ";
                    cin >> playerChoice_;
                }
                
                //If it has reached 3 rounds, this sets up the end condition
                if (roundCount == maxRounds_) playerWon_ = player;

                //Increments the round count
                ++roundCount;

            }
        }
    }
    //Function to print winner based on who had the greater score
    void printWinner() {
        if (playerSum_ > computerSum_) cout << "You win!" << endl;
        else if (computerSum_ > playerSum_) cout << "Computer wins!" << endl;
        else cout << "It's a draw!" << endl;
        cout << endl;
    }
     
private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 3;
    const int numOfDice_ = 5, maxRounds_ = 3;
    int playerRolls_[5];
    int computerRolls_[5];
    int playerSum_, computerSum_, computerChoice_ = 1;
    char playerChoice_ = 'y';
};

int main() {
    srand(time(nullptr));

    Game* gp = nullptr;

    gp = new Dice;
    gp->playGame();
    delete gp;

}
