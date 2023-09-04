# 🎲 Klondike: The Dice Game

This is a simple dice game where the player tries to score higher than the banker. The game is played with 5 dice.

![Klondike](screenshot.png)

## How to Play

1. The player rolls the dice for the banker.
2. The player rolls the dice for themselves.
3. The hands are calculated by finding the pairs of dice and adding their values together.
4. The winner is determined by the player with the higher hand.

## Code Flow

The code for this project is structured as follows:

* `main.cpp` is the main file that contains the game loop.
* `GetRules()` is the function that retrieves the rules of the game from the rules.dat file.
* `RollDice()` is the function that rolls the dice.
* `CalcHand()` is the function that calculates the hand for a player.
* `PlayGame()` is the function that plays a game of Klondike.

## Dependencies

On windows you can use Netbeans and Cygwin and utilize the included Netbeans project or check the How To Run section below for compiling on with visual studio.

If your on Linux odds are you don't need my help to compile this.

## How to Run

To run this project you can run make on linux or compile on windows using the visual studio command line.

#### Windows (from Visual Studio Command Prompt):

`cl /EHsc main.cpp /link /out:klondike.exe`

Then launch klondike.exe