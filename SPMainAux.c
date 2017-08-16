#include <stdio.h>
#include <stdlib.h>
#include "SPMainAux.h"
#include "SPMiniMax.h"
#include "SPFIARParser.h"
#include <string.h>
#include "SPMiniMaxNode.h"

SPFiarGame *curGame;
char theWinner = ' ';

void init() {
	char input[SP_MAX_LINE_LENGTH];
	bool createNewGame = true;
	GAME_STATUS status;

	while (true) {
		printf("Please enter the difficulty level between [1-7]:\n");
		while (true)
		{
			fgets(input, SP_MAX_LINE_LENGTH, stdin);
			if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
				input[strlen(input) - 1] = 0;
			if (spParserPraseLine(input).cmd == SP_QUIT)
			{
				printf("Exiting...\n");
				exit(0);
			}
			if (spParserIsInt(input)) //checking if the level entered is 1-7
			{
				if (strlen(input) == 1)
					if ((input[0] > 48) && (input[0] < 56))
					{
						while (true)
						{
							status = gameLoop((input[0]) - 48, createNewGame);
							if (status == RESTART)
							{
								createNewGame = true;
								spFiarGameDestroy(curGame);
								break;
							}
							if (status == UNDO_SUCCESS)
							{
								createNewGame = false;
							}
						}
						break;
					}
			}
			printf("Error: invalid level (should be between 1 to 7)\n");
			break;
		}
	}
}

GAME_STATUS gameLoop(int level, bool createNewGame)
{
	if (createNewGame)
	{
		curGame = spFiarGameCreate(20);
	}

	theWinner = ' ';

	GAME_STATUS gameOverStatus;

	if (gameRun(level) == RESTART) //restart
		return RESTART;

	spFiarGamePrintBoard(curGame);  //else- there is a winner (if there isn't - the game continue in the  gameRun function)

	winnerPrint();

	while (true)				//game is over- what can the user do next?
	{
		gameOverStatus = gameOver();
		if (gameOverStatus == RESTART)	//restart
			return RESTART;
		else if (gameOverStatus == UNDO_SUCCESS)// undo done, keep playing
			return UNDO_SUCCESS;
	}//else- have to run gameOver again (undo didn't work / the input was invalid)

}

GAME_STATUS gameRun(int level)
{
	while (true)
	{
		theWinner = spFiarCheckWinner(curGame);
		if (theWinner != 0)		//if there is a winner- don't continue the game as usual
			break;

		if (spFiarGameGetCurrentPlayer(curGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) //else- if now is user's turn
		{
			spFiarGamePrintBoard(curGame);
			if (userTurn(level) == RESTART)		//is means that the user pressed "restart"
			{
				printf("Game restarted!\n");
				return RESTART;					//back to start- "enter a level.. 1-7..."
			}
			continue;					//next turn
		}

		if (spFiarGameGetCurrentPlayer(curGame) == SP_FIAR_GAME_PLAYER_2_SYMBOL)	//if now is computer's turn
		{
			compTurn(level);
			continue;					//next turn
		}
	}
	return THERE_IS_A_WINNER;
}


GAME_STATUS userTurn(int level)
{
	char input[SP_MAX_LINE_LENGTH];

	SPCommand command;
	printf("Please make the next move:\n");

	while (true)
	{
		fgets(input, SP_MAX_LINE_LENGTH, stdin);
		if (input[0] == '\0' || input[0] == 0)  //null character
		{
			printf("Error: invalid command\n");
			continue;
		}
		if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
			input[strlen(input) - 1] = 0;

		command = spParserPraseLine(input);
		if (command.cmd == SP_INVALID_LINE)
			printf("Error: invalid command\n");

		if (command.cmd == SP_RESTART)
			return RESTART;

		if (command.cmd == SP_QUIT)
			quit(curGame);

		if (command.cmd == SP_SUGGEST_MOVE)
			printf("Suggested move: drop a disc to column %d\n", suggestMove(level) + 1);

		if (command.cmd == SP_UNDO_MOVE)
		{
			if (undo() != UNDO_SUCCESS)
				continue;
			break;
		}

		if (command.cmd == SP_ADD_DISC)
		{
			if (addDisc(command) == 0)
				continue;
			break;
		}
	}

	return PLAY_CONT;	//user didn't press restart
}

int suggestMove(int level)
{
	return spMinimaxSuggestMove(curGame, level);
}


void quit()
{
	spFiarGameDestroy(curGame);
	printf("Exiting...\n");
	exit(0);
}

void compTurn(int level)
{
	int move = spMinimaxSuggestMove(curGame, level);
	printf("Computer move: add disc to column %d\n", move + 1);
	SP_FIAR_GAME_MESSAGE message = spFiarGameSetMove(curGame, move);
	if (message != SP_FIAR_GAME_SUCCESS)
	{
		printf("Error: spFiarGameSetMove has failed");
		spFiarGameDestroy(curGame);
		exit(0);
	}
}

void winnerPrint()
{
	if (theWinner == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		printf("Game over: you win\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
	if (theWinner == SP_FIAR_GAME_PLAYER_2_SYMBOL)
		printf("Game over: computer wins\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");
	if (theWinner == SP_FIAR_GAME_TIE_SYMBOL)
		printf("Game over: it\'s a tie\nPlease enter \'quit\' to exit or \'restart\' to start a new game!\n");

}

GAME_STATUS gameOver()
{
	char input[SP_MAX_LINE_LENGTH];
	fgets(input, SP_MAX_LINE_LENGTH, stdin);
	if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
		input[strlen(input) - 1] = 0;
	SPCommand command = spParserPraseLine(input);
	if (command.cmd == SP_RESTART)
	{
		printf("Game restarted!\n");
		return RESTART;
	}

	if (command.cmd == SP_QUIT)
		quit();

	if (command.cmd == SP_INVALID_LINE)
		printf("Error: invalid command\n");

	if (theWinner != SP_FIAR_GAME_PLAYER_1_SYMBOL)	//undo is allowed only when the user didn't win
	{
		if (command.cmd == SP_UNDO_MOVE)
		{
			if (undo() != UNDO_SUCCESS)
				return GAME_OVER_LOOP;		//don't restart, and undo failed
			else
				return UNDO_SUCCESS;		//don't restart, and undo succseeded
		}
	}
	else
		printf("Error: the game is over\n");

	return GAME_OVER_LOOP;	//it means something went wrong and we have to run the gameOver function again
}

GAME_STATUS undo()
{
	int col = -1;
	col = spArrayListGetLast(curGame->historyArray);
	SP_FIAR_GAME_MESSAGE message = spFiarGameUndoPrevMove(curGame);

	if (message == SP_FIAR_GAME_INVALID_ARGUMENT)
	{
		printf("Error: spFiarGameUndoPrevMove has failed");
		spFiarGameDestroy(curGame);
		exit(0);
	}

	if (message == SP_FIAR_GAME_NO_HISTORY)
	{
		printf("Error: cannot undo previous move!\n");
		curGame->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		return UNDO_FAIL;
	}
	printf("Remove disc: remove computer's disc at column %d\n", col + 1);
	printf("Remove disc: remove user's disc at column %d\n", spArrayListGetLast(curGame->historyArray) + 1);
	spFiarGameUndoPrevMove(curGame);
	return UNDO_SUCCESS;
}


int addDisc(SPCommand command)
{
	if ((!command.validArg) || (command.arg > 7) || (command.arg < 1))
	{
		printf("Error: column number must be in range 1-7\n");
		curGame->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		return 0;
	}

	SP_FIAR_GAME_MESSAGE message = spFiarGameSetMove(curGame, command.arg - 1);

	if (message == SP_FIAR_GAME_INVALID_ARGUMENT)
	{
		printf("Error: spFiarGameSetMove has failed");
		spFiarGameDestroy(curGame);
		exit(0);
	}
	if (message == SP_FIAR_GAME_INVALID_MOVE)
	{
		printf("Error: column %d is full\n", command.arg);
		curGame->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
		return 0;
	}
	return 1;
}
