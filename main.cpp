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
using namespace std;

//Function Prototypes
string GetRules();
vector<char> RollDice();

//Structs
struct hand { char val; string desc; };

//Main Function
int main(int argc, char** argv) {
    //Set the random seed
    srand(time(0));

    //Variables
    hand        bHand,      //Banker Hand 
                pHand;      //Player Hand
    
    char        menuSel,    //Menu Selection   
                pPairs,     //Player pairs.
                pPairs2,
                pValue,     //Player pair dice value
                pValue2,    
                bPairs,     //Bankers pairs.
                bPairs2,
                pHand,      //Player Hand Value
                bValue,     //Banker pair dice value
                bValue2,
                bHand;      //Banker hand value
    
    //int         pDie[5], pDieT[5], //Dice and temp dice for player.
    //            bDie[5], bDieT[5]; //Dice and temp dice for banker.
    
    vector<char>    pDie,   //Player Dice.
                    bDie;   //Banker Dice.
    
    float       money,      //Amount of money the player has.
                betAmt;     //Amount of bet.
    
    bool        play;       //Are we currently playing the game?
    
    //Initialize
    play = false;
    money = 1000;
    bPairs = pPairs = pPairs2 = bPairs2 = bValue = bValue2 = pValue = pValue2 = pHand = bHand = 0;
    
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

                //Check for pairs for the banker
                memcpy(bDieT, bDie, sizeof(bDie)); //Copy the rolled array to the temp array so we can remove doubles as we go.
                //This gruesome piece of code loops through the dice array to find multiples and stores their value for scoring.
                for (int i = 0; i < 5; i++) {
                    for (int j = i + 1; j < 5; j++) {
                        if (bDieT[i] == bDieT[j]) {
                            //We got a match
                            if (bValue == 0 && bValue2 == 0) {
                                //This is the first pair found.
                                bPairs = 1;
                                bValue = bDieT[i];
                                bPairs++;
                                bDieT[i] = -1; //Counted.
                            }
                            else if (bValue == bDieT[i]) {
                                //Same value of die
                                bPairs++;
                                bDieT[i] = -1; //Counted.
                            }
                            else if (bValue != 0 && bValue2 == 0) {
                                //Storing value 2 of die
                                bValue2 = bDieT[i];
                                bPairs2 = 1;
                                bPairs2++;
                                bDieT[i] = -1; //Counted.
                            }
                            else if (bValue != 0 && bValue2 == bDieT[i]) {
                                //Storing value 2 of die if multiples.
                                bPairs2++;
                                bDieT[i] = -1; //Counted.
                            }
                        }
                    }
                }
                if (bPairs == 5 || bPairs2 == 5) { //FIVE OF A KIND WORTH 60 POINTS
                    //Display Output
                     cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                        static_cast<short>(bValue) << "'s." << endl << "FIVE OF A KIND!" << endl;

                     //Set hand worth
                     bHand = 60 + bValue + bValue2;
                }
                else if (bPairs == 4 || bPairs2 == 4) { //FOUR OF A KIND WORTH 50 POINTS
                    //Display output
                     cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                        static_cast<short>(bValue) << "'s." << endl << "FOUR OF A KIND!" << endl;

                     //Set hand worth
                     bHand = 50 + bValue + bValue2;
                }
                else if (bPairs == 3 && bPairs2 == 2 || bPairs2 == 3 && bPairs == 2) { //FULL HOUSE WORTH 40 POINTS
                    //Display output
                    cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                            static_cast<short>(bValue) << "'s and " << static_cast<short>(bPairs2) 
                            << " " << static_cast<short>(bValue2) << "'s." << endl << "FULL HOUSE!" << endl;

                    //Set hand worth
                    bHand = 40 + bValue + bValue2;
                }
                else if (bPairs == 3 || bPairs2 == 3) { //THREE OF A KIND WORTH 30 POINTS
                    //Display output
                     cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                        static_cast<short>(bValue) << "'s." << endl << "THREE OF A KIND!" << endl;

                     //Set hand worth
                     bHand = 30 + bValue + bValue2;
                }
                else if (bPairs == 2 && bPairs2 == 2) { //TWO PAIR WORTH 20 POINTS
                    //Display output
                    cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                        static_cast<short>(bValue) << "'s and " << static_cast<short>(bPairs2) 
                        << " " << static_cast<short>(bValue2) << "'s." << endl << "TWO PAIRS!" << endl;

                    //Set hand worth
                    bHand = 20 + bValue + bValue2;
                }
                else if (bPairs > 1 || bPairs2 > 1) { //ONE PAIR WORTH 10 POINTS
                    //Display output
                    cout << endl << "Banker has: " << static_cast<short>(bPairs) << " " << 
                        static_cast<short>(bValue) << "'s." << endl << "ONE PAIR!" << endl;

                    //Set hand worth
                    bHand = 10 + bValue + bValue2;
                }
                else {
                    //Display output
                    cout << endl << "Banker has no pairs!" << endl;

                    //Set hand worth
                    bHand = 0;
                }

                //Players Roll ---------------
                cout << "Player Rolls" << endl;
                //Roll the dice for the player
                for (int i = 0; i < 5; i++) {
                    pDie[i] = rand()%6+1;
                }

                //Display dice roll for the player
                for (int i = 0; i < 5; i++) {
                    cout << static_cast<short>(pDie[i]) << " "; 
                }

                //Check for pairs for the banker
                memcpy(pDieT, pDie, sizeof(pDie)); //Copy the rolled array to the temp array so we can remove doubles as we go.
                //This gruesome piece of code loops through the dice array to find multiples and stores their value for scoring.
                for (int i = 0; i < 5; i++) {
                    for (int j = i + 1; j < 5; j++) {
                        if (pDieT[i] == pDieT[j]) {
                            //We got a match
                            if (pValue == 0 && pValue2 == 0) {
                                //This is the first pair found.
                                pPairs = 1;
                                pValue = pDieT[i];
                                pPairs++;
                                pDieT[i] = -1; //Counted.
                            }
                            else if (pValue == pDieT[i]) {
                                //Same value of die
                                pPairs++;
                                pDieT[i] = -1; //Counted.
                            }
                            else if (pValue != 0 && pValue2 == 0) {
                                //Storing value 2 of die
                                pValue2 = bDieT[i];
                                pPairs2 = 1;
                                pPairs2++;
                                pDieT[i] = -1; //Counted.
                            }
                            else if (pValue != 0 && pValue2 == pDieT[i]) {
                                //Storing value 2 of die if multiples.
                                pPairs2++;
                                pDieT[i] = -1; //Counted.
                            }
                        }
                    }
                }
                if (pPairs == 5 || pPairs2 == 5) { //FIVE OF A KIND WORTH 60 POINTS
                    //Display Output
                     cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                        static_cast<short>(pValue) << "'s." << endl << "FIVE OF A KIND!" << endl;

                     //Set hand worth
                     pHand = 60 + pValue + pValue2;
                }
                else if (pPairs == 4 || pPairs2 == 4) { //FOUR OF A KIND WORTH 50 POINTS
                    //Display output
                     cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                        static_cast<short>(pValue) << "'s." << endl << "FOUR OF A KIND!" << endl;

                     //Set hand worth
                     pHand = 50 + pValue + pValue2;
                }
                else if (pPairs == 3 && pPairs2 == 2 || pPairs2 == 3 && pPairs == 2) { //FULL HOUSE WORTH 40 POINTS
                    //Display output
                    cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                            static_cast<short>(pValue) << "'s and " << static_cast<short>(pPairs2) 
                            << " " << static_cast<short>(pValue2) << "'s." << endl << "FULL HOUSE!" << endl;

                    //Set hand worth
                    pHand = 40 + pValue + pValue2;
                }
                else if (pPairs == 3 || pPairs2 == 3) { //THREE OF A KIND WORTH 30 POINTS
                    //Display output
                     cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                        static_cast<short>(pValue) << "'s." << endl << "THREE OF A KIND!" << endl;

                     //Set hand worth
                     pHand = 30 + pValue + pValue2;
                }
                else if (pPairs == 2 && pPairs2 == 2) { //TWO PAIR WORTH 20 POINTS
                    //Display output
                    cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                        static_cast<short>(pValue) << "'s and " << static_cast<short>(pPairs2) 
                        << " " << static_cast<short>(pValue2) << "'s." << endl << "TWO PAIRS!" << endl;

                    //Set hand worth
                    pHand = 20 + pValue + pValue2;
                }
                else if (pPairs > 1 || pPairs2 > 1) { //ONE PAIR WORTH 10 POINTS
                    //Display output
                    cout << endl << "Player has: " << static_cast<short>(pPairs) << " " << 
                        static_cast<short>(pValue) << "'s." << endl << "ONE PAIR!" << endl;

                    //Set hand worth
                    pHand = 10 + pValue + pValue2;
                }
                else {
                    //Display output
                    cout << endl << "Player has no pairs!" << endl;

                    //Set hand worth
                    pHand = 0;
                }

                //Determine winner
                if (pHand > bHand) { 
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

hand CalcHand()
{
    hand result; //Resulting hand
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

