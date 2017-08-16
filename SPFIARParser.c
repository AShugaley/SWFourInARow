#include "SPFIARParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool spParserIsInt(const char* str)
{
    
    unsigned int i = 0;
    if (str[0] == '-' || (str[0]<58 && str[0]>47))  //first char is minus or digit
    {
        for (i = 1; i < strlen(str); i++)
        {
            if (str[i] > 57 || str[i] < 48)  // not a digit
                return false;
        }
        return true;
    }
    return false;
}


SPCommand spParserPraseLine(const char* str)
{
    
    SPCommand command;
    command.validArg = false;				//true only in case of "add_disc" with valid number
    char currentStr[SP_MAX_LINE_LENGTH] = "\0";
    strcpy(currentStr, str);
    char* curToken;
    char delimiter[] = " \t\r\n";
    
    curToken = strtok(currentStr, delimiter);	//curtoken is the first part of the string
    if (curToken == '\0')	//doesn't suppose to get here, just in case ...
    {
        command.cmd = SP_INVALID_LINE;
        return command;
    }
    
    
    if (!strcmp(curToken, "undo_move"))	//curtoken == undo (strcmp = 0 in this case)
    {
        command.cmd = SP_UNDO_MOVE;
    }
    
    else if (!strcmp(curToken, "suggest_move"))
    {
        command.cmd = SP_SUGGEST_MOVE;
    }
    
    else if (!strcmp(curToken, "quit"))
    {
        command.cmd = SP_QUIT;
    }
    
    else if (!strcmp(curToken, "restart_game"))
    {
        command.cmd = SP_RESTART;
    }
    
    else if (!strcmp(curToken, "add_disc"))
    {
        command.cmd = SP_ADD_DISC;
        curToken = strtok(NULL, delimiter);
        if (curToken != NULL)
        {
            if (spParserIsInt(curToken))
            {
                command.arg = atoi(curToken);
                command.validArg = true;
            }
        }
        
    }
    
    else
    {
        command.cmd = SP_INVALID_LINE;
    }
    
    //if there is additional text after the first& second parts  - invalid 
    curToken = strtok(NULL, delimiter);
    if (curToken != NULL)
    {
        command.cmd = SP_INVALID_LINE;
        command.validArg = false;
        
    }
    
    
    return command;
}
