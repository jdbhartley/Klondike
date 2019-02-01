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
#include <limits>  //For checking user input
using namespace std;

//Structs
struct hand { char val; string desc; };

//Function Prototypes
string GetRules();
vector<char> RollDice();
hand CalcHand(string, vector<char>);
void PlayGame(string, float &, float &, vector<char> &, vector<char> &, hand &, hand &);
bool CheckForSave();
void LoadGame(string &, float &) ;
void SaveGame(string, float);
void DisplayDice(vector<char> &);

//Main Function
int main(int argc, char** argv) {
    //Set the random seed
    srand(time(0));

    //Variables
    hand        bHand,      //Banker Hand
                pHand;      //Player Hand
    
    char        menuSel;    //Menu Selection   
    
    bool        play;       //Are we currently playing the game?
    
    string      name;       //Players name
    
    float       money,      //Amount of money the player has.
                betAmt;     //Amount of bet.
    
    vector<char>    pDie,   //Player Dice.
                    bDie;   //Banker Dice.
    
    
    
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
    do {  
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
                //Check if we have a valid save file
                if (CheckForSave()) {
                    LoadGame(name, money) ;
                }
                else {
                    //Get player name and create save file.
                    cout << "Please enter your name: ";
                    cin >> name;
                    SaveGame(name, money);
                }
                while (play && money > 0)
                {
                    //Play the game
                    PlayGame(name, money, betAmt, bDie, pDie, pHand, bHand);

                    //Ask to play again
                    if (money > 0) {
                        cout << endl << "Play Again? (Y or N) ";
                    }
                    else {
                        cout << endl << "Game Over! You ran out of money.";
                        cout << endl << "Would you like to restart?";
                    }

                    cin >> menuSel;
                    if (toupper(menuSel) == 'N') {
                        play = false;
                        SaveGame(name, money);
                    }
                    else if (money <= 0) {
                        money = 1000;
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
    }while (toupper(menuSel) == 'R');
    
    //Exit
    return 0;
}

hand CalcHand(string name, vector<char> dice)
{
    hand result; //Resulting hand
    char value1, value2, pairs1, pairs2;
    value1 = value2 = pairs1 = pairs2 = 0;

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
        result.desc = "FIVE OF A KIND!";

        //Set hand worth
        result.val = 60 + value1 + value2;
    } else if (pairs1 == 4 || pairs2 == 4) { //FOUR OF A KIND WORTH 50 POINTS
        //Display output
        result.desc.append(name);
        result.desc.append(" has: ");
        result.desc.append(to_string(pairs1)); 
        result.desc.append(" ");
        result.desc.append(to_string(value1));
        result.desc.append("'s.");
        result.desc.append("\nFOUR OF A KIND!\n");
        
        //Set hand worth
        result.val = 50 + value1 + value2;
        //1's are greater value
        if (value1 == 1 || value2 == 1) {
            result.val += 5;
        }
    } else if (pairs1 == 3 && pairs2 == 2 || pairs2 == 3 && pairs1 == 2) { //FULL HOUSE WORTH 40 POINTS
        //Display output
        result.desc.append(name);
        result.desc.append(" has: ");
        result.desc.append(to_string(pairs1)); 
        result.desc.append(" ");
        result.desc.append(to_string(value1));
        result.desc.append("'s. and ");
        result.desc.append(to_string(pairs2));
        result.desc.append(" ");
        result.desc.append(to_string(value2));
        result.desc.append("'s.");
        result.desc.append("\nFULL HOUSE!\n");
        
        //Set hand worth
        result.val = 40 + value1 + value2;
    } else if (pairs1 == 3 || pairs2 == 3) { //THREE OF A KIND WORTH 30 POINTS
        //Display output
        result.desc.append(name);
        result.desc.append(" has: ");
        result.desc.append(to_string(pairs1)); 
        result.desc.append(" ");
        result.desc.append(to_string(value1));
        result.desc.append("'s.");
        result.desc.append("\nTHREE OF A KIND!\n");
        //Set hand worth
        result.val = 30 + value1 + value2;
        //1's are greater value
        if (value1 == 1 || value2 == 1) {
            result.val += 5;
        }
    } else if (pairs1 == 2 && pairs2 == 2) { //TWO PAIR WORTH 20 POINTS
        //Display output
        result.desc.append(name);
        result.desc.append(" has: ");
        result.desc.append(to_string(pairs1)); 
        result.desc.append(" ");
        result.desc.append(to_string(value1));
        result.desc.append("'s. and ");
        result.desc.append(to_string(pairs2));
        result.desc.append(" ");
        result.desc.append(to_string(value2));
        result.desc.append("'s.");
        result.desc.append("\nTWO PAIRS!\n");
        //Set hand worth
        result.val = 20 + value1 + value2;
        if (value1 == 1 || value2 == 1) {
            result.val += 5;
        }
    } else if (pairs1 > 1 || pairs2 > 1) { //ONE PAIR WORTH 10 POINTS
        //Display output
        //Display output
        result.desc.append(name);
        result.desc.append(" has: ");
        result.desc.append(to_string(pairs1)); 
        result.desc.append(" ");
        result.desc.append(to_string(value1));
        result.desc.append("'s.");
        result.desc.append("\nONE PAIR!\n");
        //Set hand worth
        result.val = 10 + value1 + value2;
        if (value1 == 1 || value2 == 1) {
            result.val += 5;
        }
    } else {
        result.desc = name + " has no pairs.";
        //Set hand worth
        result.val = 0;
    }
    
    return result;
}

void PlayGame(  string name, float &money, float &betAmt, vector<char> &bDie, 
                vector<char> &pDie, hand &pHand, hand &bHand    ) 
{
    //Play the game
                cout << "         -------------         " << endl;
                cout << "Your Money: $" << money << endl;
                cout << "Enter your bet: $";
                cin >> betAmt;
                //Check for correct value
                while(!cin || betAmt <= 0)
                    {
                        // user didn't input a number
                        cin.clear(); // reset failbit
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
                        // next, request user reinput
                        cout << "You must enter a positive number for your bet" << endl;
                        cout << "Enter your bet: $";
                        cin >> betAmt;
                    }
                cout << "Banker Rolls" << endl;
                
                //Roll the dice for the banker
                bDie = RollDice();
                
                //Sort and Display the dice
                DisplayDice(bDie);

                //Calculate the banker hand value
                bHand = CalcHand("Banker", bDie);
                
                //Display hand description for the banker
                cout << endl << bHand.desc << endl;

                //Players Roll ---------------
                cout << name << " Rolls" << endl;
                
                //Roll the dice for the player
                pDie = RollDice();

                //Sort and Display the dice
                DisplayDice(pDie);
                
                //Calculate player hand value
                pHand = CalcHand(name, pDie);
                
                //Display hand description
                cout << endl << pHand.desc << endl;

                //Determine winner
                if (pHand.val > bHand.val) { 
                    cout << name << " Wins!";
                    money += (betAmt*2);
                }
                else if (pHand.val == bHand.val) {
                    cout << "It's a Tie!";
                }
                else {
                    cout << "Banker Wins.";
                    money -= betAmt;
                }
}


vector<char> RollDice()
{
    //This rolls the dice and returns a vector containing the roll
    vector<char> result; //Resulting vector.
    for (int i = 0; i < 5; i++) {
        result.push_back(rand()%6+1);
    }
    return result;
}

void DisplayDice(vector<char> &dice) {
    //Marksort
    for (int i = 0; i < 5; i++) {
        for (int j = i+1; j < 5; j++) {
            if (dice[j] < dice[i]) {
                //Swap
                char tmp = dice[i];
                dice[i] = dice[j];
                dice[j] = tmp;
            }
        }
    }

    //Display dice roll for the banker
    for (int i = 0; i < 5; i++) {
        cout << static_cast<short> (dice[i]) << " ";
    }
}

void SaveGame(string name, float money)
{
    ofstream ofs;       //Output File stream.
    
    //Open the file stream
    ofs.open("save.dat");
   
    //Save Player name and money
    ofs << 1 << '\n' << name << '\n' << money;

    //close the file stream
    ofs.close();
}

bool CheckForSave() {
    ifstream ifs;       //Input File stream.
    string tmpSave;
    bool save; //0/false for new game, 1/true for saved game.
    //Open the file stream
    ifs.open("save.dat");
   
    //Get player name from line 1
    getline(ifs, tmpSave);
    save = stoi(tmpSave);
    
    //close the file stream
    ifs.close();
    
    if (save != 0) {
        cout << "----Found Save!---";
        return true;
    }
    else {
        return false;
    }
}

void LoadGame(string &name, float &money) 
{
    ifstream ifs;       //Input File stream.
    string saveSt;
    string tmpName;
    string tmpMon;
    
    //Open the file stream
    ifs.open("save.dat");
   
    //Get player name from line 1 and player money from line 2
    getline(ifs, saveSt);
    getline(ifs, tmpName);
    getline(ifs, tmpMon);
    
    //Assign the values to the vars
    name = tmpName;
    money = atof(tmpMon.c_str());
    
    //close the file stream
    ifs.close();
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
    
    //close the file stream
    ifs.close();
    
    //return the rules string
    return result;
}

