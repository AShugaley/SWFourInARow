#include "SPFIARGame.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPMiniMaxNode.h"
#include <stdbool.h>
#include <limits.h>


SPFiarGame* spFiarGameCreate(int historySize)
{
    int i, j = 0;
    if (historySize <= 0)
        return NULL;
    
    SPFiarGame *gameSt = (SPFiarGame *)malloc(sizeof(SPFiarGame));
    if (gameSt == NULL)
        return NULL;
    
    for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
    {
        gameSt->tops[i] = 0;
    }
    
    for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
    {
        for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
        {
            gameSt->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
        }
    }
    gameSt->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    gameSt->historyArray = spArrayListCreate(historySize);
    
    return gameSt;
    
}


SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
    
    if (src == NULL || col >= SP_FIAR_GAME_N_COLUMNS || col < 0)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    
    if (!spFiarGameIsValidMove(src, col)) {
        //	printf("CANNOTDOMOVE\n");
        return SP_FIAR_GAME_INVALID_MOVE;
    }
    (src->gameBoard)[(src->tops)[col]][col] = src->currentPlayer;
    src->tops[col]++;
    if (spArrayListIsFull(src->historyArray)) {
        spArrayListRemoveFirst(src->historyArray);
    }
    spArrayListAddLast(src->historyArray, col);
    if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
        src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
    else
        src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    //  printf("DOMOVE\n");
    return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
    if (((src->tops)[col] > 5) || ((src->tops)[col] < 0)) {
        return false;
    }
    return true;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
    if ((src->currentPlayer != SP_FIAR_GAME_PLAYER_1_SYMBOL) && (src->currentPlayer != SP_FIAR_GAME_PLAYER_2_SYMBOL))
        return SP_FIAR_GAME_EMPTY_ENTRY;
    else return src->currentPlayer;
}


SPFiarGame* spFiarGameCopy(SPFiarGame* src)
{
    int i, j = 0;
    if (src == NULL)
        return NULL;
    
    SPFiarGame* gameStCopy = spFiarGameCreate(src->historyArray->maxSize);
    
    gameStCopy->currentPlayer = src->currentPlayer;
    
    for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++)
    {
        gameStCopy->tops[i] = src->tops[i];
    }
    
    for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++)
    {
        for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++)
        {
            gameStCopy->gameBoard[i][j] = src->gameBoard[i][j];
        }
    }
    
    return gameStCopy;
}


void spFiarGameDestroy(SPFiarGame* src)
{
    if (src != NULL)
    {
        spArrayListDestroy(src->historyArray);
        free(src);
    }
    
}



SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src)
{
    if (src == NULL)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    
    if (spArrayListIsEmpty(src->historyArray))		// size won't be > 20, covered in arraylist
        return SP_FIAR_GAME_NO_HISTORY;
    
    if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
        src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
    else
        src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    
    int curColumn = spArrayListGetLast(src->historyArray);
    spArrayListRemoveLast(src->historyArray);
    src->gameBoard[src->tops[curColumn] - 1][curColumn] = SP_FIAR_GAME_EMPTY_ENTRY;
    src->tops[curColumn] --;
    
    return SP_FIAR_GAME_SUCCESS;
}
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
    if (src == NULL)
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    for (int i = SP_FIAR_GAME_N_ROWS - 1; i>-1; i--) {
        printf("| ");
        for (int j = 0; j<SP_FIAR_GAME_N_COLUMNS; j++) {
            printf("%c ", src->gameBoard[i][j]);
        }
        printf("|\n");
    }
    printf("-----------------\n");
    printf("  1 2 3 4 5 6 7  \n");
    return SP_FIAR_GAME_SUCCESS;
    
}

/**
 * Checks if there's a winner in the specified game status. The function returns either
 * SP_FIAR_GAME_PLAYER_1_SYMBOL or SP_FIAR_GAME_PLAYER_2_SYMBOL in case there's a winner, where
 * the value returned is the symbol of the winner. If the game is over and there's a tie
 * then the value SP_FIAR_GAME_TIE_SYMBOL is returned. in any other case the null characters
 * is returned.
 * @param src - the source game
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 won
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 won
 * SP_FIAR_GAME_TIE_SYMBOL - If the game is over and there's a tie
 * null character - otherwise
 */
char spFiarCheckWinner(SPFiarGame* src) {
    int funcVal = comFunc(src);
    
    if (funcVal == INT_MAX)
        return SP_FIAR_GAME_PLAYER_1_SYMBOL;
    if (funcVal == INT_MIN)
        return SP_FIAR_GAME_PLAYER_2_SYMBOL;
    for (int i = 0; i<SP_FIAR_GAME_N_COLUMNS; i++) {
        if (src->tops[i] < SP_FIAR_GAME_N_ROWS - 1)
            return 0;
    }
    
    return SP_FIAR_GAME_TIE_SYMBOL;
}

