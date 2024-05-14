#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9
#define NSDK 5

#define RESET        "\x1b[0m"

#define BLACK        "\x1b[30m"
#define RED          "\x1b[31m"
#define GREEN        "\x1b[32m"
#define YELLOW       "\x1b[33m"
#define BLUE         "\x1b[34m"
#define MAGENTA      "\x1b[35m"
#define CYAN         "\x1b[36m"
#define WHITE        "\x1b[37m"

char game[SIZE][SIZE];
char sdk[SIZE][SIZE];

int empty_cells = 0;

void init();
void print();
bool check();

int main(int argc, char const *argv[])
{
    
    srand(time(NULL));
    init();
    bool running = true;
    bool win = false;
    int x, y, val;

    while (running)
    {
        print();
        do{
            printf("\nInput X, Y and Value: ");
            scanf("%d %d %d", &x, &y, &val);
            if(sdk[y - 1][x - 1] == 'X' || y < 1 || y > 9 || x < 1 || x > 9 || val < 1 || val > 9)
                printf("\nError! Invalid input!\n");
        }while(sdk[y - 1][x - 1] == 'X' || y < 1 || y > 9 || x < 1 || x > 9 || val < 1 || val > 9);
        
        if(sdk[y - 1][x - 1] == 'O' && game[y - 1][x - 1] == '0')
            empty_cells--;
        
        game[y - 1][x - 1] = val + '0';

        printf("\nEmpty Cells: %d\n", empty_cells);
        
        if(empty_cells == 0)
        {
            running = false;
            win = check();
        }
    }
    
    if(win)
        printf("\nGood Job! You Won!\n");
    else
        printf("\nDamn, You Lost! Better Luck Next Time!\n");
    return 0;
}

void init()
{
    FILE *f;

    int row = rand() % NSDK;
    int count = 0;
    int k = 0;
    bool found = false;
    char sudoku[500];

    f = fopen("sudoku.txt", "r");

    do
    {
        fgets(sudoku, 500, f);
        if(count == row)
            found = true;
        else
            count++;
    } while (found == false);
    

    for(int i = 0; i < SIZE; i++)
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

    fclose(f);
}

void print()
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

bool check()
{
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            for(int k = 0; k < SIZE; k++)
            {
                if((game[k][j] == game[i][j] && k != i) || (game[i][k] == game[i][j] && k != j))
                    return false;
            }
            for(int k = i / 3 * 3; k < i / 3 * 3 + 3; k++)
            {
                for(int x = j / 3 * 3; x < j / 3 * 3; x++)
                {
                    if(game[k][x] == game[i][j] && k != i && x != j)
                        return false;
                }
            }
        }
    }

    return true;
}