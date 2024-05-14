#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9                                                          //size of table
#define NSDK 5                                                          //number of possible sudokus in sudoku.txt

#define RESET        "\x1b[0m"                                          //various prompt colors

#define BLACK        "\x1b[30m"
#define RED          "\x1b[31m"
#define GREEN        "\x1b[32m"
#define YELLOW       "\x1b[33m"
#define BLUE         "\x1b[34m"
#define MAGENTA      "\x1b[35m"
#define CYAN         "\x1b[36m"
#define WHITE        "\x1b[37m"

char game[SIZE][SIZE];                                                  //game table
char sdk[SIZE][SIZE];                                                   //logic game table

int empty_cells = 0;                                                    //self explanatory

void init();                                                            //initialize game variables
void print();                                                           //prints the game table (and gui of the game)
bool check();                                                           //check if at the end of the game

int main(int argc, char const *argv[])
{
    
    srand(time(NULL));
    init();
    bool running = true;
    bool win = false;
    int x, y, val;                                                      //input x, y and value to store in the grid

    while (running)                                                     //game loop
    {
        print();
        do{
            printf("\nInput X, Y and Value: ");
            scanf("%d %d %d", &x, &y, &val);
            if(sdk[y - 1][x - 1] == 'X' || y < 1 || y > 9 || x < 1 || x > 9 || val < 1 || val > 9)
                printf("\nError! Invalid input!\n");
        }while(sdk[y - 1][x - 1] == 'X' || y < 1 || y > 9 || x < 1 || x > 9 || val < 1 || val > 9); //check for input mistakes
        
        if(sdk[y - 1][x - 1] == 'O' && game[y - 1][x - 1] == '0')   //if the input is "new" then decrease empty cells variable
            empty_cells--;
        
        game[y - 1][x - 1] = val + '0';                             //store value in the grid
        
        if(empty_cells == 0)                                        //if all cells are been filled then stop game loop and check if the player won
        {
            running = false;
            win = check();
        }
    }                                                               //game loop end
    
    if(win)                                                         //if the player won send good message, otherwise "bad" message
        printf("\nGood Job! You Won!\n");
    else
        printf("\nDamn, You Lost! Better Luck Next Time!\n");
    return 0;
}

void init()                                                                 //reads from sudoku.txt a random row that contains a sudoku
{                                                                           //then it stores it on the game table and then initializes the logic table          
    FILE *f;                                                                //file pointer

    int row = rand() % NSDK;
    int count = 0;
    int k = 0;
    bool found = false;
    char sudoku[500];

    f = fopen("sudoku.txt", "r");                                           //open file

    do
    {
        fgets(sudoku, 500, f);
        if(count == row)
            found = true;
        else
            count++;
    } while (found == false);                                               //loop that gets the wanted row determined by the random function
    

    for(int i = 0; i < SIZE; i++)                                           //game and logic grid and empty cells var initialization loop
    {
        for(int j = 0; j < SIZE; j++)
        {
            game[i][j] = sudoku[k];
            if(game[i][j] != '0')
                sdk[i][j] = 'X';
            else
            {
                sdk[i][j] = 'O';
                empty_cells++;
            }
            k++;
        }
    }

    fclose(f);                                                              //close file
}

void print()                                                                //game gui print
{
    int i, j;
    printf(YELLOW "    1  2  3   4  5  6   7  8  9\n");
    for(i = 0; i < SIZE; i++)
    {
        printf(YELLOW " %d " RESET, i+1);
        for(j = 0; j < SIZE; j++)
        {
            if(game[i][j] != '0' && sdk[i][j] == 'X')
                printf(BLUE "[" GREEN "%c" BLUE "]" RESET,  game[i][j]);
            else if (game[i][j] != '0' && sdk[i][j] == 'O')
                printf(BLUE "[" CYAN "%c" BLUE "]" RESET,  game[i][j]);
            else
                printf(BLUE "[ ]" RESET);
            if((j + 1) % 3 == 0 && (j + 1) != SIZE)
                printf(MAGENTA "|" RESET);
        }
        printf("\n");
        if((i + 1) % 3 == 0 && (i + 1) != SIZE)
            printf(MAGENTA "  ---------+---------+---------\n" RESET);
    }
}

bool check()                                                                    //for each cell checks for duplicates in its row, column and inner grid
{
    for(int i = 0; i < SIZE; i++)                                               //i for cells in the y axis
    {
        for(int j = 0; j < SIZE; j++)                                           //j for cells in the x axis
        {
            for(int k = 0; k < SIZE; k++)                                       //k to check rows and columns
            {
                if((game[k][j] == game[i][j] && k != i) || (game[i][k] == game[i][j] && k != j))
                    return false;
            }
            for(int k = i / 3 * 3; k < i / 3 * 3 + 3; k++)                      //k and j to do an inner loop inside the inner grid of the cell
            {
                for(int x = j / 3 * 3; x < j / 3 * 3; x++)
                {
                    if(game[k][x] == game[i][j] && k != i && x != j)
                        return false;
                }
            }                                                                   //if two equal cells in different positions where found
        }                                                                       //the function would have already returned false (meaning the use didnt win)
    }

    return true;                                                                //otherwise the function returns true
}