#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include "SPFIARGame.h"

//Put all decleartions and constants here

/**
 * @param  t - char.
 * @return 1 if t == player_1_char, (-1) if t == player_2_char, or o otherwise.
 */
int charToVal(char t);

/**
 * Given a current game state, goes over the board game and updates the scoring vector
 * as per the current discs in the horizontal rows. e.g. vetcorsArray[0]++ if player_2
 * has 4 discs in a row in a horizontal lane.
 * @param resArray -the resultVectorArray, currentGame - game State
 */
void verticals(int resArray[9], SPFiarGame* currentGame);

/**
 * Given a current game state, goes over the board game and updates the scoring vector
 * as per the current discs in the vertical rows. e.g. vetcorsArray[0]++ if player_2
 * has 4 discs in a row in a vertical lane.
 * @param resArray -the resultVectorArray, currentGame - game State
 */
void horizontals(int resArray[9], SPFiarGame* currentGame);

/**
 * Given a current game state, goes over the board game and updates the scoring vector
 * as per the current discs in the diagunals (that go from the bottom left corner to 
 * the top right corner) rows. e.g. vetcorsArray[0]++ if player_2 has 4 discs in a row
 * in a diagunal.
 * @param resArray -the resultVectorArray, currentGame - game State
 */
void diagunalsLeftRight(int resArray[9], SPFiarGame* currentGame);

/**
 * Given a current game state, goes over the board game and updates the scoring vector
 * as per the current discs in the diagunals (that go from the bottom right corner to
 * the top left corner) rows. e.g. vetcorsArray[0]++ if player_2 has 4 discs in a row
 * in a diagunal.
 * @param resArray -the resultVectorArray, currentGame - game State
 */
void diagunalsRightRight(int resArray[9], SPFiarGame* currentGame);

/**
 * Given a current game state, computes the value of the scoring funcion
 * @param currentGame
 * @retrun socringFunctionValue
 */
int comFunc(SPFiarGame* currentGame);

/**
 * The function initilazes the MinMax tree. It then computes the minimal and maximal
 * potential values of the scoring function, and returns bestMoveMIN of bestMoveMax
 * according to the current player.
 *
 * @param currentGame
 * @retrun SuggestedMove
 */
int initMinmaxTree(SPFiarGame* currentGame, unsigned int maxDepth);

/**
 * The function is running the minmax tree - if we are at the desiered depth
 * and/or the game is over return the scoring function value
 * otherwise recursive call to the bottom nodes
 *
 * @param currentGame
 * @retrun maximal/minimal value of the scoring function
 */
int minmaxTree(SPFiarGame* currentGame, int depth, int alpha, int beta);

/**
 * return b if b > a, a otherwise
 */
int maxFunc(int a, int b);
/**
 * return b if b < a, a otherwise
 */
int minFunc(int a, int b);


#endif
