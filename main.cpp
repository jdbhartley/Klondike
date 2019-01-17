/* 
 * File:    main.cpp
 * Author:  James Hartley
 * Created: January 14, 2019, 5:28 PM
 * Purpose: Klondike, The classic Dice Game.
 */

#include <iostream>//IO Stream.
#include <iomanip> //IO Manipulation, for formatting
#include <fstream> //File Stream, for reading in the rules.
#include <cstdlib> //Random numbers
#include <ctime>   //Time library.
#include <cstring> //memcpy
#include <vector>  //using vectors instead of arrays for function types.
#include <string>  //For the to_string function
using namespace std;

//Structs
struct hand { char val; string desc; };

//Function Prototypes
string GetRules();
vector<char> RollDice();
hand CalcHand(vector<char>);

//Main Function
int main(int argc, char** argv) {
    //Set the random seed
    srand(time(0));

    //Variables
    hand        bHand,      //Banker Hand 
                pHand;      //Player Hand
    
    char        menuSel;    //Menu Selection   
    
    vector<char>    pDie,   //Player Dice.
                    bDie;   //Banker Dice.
    
    float       money,      //Amount of money the player has.
                betAmt;     //Amount of bet.
    
    bool        play;       //Are we currently playing the game?
    
    //Initialize
    play = false;
    money = 1000;
    
    //Intro
    cout << "*******************************" << endl;
    cout << "****        Klondike       ****" << endl;
    cout << "****     The Dice Game     ****" << endl;
    cout << "****          By           ****" << endl;
    cout << "****     James Hartley     ****" << endl;
    cout << "*******************************" << endl;
    cout << "         -------------         " << endl;
    
    //Display Menu
    cout << setfill('.');
    cout << "P" << setw(30) << "Play" << endl;
    cout << "R" << setw(30) << "Rules" << endl;
    cout << "E" << setw(30) << "Exit" << endl;
    cout << "Please input a selection..." << endl;
    cin >> menuSel;
    
    //Check menu selection
    switch (toupper(menuSel)) {
        case 'P':
            play = true;
            cout << fixed << setprecision(2);
            while (play && money > 0)
            {
                //Play the game
                cout << "         -------------         " << endl;
                cout << "Your Money: $" << money << endl;
                cout << "Enter your bet: $";
                cin >> betAmt;
                cout << "Banker Rolls" << endl;
                
                //Roll the dice for the banker
                bDie = RollDice();

                //Display dice roll for the banker
                for (int i = 0; i < 5; i++) {
                    cout << static_cast<short>(bDie[i]) << " "; 
                }
                
                //Calculate the banker hand value
                bHand = CalcHand(bDie);
                
                //Display hand description for the banker
                cout << bHand.desc;

                //Players Roll ---------------
                cout << "Player Rolls" << endl;
                
                //Roll the dice for the player
                pDie = RollDice();

                //Display dice roll for the player
                for (int i = 0; i < 5; i++) {
                    cout << static_cast<short>(pDie[i]) << " "; 
                }
                
                //Calculate player hand value
                pHand = CalcHand(pDie);
                
                //Display hand description
                cout << pHand.desc;

                //Determine winner
                if (pHand.val > bHand.val) { 
                    cout << "Player Wins!";
                    money += (betAmt*2);
                }
                else {
                    cout << "Banker Wins!";
                    money -= betAmt;
                }
                
                //Ask to play again
                if (money > 0) {
                    cout << endl << "Play Again? (Y or N) ";
                }
                else {
                    cout << endl << "Game Over! You ran out of money.";
                    play = false;
                }
                
                cin >> menuSel;
                if (toupper(menuSel) == 'N') {
                    play = false;
                }   
            }
            break;
        case 'R':
            //Display Rules
            cout << GetRules() << endl;
            break;
        case 'E':
            //Exit
            cout << "Goodbye!";
            break;
    }
    
    //Exit
    return 0;
}

hand CalcHand(vector<char> dice)
{
    hand result; //Resulting hand
    short value1, value2, pairs1, pairs2 = 0;

    //Check for pairs for the banker
    //This gruesome piece of code loops through the dice array to find multiples and stores their value for scoring.
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (dice[i] == dice[j]) {
                //We got a match
                if (value1 == 0 && value2 == 0) {
                    //This is the first pair found.
                    pairs1 = 1;
                    value1 = dice[i];
                    pairs1++;
                    dice[i] = -1; //Counted.
                } else if (value1 == dice[i]) {
                    //Same value of die
                    pairs1++;
                    dice[i] = -1; //Counted.
                } else if (value1 != 0 && value2 == 0) {
                    //Storing value 2 of die
                    value2 = dice[i];
                    pairs2 = 1;
                    pairs2++;
                    dice[i] = -1; //Counted.
                } else if (value1 != 0 && value2 == dice[i]) {
                    //Storing value 2 of die if multiples.
                    pairs2++;
                    dice[i] = -1; //Counted.
                }
            }
        }
    }
    if (pairs1 == 5 || pairs2 == 5) { //FIVE OF A KIND WORTH 60 POINTS
        //Set description
        result.desc = '\n' + string("Test") + string("Test") + std::to_string(pairs1);
        //result.desc = '\n' + string("Player has: ") + static_cast<string>(pairs1) + string(" ") + 
        //        static_cast<string>(value1) + string("'s.") + string('\n') + string("FIVE OF A KIND!") + string('\n');

        //Set hand worth
        result.val = 60 + value1 + value2;
    } else if (pairs1 == 4 || pairs2 == 4) { //FOUR OF A KIND WORTH 50 POINTS
        //Display output
        result.desc = '\n' + "Player has: " + static_cast<short>(pairs1) + " " +
                static_cast<short> (value1) + "'s." + '\n' + "FOUR OF A KIND!" + '\n';

        //Set hand worth
        result.val = 50 + value1 + value2;
    } else if (pairs1 == 3 && pairs2 == 2 || pairs2 == 3 && pairs1 == 2) { //FULL HOUSE WORTH 40 POINTS
        //Display output
        result.desc = '\n' + "Player has: " + static_cast<short> (pairs1) + " " +
                static_cast<short> (value1) + "'s and " + static_cast<short> (pairs2)
                + " " + static_cast<short> (value2) + "'s." + '\n' + "FULL HOUSE!" + '\n';

        //Set hand worth
        result.val = 40 + value1 + value2;
    } else if (pairs1 == 3 || pairs2 == 3) { //THREE OF A KIND WORTH 30 POINTS
        //Display output
        result.desc = '\n' + "Player has: " + static_cast<short> (pairs1) + " " +
                static_cast<short> (value1) + "'s." + '\n' + "THREE OF A KIND!" + '\n';

        //Set hand worth
        result.val = 30 + value1 + value2;
    } else if (pairs1 == 2 && pairs2 == 2) { //TWO PAIR WORTH 20 POINTS
        //Display output
        result.desc = '\n' + "Player has: " + static_cast<short> (pairs1) + " " +
                static_cast<short> (value1) + "'s and " + static_cast<short> (pairs2)
                + " " + static_cast<short> (value2) + "'s." + '\n' + "TWO PAIRS!" + '\n';

        //Set hand worth
        result.val = 20 + value1 + value2;
    } else if (pairs1 > 1 || pairs2 > 1) { //ONE PAIR WORTH 10 POINTS
        //Display output
        result.desc = '\n' + "Player has: " + static_cast<short> (pairs1) + " " +
                static_cast<short> (value1) + "'s." + '\n' + "ONE PAIR!" + '\n';

        //Set hand worth
        result.val = 10 + value1 + value2;
    } else {
        //Display output
        result.desc = '\n' + "Player has no pairs!" + '\n';

        //Set hand worth
        result.val = 0;
    }
    
    return result;
}


vector<char> RollDice()
{
    vector<char> result; //Resulting vector.
    for (int i = 0; i < 5; i++) {
        result.push_back(rand()%6+1);
    }
}

string GetRules()
{
    ifstream ifs;       //Input File stream.
    string temp;        //Temporary string for building result
    string result = ""; //Resulting string
    
    //Open the file stream
    ifs.open("rules.dat");
   
    //Loop through and add the text into the string.
    while (!ifs.eof()) {
        getline(ifs, temp);
        result += temp;
        result += '\n';
    }
    ifs.close();
    return result;
}

