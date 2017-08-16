#include <stdio.h>
#include <limits.h>
#include "SPFIARGame.h"
#include "SPMiniMaxNode.h"


int charToVal(char t){
    switch (t)
    {
        case SP_FIAR_GAME_PLAYER_1_SYMBOL:
            return 1;
            break;
        case SP_FIAR_GAME_PLAYER_2_SYMBOL:
            return -1;
            break;
        default:
            return 0;
    }
}

void horizontals(int resArray[9], SPFiarGame* currentGame){
    for(int i = 0; i<6; i++){
        int span = charToVal((currentGame->gameBoard)[i][0]) + charToVal((currentGame->gameBoard)[i][1]) + charToVal((currentGame->gameBoard)[i][2]);
        for(int j = 3; j<7; j++){
            span += charToVal((currentGame->gameBoard)[i][j]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[i][j-3]);
        }
    }
}

void verticals(int resArray[9], SPFiarGame* currentGame){
    for(int j = 0; j<7; j++){
        int span = charToVal((currentGame->gameBoard)[0][j]) + charToVal((currentGame->gameBoard)[1][j]) + charToVal((currentGame->gameBoard)[2][j]);
        for(int i = 3; i<6; i++){
            span += charToVal((currentGame->gameBoard)[i][j]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[i-3][j]);
        }
    }
    
}

void diagunalsLeftRight(int resArray[9], SPFiarGame* currentGame){
    for(int i = 3; i<6; i++){
        int span = charToVal((currentGame->gameBoard)[i][0]) + charToVal((currentGame->gameBoard)[i-1][1]) + charToVal((currentGame->gameBoard)[i-2][2]);
        for (int k = i-3; k > -1; k--){
            int j = i-k;
            span += charToVal((currentGame->gameBoard)[k][j]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[k+3][j-3]);
        }
    }
    
    for(int i = 6; i<9; i++){
        int span = charToVal((currentGame->gameBoard)[5][i-5]) + charToVal((currentGame->gameBoard)[4][i-5+1]) + charToVal((currentGame->gameBoard)[3][i-5+2]);
        for (int j = i-2; j < 7; j++){
            int k = i-j;
            span += charToVal((currentGame->gameBoard)[k][j]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[k+3][j-3]);
        }
    }
}

void diagunalsRightLeft(int resArray[9], SPFiarGame* currentGame){
    for (int i = 0; i < 3; i++){
        int span = charToVal((currentGame->gameBoard)[i][0]) + charToVal((currentGame->gameBoard)[i+1][1]) + charToVal((currentGame->gameBoard)[i+2][2]);
        int j = 3;
        for(int k = i+3; k <6; k++){
            span += charToVal((currentGame->gameBoard)[k][j]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[k-3][j-3]);
            j++;
        }
    }
    
    for(int j = 1; j <4; j++){
        
        int span = charToVal((currentGame->gameBoard)[0][j]) + charToVal((currentGame->gameBoard)[1][j+1]) + charToVal((currentGame->gameBoard)[2][j+2]);
        int i = 3;
        for (int k = j + 3; k<7; k ++){
            
            span += charToVal((currentGame->gameBoard)[i][k]);
            resArray[4+span]++;
            span -= charToVal((currentGame->gameBoard)[i-3][k-3]);
            i++;
        }
    }

}


int comFunc(SPFiarGame* currentGame){
    int resArray[9] = {0,0,0,0,0,0,0,0,0};
    int res = 0;
    
    horizontals(resArray, currentGame);
    verticals(resArray, currentGame);
    diagunalsLeftRight(resArray, currentGame);
    diagunalsRightLeft(resArray, currentGame);

    
    if(resArray[0] > 0)
        return INT_MIN;
    if(resArray[8] > 0)
        return INT_MAX;
    
    res = resArray[1]*(-5) + resArray[2]*(-2) + resArray[3]*(-1) + resArray[5]*1+resArray[6]*2 + resArray[7]*5;
    
    return res;
}





int initMinmaxTree(SPFiarGame* currentGame,unsigned int depth){
    int alpha = INT_MIN;
    int beta = INT_MAX;
    int max = INT_MIN;
    int min = INT_MAX;
    int temp = 0;
    int bestMoveMIN = 0;
    int bestMoveMAX = 0;
    for(int i = 0; i <7; i++){
        if(!spFiarGameIsValidMove(currentGame,i))
            continue;
        spFiarGameSetMove(currentGame,i);
        temp = minmaxTree(currentGame, depth-1, alpha, beta);
        spFiarGameUndoPrevMove(currentGame);
        
        if(temp>max){
            bestMoveMAX = i;
            max = temp;
        }
        if(temp<min){
            bestMoveMIN = i;
            min = temp;
        }
    }

        if (currentGame->currentPlayer==SP_FIAR_GAME_PLAYER_1_SYMBOL)
            return bestMoveMAX;
    return bestMoveMIN;
}




int minmaxTree(SPFiarGame* currentGame, int depth, int alpha, int beta){
    if ((depth == 0) || (spFiarCheckWinner(currentGame) != 0)){
        return comFunc(currentGame);
    }
    if (currentGame->currentPlayer==SP_FIAR_GAME_PLAYER_1_SYMBOL){
        int v = INT_MIN;
        for(int i = 0; i < 7; i++){
            
            if (!spFiarGameIsValidMove(currentGame, i))
                continue;;
            spFiarGameSetMove(currentGame,i);
            
            v = maxFunc(v, minmaxTree(currentGame, depth-1, alpha, beta));
            spFiarGameUndoPrevMove(currentGame);
            
            alpha = maxFunc(alpha, v);
            if (beta <= alpha)
                break;
            
            
        }
        return v;
        
    }
    else{
        int v = INT_MAX;
        for(int i = 0; i <7; i++){
            if(!spFiarGameIsValidMove(currentGame,i))
                continue;
            
            spFiarGameSetMove(currentGame,i);
            v = minFunc(v, minmaxTree(currentGame, depth-1, alpha, beta));
            spFiarGameUndoPrevMove(currentGame);
            
            beta = minFunc(beta, v);
            if(beta <= alpha)
                break;
        }
        return v;
        
    }
}

int maxFunc(int a, int b)
{
    if (a>=b)
        return a;
    return b;
}
int minFunc(int a, int b){
    if (a<=b)
        return a;
    return b;
}
