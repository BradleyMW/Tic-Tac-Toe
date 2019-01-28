//  Created by Bradley Wersterfer for Mac on 11/30/17.
//  Copyright © 2017 Bradley Wersterfer. All rights reserved.

#include <iostream>
#include <iomanip>
using namespace std;

void InitializeArray(char [][3], int, int, int [][3]);
void DisplayBoard(char [][3], int, int);
void PlayerTurn(char [][3], int, int, int, int [][3]);
bool validNum(string);
void EvaluateScore(int [][3], int, bool&);

int main(int argc, const char * argv[]) {
    
    //The number of rows and columns will always be 3.
    const int ROWS = 3,
              COLS = 3;
    
    //The number of turns taken and whether a player has won yet are declared here.
    int turns = 0;
    bool won = false;
    
    //These are the arrays representing the playing field and keeping track of scoring.
    char gameBoard[ROWS][COLS];
    int scoreArray[ROWS][COLS];
    
    //Calls the functions to initialize and display the game board.
    InitializeArray(gameBoard, ROWS, COLS, scoreArray);
    
    //Loops through the players' turns until either someone has won or there are no spaces left.
    while (turns < 9 && !won)
    {
        //Calls the function to take player 1's turn and increments the turns.
        PlayerTurn(gameBoard, ROWS, COLS, 1, scoreArray);
        turns++;
        
        //Checks to see if the first player just won.
        EvaluateScore(scoreArray, turns, won);
        
        //If the game is not over, then player 2 can take a turn.
        if(turns < 9 && !won)
        {
            //Calls the function to take player 2's turn and increments the tries.
            PlayerTurn(gameBoard, ROWS, COLS, 2, scoreArray);
            turns++;
            
            //Calls the function to evaluate the scoring array.
            EvaluateScore(scoreArray, turns, won);
        }
    }
    
    //Displays the final results and terminates the program.
    DisplayBoard(gameBoard, ROWS, COLS);
    return 0;
}

//This function sets up the display board and hidden scoring array for the game.
void InitializeArray(char gameBoard[][3], int rows, int cols, int scoreArray[][3])
{
    //Moves through each row of the display array.
    for(int i = 0; i < rows; i++)
    {
        //Moves through each element in the current row.
        for(int j = 0; j < cols; j++)
        {
            //Initializes the board to asterisks and the numerical representation to 0s.
            gameBoard[i][j] = '*';
            scoreArray[i][j] = 0;
        }
    }
}

//This function shows the users the current playing field.
void DisplayBoard(char gameBoard[][3], int rows, int cols)
{
    //Displays the header of columns.
    cout << right << setw(16) << "Column 1";
    cout << setw(11) << "Column 2";
    cout << setw(11) << "Column 3" << endl;
    
    //Loops through every row after the header.
    for(int i = 0; i < rows; i++)
    {
        //Displays the row numbers and then the proceeding row.
        cout << "Row " << (i + 1);
        for(int j = 0; j < cols; j++)
        {
            //Evenly spaces the game elements.
            cout << setw(11) << gameBoard[i][j];
        }
        
        //Moves on to the next line for the following row.
        cout << endl;
    }
}

//This function lets either player take their turn.
void PlayerTurn(char gameBoard[][3], int rows, int cols, int player, int scoreArray[][3])
{
    //Displays the board for the player.
    DisplayBoard(gameBoard, rows, cols);
    
    //The temporary variables for the player's selections.
    int playRow = 0,
        playCol = 0;
    
    string input;
    
    //Loops until the player chooses a valid space on the grid.
    do
    {
        //Loops until a valid row is entered.
        do
        {
            //Lets the player select a row from the game board.
            cout << "Player " << player << ", choose a row: ";
            getline(cin, input);
            if(!validNum(input))
            {
                //Notifies the user that their input was not valid.
                cout << "Error: Please enter a valid row number." << endl;
            }
            else
                playRow = stoi(input);
        } //Loop ends once playRow has been successfully set.
        while (playRow == 0);
        
        //Loops until a valid column is entered.
        do
        {
            //Lets the player select a column from the game board.
            cout << "Player " << player << ", choose a column: ";
            getline(cin, input);
            if(!validNum(input))
            {
                //Notifies the user that their input was not valid.
                cout << "Error: Please enter a valid column number." << endl;
            }
            else
                playCol = stoi(input);
        } //Loop ends once playCol has been successfully set.
        while (playCol == 0);
        
        //Displays an error message if the space has already been used.
        if(scoreArray[playRow - 1][playCol - 1] != 0)
            cout << "Error: That space has already been taken." << endl;
        
    } //Loop ends once an empty space has been selected.
    while(scoreArray[playRow - 1][playCol - 1] != 0);
    
    //Determines whether the player was 1 or 2 for display purposes.
    if(player == 1)
    {
        //Sets the appropriate space to an X for player 1, taking into account the index.
        gameBoard[playRow - 1][playCol - 1] = 'X';
        
        //In order to keep track of which spaces have been taken and which player will win, a second array of hidden numbers is kept. Player 1 uses the number 1, and must get a sum of 3 in any combination. This number was chosen because 1 and 4 (Player 2's number) cannot be combined to get either sum.
        scoreArray[playRow - 1][playCol - 1] = 1;
    }
    else
    {
        //Sets the appropriate space to an X for player 2, taking into account the index.
        gameBoard[playRow - 1][playCol - 1] = 'O';
        
        //In order to keep track of which spaces have been taken and which player will win, a second array of hidden numbers is kept. Player 2 uses the number 4, and must get a sum of 12 in any combination. This number was chosen because 1 (Player 1's number) and 4 cannot be combined to get either sum.
        scoreArray[playRow - 1][playCol - 1] = 4;
    }
    
    //Skips a line to make the output easier to read.
    cout << endl;
}

//Verifies that the given input is a single digit between 1 and 3.
bool validNum(string input)
{
    //If the length is not 1, then input cannot be valid.
    if(input.length() != 1)
        return false;
    
    //If the character that was entered is not a digit between 1 and 3, then it is not valid.
    if(input[0] < '1' || input[0] > '3')
        return false;
    
    //If no errors were found, then the input was valid.
    return true;
}

//This function runs through the scoring board to see if any winning combinations were made.
void EvaluateScore(int scoreArray[][3], int turns, bool& won)
{
    //Keeps track of possible winning combinations in an array.
    int possibleWins[8];
    bool player1Won = false,
         player2Won = false;
    
    //Runs through each row to check the final sums.
    for(int row = 0; row < 3 && !won; row++)
    {
        //The first three values are reserved for row sums.
        possibleWins[row] = scoreArray[row][0] + scoreArray[row][1] + scoreArray[row][2];
        
        //If a winning combo was found, then the loop ends.
        if(possibleWins[row] == 3 || possibleWins[row] == 12)
            won = true;
    }
    
    //Runs through each column to check the final sums.
    for(int col = 3; col < 6 && !won; col++)
    {
        //The second set of three values are reserved for column sums.
        possibleWins[col] = scoreArray[0][col] + scoreArray[1][col] + scoreArray[2][col];
        
        //If a winning combo was found, then the loop ends.
        if(possibleWins[col] == 3 || possibleWins[col] == 12)
            won = true;
    }
    
    //Runs through both diagonals to check for winning sums.
    if(!won)
    {
        //Checks the first diagonal for a winning combination.
        possibleWins[6] = scoreArray[0][0] + scoreArray[1][1] + scoreArray[2][2];
        if(possibleWins[6] == 3 || possibleWins[6] == 12)
            won = true;
        
        //Checks the second diagonal for a winning combination.
        possibleWins[7] = scoreArray[2][0] + scoreArray[1][1] + scoreArray[0][2];
        if(possibleWins[7] == 3 || possibleWins[7] == 12)
            won = true;
    }
    
    //If a player has won, then the winning combinations are checked to see who won.
    for(int i = 0; i < 8 && won; i++)
    {
        if(possibleWins[i] == 3)
            player1Won = true;
        if(possibleWins[i] == 12)
            player2Won = true;
    }
    
    //Checks to see if any patterns came out to 3, Player 1's winning sum.
    if(player1Won)
    {
        //Acknowledges that a player just won.
        cout << "Congratulations, Player 1! You have won!" << endl;
    }
    //Checks to see if any patterns came out to 12, Player 2's winning sum.
    else if(player2Won)
    {
        //Acknowledges that a player just won.
        cout << "Congratulations, Player 2! You have won!" << endl;
    }
    //If no one has won and all turns have been expended, it must be a tie.
    else if(turns >= 9)
    {
        cout << "It was a tie!" << endl;
    }
}
