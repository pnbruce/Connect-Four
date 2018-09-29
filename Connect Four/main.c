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
void takeTurn(char board[rows][collumns], char player, int);
char switchPlayer(char);
char gameState(char board[rows][collumns]);
void winner(char board[rows][collumns], short*);
int AI(char board[rows][collumns], char,int );
int frees( char board[rows][collumns]);
void freesLocations(int*, char board[rows][collumns]);
void testPlace(char board[rows][collumns], char player, int placement);
int moveEval(char board[rows][collumns], char player, int depth, int maxDepth);
void printOutComes(int a[], int b[], int);
int newHighScore(int score);
void checkHighScoreFile(void);

int main()
{
    srand((unsigned)time(0));
    char startingPlayer;
    char player;
    char board[rows][collumns];
    short gameOn = 1;
    int maxDepth = 1;
    short valid;
    short playAgain = 1;
    char yesNo;
    int score = 0;
    
    //see if there is a high score file. if there isnt one it make one.
    checkHighScoreFile();
    
    
    printf("WELCOME TO CONNECT FOUR\n\n");
    
    while(playAgain)
        {
        if(rand()%2 == 0)
            startingPlayer = 'O';
        else
            startingPlayer = 'X';
        player = startingPlayer;
        valid = 0;
        
        int currentHighScore = 100;
        char currentUsername[3];
        FILE* highScores = fopen("HighScores.txt", "r");
        int idkWhyThisWorks;
        fscanf(highScores, "%d", &currentHighScore);
        idkWhyThisWorks = currentHighScore;
        fscanf(highScores, "%s", currentUsername);
        printf("Previous Best:\n%s won in just %d moves!\n\n", currentUsername, idkWhyThisWorks);
        fclose(highScores);
        
        while(!valid)
            {
            printf("Enter Dificulty Level (1-20):");
            scanf("%d", &maxDepth);
            if(maxDepth > 0 &&maxDepth <= 20 )
                valid = 1;
            else
                printf("Try again\n");
            }
        
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
            player = switchPlayer(player);
            takeTurn(board, player, maxDepth);
            drawBoard(board);
            winner(board, &gameOn);
            score++;
            }
        
        if(newHighScore(score) && player == 'X')
            {
            printf("YOU HAVE THE NEW BEST GAME!\nIt took %d to beat the machine.\n\n", score);
            char username[4];
            valid = 0;
            while (!valid)
                {
                    printf("Please enter a 3 letter user name:\n");
                    scanf("%s", &username);
                    printf(username);
                    if(username[3] == '\0')
                        {
                        valid = 1;
                        }
                    else
                        {
                        printf("Try again.\n\n");
                        }
                    }
            FILE* highScores;
            highScores = fopen("HighScores.txt", "w");
            fprintf(highScores, "%d ", score);
            fprintf(highScores, "%s", username);
            fclose(highScores);
            }
        
        
        valid = 0;
        while(!valid)
            {
            printf("\n\nWould you like to play again? (y/n):\n");
            scanf("%c", &yesNo);
            if(yesNo == 'n')
                playAgain = 0;
            if(yesNo == 'y')
                gameOn = 1;
            if(yesNo == 'n' || yesNo == 'y' )
                {
                valid = 1;
                printf("\n\n");
                }
            else
                printf("Try again\n");
            }
        }
    return 0;
}

//displayes the current state of the board.
void drawBoard(char board[rows][collumns])
{
    printf("\n\n\n\n\n\n\n  1   2   3   4   5   6   7\n");
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

void takeTurn(char board[rows][collumns], char player, int maxDepth)
{
    int placement = 0;
    short placed = 0;
    short legal = 0;
    
    while(!legal)
        {
        printf("Which column would you like to drop your piece down, %c?\n", player);
        if(player == 'X')
            {
            scanf("%d", &placement);
            placement--;
            }
        else
            placement = AI(board, player, maxDepth);
        
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
                            return board[i][k];
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
                                return board[t][k];
                            k++;
                            t++;
                            }
                        }
                    }
                if(i <rows - 3)
                    {
                    connected = 1;
                    k = j;
                    t = i;
                    //checks vertical win
                    while(board[t][k] == board[t+1][k] && t < rows-1)
                        {
                        connected++;
                        if(connected == 4)
                            return board[t][k];
                        t++;
                        }
                   if(j > 2)
                       {
                       connected = 1;
                       k = j;
                       t = i;
                       //checks left diagonal win
                       while(board[t][k] == board[t+1][k-1] && k >= 0 && t < rows-1)
                           {
                           connected++;
                           if(connected == 4)
                               return board[t][k];
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

int AI(char board[rows][collumns], char player, int maxDepth)
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
    int out;
    
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
            {
            outComes[i] = -10;
            }
        else if( gameState(boardCopy) == 'O')
            {
            printOutComes(outComes, freeSpaces, free);
            return freeSpaces[i];
            }
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
    out = outComes[0];
    int count = 0;
    int ties[free];
    for( int i = 0; i < free; i++)
        {
        if(outComes[i] == 10)
            return freeSpaces[i];
        if(out < outComes[i])
            {
            out = outComes[i];
            count = 0;
            }
        if(out == outComes[i])
            {
            ties[count] = i;
            count++;
            }
        }
    return freeSpaces[ties[rand()%count]];
}

int frees(char board[rows][collumns])
{
    int free = 0;
    
    for( int j = 0; j < collumns; j++)
        if(board[0][j] == ' ')
            free++;
    
    return free;
}

void freesLocations(int freeSpaces[], char board[rows][collumns])
{
    int* slider = freeSpaces;
    for( int j = 0; j < collumns; j++)
        if(board[0][j] == ' ')
            {
            *slider = j;
            slider++;
            }
    return;
}

void testPlace(char board[rows][collumns], char player, int placement)
{
    short placed = 0;
    if (placement >= collumns||placement < 0)
        drawBoard(board);
    for(int i = 0; !placed; i++)
        if(board[i+1][placement] != ' ')
            {
            placed = 1;
            board[i][placement] = player;
            }
}

int moveEval(char board[rows][collumns], char player, int depth, int maxDepth)
{
    if(depth >= maxDepth)
        return -1;
    char boardCopy[rows][collumns];
    int free = 0;
    for( int i = 0; i < rows; i++)
        for(int j = 0; j < collumns; j++)
            boardCopy[i][j] = board[i][j];
    free = frees(boardCopy);
    int freeSpaces[free];
    freesLocations(freeSpaces, boardCopy);
    int outComes[free];
    int goodness = 0;
    
    
    if(player == 'O')
        {
        for(int x = 0; x < free; x++)
            {
            player = 'O';
            
            for( int i = 0; i < rows; i++)
                for(int j = 0; j < collumns; j++)
                    boardCopy[i][j] = board[i][j];
            if(freeSpaces[x] >= collumns||freeSpaces[x] < 0)
                {
                for(int i = 0; i<=x; i++)
                    {
                    printf("free space issue:%d\n",freeSpaces[i]);
                    }
                }
            testPlace(boardCopy, player, freeSpaces[x]);
            if( gameState(boardCopy) == 'D')
                outComes[x] = 0;
            else if(gameState(boardCopy ) == 'X')
                {
                outComes[x] = -10;
                }
            else if( gameState(boardCopy) == 'O')
                return 10;
            else
                {
                player = 'X';
                depth++;
                outComes[x] = moveEval(boardCopy, player, depth, maxDepth);
                if(outComes[x] == 10)
                    return 10;
                }
            }
        
        for(int i = 0; i < free; i++)
            goodness += outComes[i];
        goodness = goodness/free;
        
        for(int i = 0; i < free; i++)
            if( outComes[i] == 10)
                return 10;
        
        return goodness;
        }
    else if(player == 'X')
        {
        for(int x = 0; x < free; x++)
            {
            player = 'X';
            for( int i = 0; i < rows; i++)
                for(int j = 0; j < collumns; j++)
                    boardCopy[i][j] = board[i][j];
            testPlace(boardCopy, player, freeSpaces[x]);
            if( gameState(boardCopy) == 'D')
                outComes[x] = 0;
            else if(gameState(boardCopy ) == 'X')
                return -10;
            else if( gameState(boardCopy) == 'O')
                {
                outComes[x] = 10;
                }
            else
                {
                player = 'O';
                depth++;
                outComes[x] = moveEval(boardCopy, player, depth, maxDepth);
                if(outComes[x] == -10)
                    return -10;
                }
            }
        
        for(int i = 0; i < free; i++)
            goodness += outComes[i];
        goodness = goodness/free;
        for(int i = 0; i < free; i++)
            if( outComes[i] == -10)
                return -10;
        if (goodness>10 || goodness < -10)
            
            printf("goodness problem %d, ", goodness);
        return goodness;
        }
    
    drawBoard(board);
    drawBoard(boardCopy);
    printf("there was a problem\n");
    
    return 0;
}

void printOutComes( int a[], int b[], int free ){
    int shift;
    
    printf( "\n" );
    for(int i = 0; i < free; i++){
        shift = b[i] + 1;
        printf("%d: %d\n", shift, a[i]);
    }
    printf("\n");
}

int newHighScore(int score)
{
    FILE* highScores;
    int currentHighScore;
    highScores = fopen("HighScores.txt", "r");
    fscanf(highScores, "%d", &currentHighScore);
    
    if(score > currentHighScore)
        {
        return 0;
        }
    else
        {
        return 1;
        }
}

void checkHighScoreFile()
{
    FILE* highScores;
    
    highScores = fopen("HighScores.txt", "r");
    if(highScores == NULL)
        {
        highScores = fopen("HighScores.txt", "w");
        fprintf(highScores, "%d ", 42);
        fprintf(highScores, "%s", "DEV");
        }
    fclose(highScores);
}
