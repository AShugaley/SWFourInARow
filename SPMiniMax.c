
#include <stdio.h>
#include "SPFIARGame.h"
#include "SPMiniMaxNode.h"

int spMinimaxSuggestMove(SPFiarGame* src, unsigned int depth){
    //printf("START MINMAXTREE\n");
    int i =initMinmaxTree(src, depth);
    //printf("END MINMAXTREE\n");
    return i;
}
