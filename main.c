#include <stdio.h>
#include <stdbool.h>

#define SIZE 9                                                                                          //table size

#define ANSI_RESET_ALL          "\x1b[0m"

#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_WHITE        "\x1b[37m"

char table[SIZE] = {'1','2','3','4','5','6','7','8','9'};                                               //game table

void print_table();                                                                                     //print game table
bool check_win(char c);                                                                                 //check if on the table there's a win

int main(int argc, char const *argv[])
{
    bool running = true;
    int input, turn = 1, reset;

    while(running)                                                                                      //game loop
    {
        print_table();

        do                                                                                              //check if input is valid
        {                                                                                               //if not, error message and try again
            printf("\nInput: ");
            scanf("%d", &input);
            if(input < 1 || input > 9 || table[input - 1] == 'X' || table[input - 1] == 'O')
                printf("\nError! Invalid Input.\n");
        }
        while(input < 1 || input > 9 || table[input - 1] == 'X' || table[input - 1] == 'O');

        if(turn % 2 == 0)                                                                               //if turn is odd -> X otherwise -> O
            table[input - 1] = 'O';
        else
            table[input - 1] = 'X';
        
        turn++;

        if(check_win('X'))                                                                              //check whether X or O has won
        {                                                                                               //or if it's a draw
            running = false;
            printf("\nX has won!\n");
        }
        else if(check_win('O'))
        {
            running = false;
            printf("\nO has won!\n");
        }else if(turn == 10)
        {
            running = false;
            printf("\nIt's a draw!\n");
        }

        if(!running)                                                                                    //if the game ended
        {                                                                                               //ask if you want to reset
            print_table();

            do                                                                                          //check for valid input
            {
                printf("\nDo you want to start over?\n" ANSI_COLOR_GREEN "  1. Yes\n  " ANSI_COLOR_RED "2. No\n" ANSI_COLOR_WHITE "Input: ");
                scanf("%d", &reset);    

                if(reset < 1 || reset > 2)
                    printf("\nError! Invalid Input.\n");
            } 
            while (reset < 1 || reset > 2);
            
            if(reset == 1)                                                                              //if player wanted to reset
            {                                                                                           //reset game variables
                running = true;
                turn = 1;
                for(int i = 0; i < SIZE; i++)
                    table[i] = i + '1';
            }
        }
    }

    printf("\nThanks for playing!\n" ANSI_RESET_ALL);                                                                  //end of the game

    return 0;
}

void print_table()
{
    printf("\n");

    for(int i = 0; i < SIZE; i++)                                                                       //print every value of the table
    {
        if(table[i] == 'X') printf(ANSI_COLOR_GREEN "[" ANSI_COLOR_RED "%c" ANSI_COLOR_GREEN "]", table[i]);
        else if(table[i] == 'O') printf(ANSI_COLOR_GREEN "[" ANSI_COLOR_BLUE "%c" ANSI_COLOR_GREEN "]", table[i]);
        else printf(ANSI_COLOR_GREEN "[" ANSI_COLOR_WHITE "%c" ANSI_COLOR_GREEN "]", table[i]);
        if((i+1)%3 == 0) printf("\n");                                                                  //if the value is on a %3 position
    }                                                                                                   //print '\n'
    printf(ANSI_COLOR_WHITE);
}


bool check_win(char c)                                                                                  //check if there's a tris of the char
{
    return (table[0] == c && table[1] == c && table[2] == c) 
    || (table[3] == c && table[4] == c && table[5] == c) 
    || (table[6] == c && table[7] == c && table[8] == c) 
    || (table[0] == c && table[3] == c && table[6] == c) 
    || (table[1] == c && table[4] == c && table[7] == c) 
    || (table[2] == c && table[5] == c && table[8] == c) 
    || (table[0] == c && table[4] == c && table[8] == c) 
    || (table[2] == c && table[4] == c && table[6] == c);
}