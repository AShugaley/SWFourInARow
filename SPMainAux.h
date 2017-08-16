#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_
#include <stdbool.h>
#include "SPArrayList.h"
#include <stdbool.h>
#include "SPFIARGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>	
#include "SPMiniMax.h"
#include "SPFIARParser.h"
//put auxiliary functions and constants used by the main function here.

typedef enum game_status_t {
	RESTART,
	UNDO_SUCCESS,
	UNDO_FAIL,
	THERE_IS_A_WINNER,
	GAME_OVER_LOOP,
	PLAY_CONT
} GAME_STATUS;


/*
*starting a new game
*
*/
void init();

/*
*this is the game skeleton- create the game,
*then use the functions of user's and computer's turns, and then the function of gameOver
*@return the correct enum 
*@param - level that user chose, and a boolean that tells whether it's a new game or restart of an old game. 
*/
GAME_STATUS gameLoop(int difficLevel, bool createNewGame);

/*
*all the options of the user on his turn
*
*@return the correct enum 
*/
GAME_STATUS userTurn();

/*
*the computer's move according to the game level 
*
*
*/
void compTurn(int level);

/*
* destroy the game and exit
*
*/
void quit();


/*
* suggest the next move, using minimax calculate
*
*/
int suggestMove();

/**
* running the game - checking if there is a winner, and if not- running the user's or comuter's turns
*
*@param level - the game level that the user chose
*@ return the correct enum
*/
GAME_STATUS gameRun(int level);



/**
* Checks who won and print the right message
*

* @return
* null
*/
void winnerPrint();


/**
* when the game is over, the user can either press:
* restart (then return 1) - go back to the start, choose level and go on
* quit - terminate in that case
* undo_move- make the undo move
*
*@param game - the current game
* @return the game status
*
*/
GAME_STATUS gameOver();


/**
* when user pressed undo, check if its possible (of not - return 0),
* and if it's OK - do it twice
* exit if the undo is an invalid argument (NULL)
*
*@param game - the current game
*
*@return the correct enum
*
*/
GAME_STATUS undo();

/**
* when user pressed add disc,
* if it's OK - send to setMove
* exit if this is an invalid argument (NULL)
*
*
*@param game - the current game
*
*@return 1 if the add disc order successed
*@return 0 if the column is full or the number is not 1-7
*
*/
int addDisc(SPCommand command);


#endif
