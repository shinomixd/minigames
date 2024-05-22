#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//parameters depending on difficulty selected

#define BEGINNER_HEIGHT  8
#define BEGINNER_WIDTH   8
#define BEGINNER_MINES  10

#define INTERMEDIATE_HEIGHT 16
#define INTERMEDIATE_WIDTH  16
#define INTERMEDIATE_MINES  40

#define EXPERT_WIDTH  30
#define EXPERT_HEIGHT 16
#define EXPERT_MINES  99

//colors for "ui"

#define RESET        "\x1b[0m"
#define BLACK        "\x1b[30m"
#define RED          "\x1b[31m"
#define GREEN        "\x1b[32m"
#define YELLOW       "\x1b[33m"
#define BLUE         "\x1b[34m"
#define MAGENTA      "\x1b[35m"
#define CYAN         "\x1b[36m"
#define WHITE        "\x1b[37m"

//functions prototypes

void init(char *** grid, int height, int width, int mines);                                                 //initialize grid
void print(char ** grid, int height, int width);                                                            //print game ui
void free_grid(char *** grid, int height);                                                                  //free memory allocated by grid
void init_clues(char *** grid, int height, int width);                                                      //insert clues of where to find mines
void clear_cells(char *** grid, char *** game, int height, int width, int x, int y);                        //functions to clear cells on coords given
bool check_win(char ** grid, char **game, int height, int width);                                           //check if the user has put flags on every mine and clicked every other cell

int main(int argc, char const *argv[])
{
    char ** grid;                                                                                           //logic grid
    char ** game;                                                                                           //game grid (what the user sees)
    bool running = true;                                                                                    //game loop boolean
    int diff;                                                                                               //select difficulty variable
    int todo;                                                                                               //select action variable

    int height, width, mines;                                                                               //grid parameters variables
    int x, y;                                                                                               //input coords variables
    bool win = true;                                                                                        //win boolean

    srand(time(NULL));                                                                                      //initialize random seed

    printf("\nWelcome to Minesweeper!\n  1. Beginner\n  2. Intermediate\n  3. Expert\n");                   //difficulty selector
    do
    {
        printf("\nPlease select difficulty: ");
        scanf("%d", &diff);
        if(diff < 1 || diff > 3)
            printf("\nError! Invalid Input!\n");
    }
    while(diff < 1 || diff > 3);

    if(diff == 1)                                                                                           //initialize grid parameters
    {
        height = BEGINNER_HEIGHT;
        width = BEGINNER_WIDTH;
        mines = BEGINNER_MINES;
    }
    else if(diff == 2)
    {
        height = INTERMEDIATE_HEIGHT;
        width = INTERMEDIATE_WIDTH;
        mines = INTERMEDIATE_MINES;
    }
    else
    {
        height = EXPERT_HEIGHT;
        width = EXPERT_WIDTH;
        mines = EXPERT_MINES;
    }

    init(&grid, height, width, mines);                                                                      //initialize grid
    init_clues(&grid, height, width);                                                                       //initialize clues
    init(&game, height, mines, 0);                                                                          //initialize game

    for(int i = 0; i < height; i++)                                                                         //create ui
        for(int j = 0; j < width; j++)
            game[i][j] = '#';

    printf("\n");

    while(running)                                                                                          //game loop
    {   
        print(game, height, width);                                                                 

        printf("\nWhat do you want to do?\n  1. Touch a Cell\n  2. Put a Flag\n  3. Remove a Flag\n");      //select action

        do
        {
            printf("\nInput: ");
            scanf("%d", &todo);
            if(todo < 1 || todo > 3)
                printf("\nError! Invalid Input!\n");
        }
        while(todo < 1 || todo > 3);

        do                                                                                                  //select coords to perform action
        {
            printf("\nInsert Coordinates: ");
            scanf("%d %d", &x, &y);
            if(x < 0 || x >= width || y < 0 || y >= height)
                printf("\nError! Invalid Input!\n");
            else
                if(game[y][x] > '0' && game[y][x] < '9')
                {    
                    y = -10;
                    printf("\nError! Invalid Input!\n");
                }
        }
        while(x < 0 || x >= width || y < 0 || y >= height);

        if(todo == 1)                                                                                       //click a cell
        {
            clear_cells(&grid,&game,height,width,x,y);                                                      //clear cells
            if(game[y][x] == 'M')                                                                           //if the cell was a mine
            {
                running = false;                                                                            //lost game
                win = false;                                                                                
            }        
        }
        else if(todo == 2)                                                                                  //put a flag
        {
            game[y][x] = 'F';                                                                           
            mines--;
        }
        else if(todo == 3)                                                                                  //remove a flag
        {
            game[y][x] = '#';                                                       
            mines++;
        }
        if(mines == 0 && check_win(grid, game, height, width))                                              //check win con
            running = false;
    }
    printf("\n");
    print(game, height, width);                                                                             //print last grid frame
    free_grid(&grid, height);                                                                               //free game and grid
    free_grid(&game, height);

    if(win)                                                                                                 //final message
    {
        printf("\nYou Won! Good Job!\n");
    }
    else
    {
        printf("\nYou Lost! Better Luck Next Time!\n");
    }

    return 0;                                                                                               //end
}

bool check_win(char ** grid, char ** game, int height, int width)   //check if there are cells to click, if none it means that the player won
{
    int i, j;

    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            if(game[i][j] == '#')
                return false;
    return true;
}

void init(char *** grid, int height, int width, int mines)          //dynamically allocate a matrix and randomly generate mines on it
{
    int i, j, x, y;

    *grid = (char**)malloc(height * sizeof(char*));
    for (i = 0; i < height; i++)
        (*grid)[i] = (char*)malloc(width * sizeof(char));

    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            (*grid)[i][j] = ' ';

    for(i = 0; i < mines;)
    {
        x = rand() % width;
        y = rand() % height;
        if((*grid)[y][x] != 'M')
        {
            (*grid)[y][x] = 'M';
            i++;
        }
    }
}

void print(char ** grid, int height, int width)                     //prints the ui of the game
{
    int i, j;
    printf(YELLOW "   ");
    for(i = 0; i < width; i++)
        if(i <= 9)
            printf(" %d ", (i));
        else
            printf(" %d", i);
    printf("\n" RESET);

    for(i = 0; i < height; i++)
    {
        if(i <= 9)
            printf(YELLOW " %d " RESET, (i));
        else
            printf(YELLOW "%d " RESET, (i));
        for(j = 0; j < width; j++)
        {
            if(grid[i][j] == 'M')
                printf(BLUE "[" RED "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] >= '1' && grid[i][j] <= '3')
                printf(BLUE "[" CYAN "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] >= '4' && grid[i][j] <= '6')
                printf(BLUE "[" GREEN "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] >= '7' && grid[i][j] < '9')
                printf(BLUE "[" MAGENTA "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] == '0')
                printf(BLUE "[" WHITE "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] == '#')
                printf(BLUE "[" YELLOW "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] == 'F')
                printf(BLUE "[" MAGENTA "%c" BLUE "]" RESET, grid[i][j]);
            else if(grid[i][j] == 'X')
                printf(BLUE "[" RED "%c" BLUE "]" RESET, grid[i][j]);
            else
                printf(BLUE "[ ]" RESET);
        }
        printf("\n");
    }
}

void free_grid(char *** grid, int height)      //frees the memory of a 2 dimensional array
{
    for(int i = 0; i < height; i++)
        free((*grid)[i]);
    free(*grid);
}

void init_clues(char *** grid, int height, int width)           //for every cells checks avaiable surrounding cells and counts how many mines are there
{                                                               //then it initializes the number on that cell if its not a mine
    int i, j, count;

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if((*grid)[i][j] != 'M')
            {
                count = 0;
                if(i == 0 && j == 0)
                {
                    if((*grid)[i][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j] == 'M') count++;
                }
                else if(i == height - 1 && j == width - 1)
                {
                    if((*grid)[i][j - 1] == 'M') count++;
                    if((*grid)[i - 1][j - 1] == 'M') count++;
                    if((*grid)[i - 1][j] == 'M') count++;
                }
                else if(i == 0 && j == width - 1)
                {
                    if((*grid)[i][j - 1] == 'M') count++;
                    if((*grid)[i + 1][j - 1] == 'M') count++;
                    if((*grid)[i][j] == 'M') count++;
                }
                else if(i == height - 1 && j == 0)
                {
                    if((*grid)[i - 1][j] == 'M') count++;
                    if((*grid)[i - 1][j + 1] == 'M') count++;
                    if((*grid)[i - 1][j] == 'M') count++;
                }
                else if(i == 0)
                {
                    if((*grid)[i][j - 1] == 'M') count++;
                    if((*grid)[i][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j - 1] == 'M') count++;
                    if((*grid)[i + 1][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j] == 'M') count++;
                }
                else if(i == height - 1)
                {
                    if((*grid)[i][j - 1] == 'M') count++;
                    if((*grid)[i][j + 1] == 'M') count++;
                    if((*grid)[i - 1][j - 1] == 'M') count++;
                    if((*grid)[i - 1][j + 1] == 'M') count++;
                    if((*grid)[i - 1][j] == 'M') count++;
                }
                else if(j == 0)
                {
                    if((*grid)[i - 1][j] == 'M') count++;
                    if((*grid)[i + 1][j] == 'M') count++;
                    if((*grid)[i - 1][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j + 1] == 'M') count++;
                    if((*grid)[i][j + 1] == 'M') count++;
                }
                else if(j == width - 1)
                {
                    if((*grid)[i - 1][j] == 'M') count++;
                    if((*grid)[i + 1][j] == 'M') count++;
                    if((*grid)[i - 1][j - 1] == 'M') count++;
                    if((*grid)[i + 1][j - 1] == 'M') count++;
                    if((*grid)[i][j - 1] == 'M') count++;
                }
                else
                {
                    if((*grid)[i - 1][j - 1] == 'M') count++;
                    if((*grid)[i - 1][j] == 'M') count++;
                    if((*grid)[i - 1][j + 1] == 'M') count++;
                    if((*grid)[i][j - 1] == 'M') count++;
                    if((*grid)[i][j + 1] == 'M') count++;
                    if((*grid)[i + 1][j - 1] == 'M') count++;
                    if((*grid)[i + 1][j] == 'M') count++;
                    if((*grid)[i + 1][j + 1] == 'M') count++;
                }
                (*grid)[i][j] = count + '0';
            }
        }
    }
}

void clear_cells(char *** grid, char *** game, int height, int width, int x, int y)     //with given coords it clears if the cell in grid is zero and then does the same on the above, left, right and under cells
{                                                                                       //if its a number greater than 0 and less than 9 it copies the value in game and then stops
    
    if((*grid)[y][x] != '0')
        (*game)[y][x] = (*grid)[y][x];
    else
    {
        (*game)[y][x] = ' ';
        (*grid)[y][x] = ' ';
        if(x > 0 && y > 0 && x < width - 1 && y < height - 1)
        {
            clear_cells(grid, game, height, width, x - 1, y);
            clear_cells(grid, game, height, width, x, y - 1);
            clear_cells(grid, game, height, width, x + 1, y);
            clear_cells(grid, game, height, width, x, y + 1);
        }
        else if(x == 0 && y == 0)
        {
            clear_cells(grid, game, height, width, x, y + 1);
            clear_cells(grid, game, height, width, x + 1, y);
        }
        else if(x == 0 && y == height - 1)
        {
            clear_cells(grid, game, height, width, x + 1, y);
            clear_cells(grid, game, height, width, x, y - 1);
        }
        else if(x == width - 1 && y == 0)
        {
            clear_cells(grid, game, height, width, x, y + 1);
            clear_cells(grid, game, height, width, x - 1, y);
        }
        else if(x == width - 1 && y == height - 1)
        {
            clear_cells(grid, game, height, width, x - 1, y);
            clear_cells(grid, game, height, width, x, y - 1);
        }
        else if(x == 0)
        {
            clear_cells(grid, game, height, width, x + 1, y);
            clear_cells(grid, game, height, width, x, y + 1);
            clear_cells(grid, game, height, width, x, y - 1);
        }
        else if(x == width - 1)
        {
            clear_cells(grid, game, height, width, x - 1, y);
            clear_cells(grid, game, height, width, x, y - 1);
            clear_cells(grid, game, height, width, x, y + 1);
        }
        else if(y == 0)
        {
            clear_cells(grid, game, height, width, x, y + 1);
            clear_cells(grid, game, height, width, x - 1, y);
            clear_cells(grid, game, height, width, x + 1, y);
        }
        else if(y == height - 1)
        {
            clear_cells(grid, game, height, width, x, y - 1);
            clear_cells(grid, game, height, width, x - 1, y);
            clear_cells(grid, game, height, width, x + 1, y);
        }
    }
}   