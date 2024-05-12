#include <stdio.h>
#include <stdbool.h>

#define HEIGHT 6                                                        //game table height
#define WIDTH  7                                                        //game table width
#define WINCON 4                                                        //win condition (symbols in a row)

#define RESET        "\x1b[0m"                                      
#define RED          "\x1b[31m"
#define YELLOW       "\x1b[33m"
#define BLUE         "\x1b[34m"

char game[HEIGHT][WIDTH] = {' ',' ',' ',' ',' ',' ',' ',                //game table initialization
                            ' ',' ',' ',' ',' ',' ',' ',
                            ' ',' ',' ',' ',' ',' ',' ',
                            ' ',' ',' ',' ',' ',' ',' ',
                            ' ',' ',' ',' ',' ',' ',' ',
                            ' ',' ',' ',' ',' ',' ',' '};

int turn = 1;
int win = 0;

void print();                                                           //print game table
void edit(int column);                                                  //edit game table after player input
bool check(char c);                                                     //check if theres a win condition

int main(int argc, char const *argv[])
{
    bool running = true;
    int col;

    while(running)                                                      //game loop
    {
        print();
        do
        {
            printf("\nInput: ");
            scanf("%d", &col);
            if(col < 1 || col > 7 || game[0][col - 1] != ' ')
                printf("\nInvalid Input!\n");
        }
        while(col < 1 || col > 7 || game[0][col - 1] != ' ');           //column input loop until it's acceptable
        edit(col);                                                      //edit selected column (insert "coin")
        turn++;                                                         //turn increased
        if(check('X'))                                                  //check if either X or O has won or if there's a draw
        {
            running = false;
            win = 1;                                                    //win = 1 means X has won
        }
        else if(check('O'))
        {
            running = false;
            win = 2;                                                    //win = 2 means O has won
        }
        else if(turn == WIDTH * HEIGHT)                                 //if the table is full but no win condition has been detected there's a draw (win = 0)
            running = false;
    }

    print();                                                            //final print

    if(win == 0)                                                        //check who won
        printf("\nIt's a draw!\n");
    else if(win == 1)
        printf("\nX won!\n");
    else
        printf("\nO won!\n");

    printf("\nThanks for playing!");                                    //final message
    return 0;                                                           //end of the game
}

void print()                                                            //prints the table in blue color, the Xs are in red and the Os are in yellow
{
    int i, j;

    printf("\n");

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            if(game[i][j] == 'X')
                printf(BLUE "[" RED "%c" BLUE "]", game[i][j]);
            else if(game[i][j] == 'O')
                printf(BLUE "[" YELLOW "%c" BLUE "]", game[i][j]);
            else
                printf(BLUE "[%c]", game[i][j]);
        }
        printf("\n" RESET);
    }
}

void edit(int column)                                                   //checks the last non-empty cell in the selected column of the table and puts the symbol on top of it
{

    char c;
    int i;

    if(turn % 2 == 0)
        c = 'X';
    else
        c = 'O';

    for(i = 0; game[i+1][column - 1] == ' '; i++);
    game[i][column - 1] = c;
}

bool check(char c)                                                      //checks if there are win conditions in rows, columns or in oblique
{
    int i, j, k, count;

    for(i = 0; i < HEIGHT - 3; i++)                                     //this is for columns
    {
        for(j = 0; j < WIDTH; j++)
        {
            count = 0;
            for(k = 0; k < WINCON; k++)
            {
                if(game[i + k][j] == c)
                    count++;
            }
            if(count == 4)
                return true;
        }
    }

    for(i = 0; i < HEIGHT; i++)                                         //this is for rows
    {
        for(j = 0; j < WIDTH - 3; j++)
        {
            count = 0;
            for(k = 0; k < WINCON; k++)
            {
                if(game[i][j + k] == c)
                    count++;
            }
            if(count == 4)
                return true;
        }
    }

    for(i = 0; i < HEIGHT - 3; i++)                                     //this is for left half oblique win conditions
    {
        for(j = 0; j < WIDTH - 3; j++)
        {
            count = 0;
            for(k = 0; k < WINCON; k++)
            {
                if(game[i + k][j + k] == c)
                    count++;
            }
            if(count == 4)
                return true;
        }
    }

    for(i = 0; i < HEIGHT - 3; i++)                                     //this is for right half oblique win conditions
    {
        for(j = 3; j < WIDTH; j++)
        {
            count = 0;
            for(k = 0; k < WINCON; k++)
            {
                if(game[i + k][j - k] == c)
                    count++;
            }
            if(count == 4)
                return true;
        }
    }

    return false;                                                       //if in neither way a win condition was detected return false
}
