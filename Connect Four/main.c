//
//  main.c
//  Connect Four
//
//  Created by Patrick Bruce on 4/6/18.
//  Copyright © 2018 Patrick Bruce. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rows = 6;
int collumns = 7;

void drawBoard(char board[rows][collumns]);
void takeTurn(char board[rows][collumns], char player);
char switchPlayer(char);
char gameState(char board[rows][collumns]);
void winner(char board[rows][collumns], short*);
int AI(char board[rows][collumns], char );
int frees( char board[rows][collumns]);
void freesLocations(int*, char board[rows][collumns]);
void testPlace(char board[rows][collumns], char player, int placement);
int moveEval(char board[rows][collumns], char player, int depth, int maxDepth);
void printOutComes(int a[], int b[], int);

int main()
{
    srand(time(0));
    char startingPlayer = 'O';
    char player = startingPlayer;
    char board[rows][collumns];
    short gameOn = 1;
    
    //populates an empty board
    for(int i = 0; i < rows; i++)
        {
        for(int j = 0; j < collumns; j++)
            {
            board[i][j] = ' ';
            }
        }
    
    drawBoard(board);

    while(gameOn)
        {
        takeTurn(board, player);
        drawBoard(board);
        player = switchPlayer(player);
        winner(board, &gameOn);
        }
    
    return 0;
}

//displayes the current state of the board.
void drawBoard(char board[rows][collumns])
{
    printf("\n\n\n\n\n\n\n  0   1   2   3   4   5   6\n");
    for(int i = 0; i < rows; i++)
        {
        printf("|");
        for(int j = 0; j < collumns; j++)
            {
                printf(" %c |",board[i][j]);
            }
        printf("\n-----------------------------\n");
        }
}

void takeTurn(char board[rows][collumns], char player)
{
    int placement = 0;
    short placed = 0;
    short legal = 0;
    
    while(!legal)
        {
        printf("Which column would you like to drop your piece down, %c?\n", player);
        if(player == 'X')
            scanf("%d", &placement);
        else
            placement = AI(board, player);
        
        if(board[0][placement] != ' ' || placement < 0 || placement > rows)
            printf("That is not a legal move!\n\n");
        else
            legal = 1;
        }
    
    for(int i = 0; !placed; i++)
        if(board[i+1][placement] != ' ')
            {
            placed = 1;
            board[i][placement] = player;
            }
    
}

//returns the opponent of the current player
char switchPlayer(char player)
{
    if(player == 'O')
        player ='X';
    else
        player = 'O';
    return player;
}

//returns the state of the board including winners, draws, and neither.
char gameState(char board[rows][collumns])
{
    int k = 0;
    int t = 0;
    int connected = 1;
    //checks for a draw
    if(frees(board) == 0)
        return 'D';
    
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < collumns; j++)
            {
            if(board[i][j] != ' ')
                {
                if(j < collumns - 3)
                    {
                    k = j;
                    connected = 1;
                    //checks horizontal win
                    while(board[i][k] == board[i][k+1] && k < collumns-1)
                        {
                        connected++;
                        if(connected == 4)
                            {
                            return board[i][k];
                            }
                        k++;
                        }
                    if(i < rows - 3)
                        {
                        k = j;
                        t = i;
                        connected = 1;
                        //checks diagonal righ win
                        while(board[t][k] == board[t+1][k+1]&& k < collumns-1 && t < rows-1)
                            {
                            connected++;
                            if(connected == 4)
                                {
                                return board[t][k];
                                }
                            k++;
                            t++;
                            }
                        }
                    }
                if(i < rows - 3)
                    {
                    connected = 1;
                    k = j;
                    t = i;
                    //checks vertical win
                    while(board[t][k] == board[t+1][k] && k < collumns-1 && t < rows-1)
                        {
                        connected++;
                        if(connected == 4)
                            {
                            return board[t][k];
                            }
                        t++;
                        }
                   if(j > 2)
                       {
                       connected = 1;
                       k = j;
                       t = i;
                       //checks left diagonal win
                       while(board[t][k] == board[t+1][k-1] && k < collumns-1 && t < rows-1)
                           {
                           connected++;
                           if(connected == 4)
                               {
                               return board[t][k];
                               }
                           t++;
                           k--;
                           }
                       }
                    }
                }
            }
    return 'N';
}

void winner(char board[rows][collumns], short* gameOnPtr)
{
    if(gameState(board) == 'D')
        {
        printf("ITS A DRAW! Close one!\n\n");
        *gameOnPtr = 0;
        return;
        }
    
    if(gameState(board) == 'X')
        {
        printf("X wins!\n\n");
        *gameOnPtr = 0;
        }

    if(gameState(board) == 'O')
        {
        printf("O wins!\n\n");
        *gameOnPtr = 0;
        }
}

int AI(char board[rows][collumns], char player)
{
    char boardCopy[rows][collumns];
    for( int q = 0; q < rows; q++)
        for(int j = 0; j < collumns; j++)
            boardCopy[q][j] = board[q][j];
    int free = 0;
    free = frees(board);
    int freeSpaces[free];
    freesLocations(freeSpaces, boardCopy);
    int outComes[free];
    int depth = 0;
    int maxDepth = 50;
    
    for(int i = 0; i < free; i++)
        {
        player ='O';
        for( int q = 0; q < rows; q++)
            for(int j = 0; j < collumns; j++)
                boardCopy[q][j] = board[q][j];
        testPlace(boardCopy, player, freeSpaces[i]);
        if( gameState(boardCopy) == 'D')
            outComes[i] = 0;
        else if(gameState(boardCopy ) == 'X')
            outComes[i] = -10;
        else if( gameState(boardCopy) == 'O')
            return freeSpaces[i];
        else
            {
            player = 'X';
            depth = 0;
            outComes[i] = moveEval(boardCopy, player, depth, maxDepth);
            if(outComes[i] == 10)
                {
                printOutComes(outComes, freeSpaces, free);
                return freeSpaces[i];
                }
            }
        }
    printOutComes(outComes, freeSpaces, free);
    for( int i = 0; i < free; i++)
        if(outComes[i] == 10)
            return freeSpaces[i];
    int count = 0;
    int ties[free];
    for( int i = 0; i < free; i++)
        if(outComes[i] == 0)
            {
            ties[count] = i;
            count++;
            }
    if (count)
        return freeSpaces[ties[rand()%count]];
    return freeSpaces[rand()%free];
    
}

int frees(char board[rows][collumns])
{
    int free = 0;
    
    for( int j = 0; j < collumns; j++)
        if((board[0][j] != 'X') && (board[0][j] != 'O'))
            free++;
    return free;
}

void freesLocations(int freeSpaces[], char board[rows][collumns])
{
    int* slider = freeSpaces;
    for( int j = 0; j < collumns; j++)
        if((board[0][j] != 'X') && (board[0][j] != 'O'))
            {
            *slider = j;
            slider++;
            }
    return;
}

void testPlace(char board[rows][collumns], char player, int placement)
{
    short placed = 0;
    for(int i = 0; !placed; i++)
        {
        if(board[i+1][placement] != ' ')
            {
            placed = 1;
            board[i][placement] = player;
            }
        }
}

int moveEval(char board[rows][collumns], char player, int depth, int maxDepth)
{
    if(depth >= maxDepth)
        return 0;
    char boardCopy[rows][collumns];
    int free = 0;
    for( int i = 0; i < rows; i++)
        for(int j = 0; j < collumns; j++)
            boardCopy[i][j] = board[i][j];
    free = frees(boardCopy);
    int freeSpaces[free];
    freesLocations(freeSpaces, boardCopy);
    int outComes[free];
    
    
    if(player == 'O')
        {
        for(int i = 0; i < free; i++)
            {
            player = 'O';
            for( int i = 0; i < rows; i++)
                for(int j = 0; j < collumns; j++)
                    boardCopy[i][j] = board[i][j];
            testPlace(boardCopy, player, freeSpaces[i]);
            
            if( gameState(boardCopy) == 'D')
                outComes[i] = 0;
            else if(gameState(boardCopy ) == 'X')
                outComes[i] = -10;
            else if( gameState(boardCopy) == 'O')
                return 10;
            else
                {
                player = 'X';
                depth++;
                outComes[i] = moveEval(boardCopy, player, depth, maxDepth);
                if(outComes[i] == 10)
                    return 10;
                }
            }
        for(int i = 0; i < free; i++)
            if( outComes[i] == 10)
                return 10;
        for(int i = 0; i < free; i++)
            if( outComes[i] == 0)
                return 0;
        for(int i = 0; i < free; i++)
            if( outComes[i] == -10)
                return -10;
        }
    else if(player == 'X')
        {
        for(int i = 0; i < free; i++)
            {
            player = 'X';
            for( int i = 0; i < rows; i++)
                for(int j = 0; j < collumns; j++)
                    boardCopy[i][j] = board[i][j];
            testPlace(boardCopy, player, freeSpaces[i]);
            if( gameState(boardCopy) == 'D')
                outComes[i] = 0;
            else if(gameState(boardCopy ) == 'X')
                return -10;
            else if( gameState(boardCopy) == 'O')
                outComes[i] = 10;
            else
                {
                player = 'O';
                depth++;
                outComes[i] = moveEval(boardCopy, player, depth, maxDepth);
                if(outComes[i] == -10)
                    {
                    return -10;
                    }
                }
            }
        for(int i = 0; i < free; i++)
            if( outComes[i] == -10)
                return -10;
        for(int i = 0; i < free; i++)
            if( outComes[i] == 0)
                return 0;
        for(int i = 0; i < free; i++)
            if( outComes[i] == 10)
                return 10;
        }
    drawBoard(board);
    drawBoard(boardCopy);
    printf("there was a problem\n");
    return 0;
}

void printOutComes( int a[], int b[], int free ){
    
    printf( "\n" );
    for(int i = 0; i < free; i++)
        printf("%d: %d\n", b[i], a[i]);
    printf("\n");
}
