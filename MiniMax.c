#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "MiniMax.h"

GameCommand *calculateMove(Game *game) {
	GameCommand *result=(GameCommand*)malloc(sizeof(GameCommand));
	if (game == NULL ) {
		result->validArg = false;
		return result;
	}
	int depth = game->difficulty;
	Game *copyGame = copy_game(game);
	int alpha = INT_MIN;
	int beta = INT_MAX;
	if(game->user_color==BLACK_PLAYER) //computer maximize score
		alphaBetaMax(copyGame, alpha, beta, depth, result);
	else //computer minimizes score
		alphaBetaMin(copyGame,alpha,beta, depth, result);

	result->cmd = GAME_MOVE;
	result->validArg = true;
	return result;
}

int alphaBetaMax(Game *game, int alpha, int beta,
		int depthLeft, GameCommand *cmd) {
	int possibleResult=0, bestScore=0;
	int a,b,c,d;
	bool setAnyMove=false;
	if (depthLeft == 0)
		return evaluateScore(game);

	bestScore = INT_MIN;
	for (a = 0; a < 8; a++) {
		for (b = 0; b < 8; b++) {
			for (c = 0; c < 8; c++) {
				for (d = 0; d < 8; d++) {
					if ((is_valid_move(game, a, b, c, d)) == GAME_SUCCESS) {
						if(game->difficulty==depthLeft && !setAnyMove){
							//makes sure that at least one move is set
							cmd->move_args[0]=a;
							cmd->move_args[1]=b;
							cmd->move_args[2]=c;
							cmd->move_args[3]=d;
							setAnyMove=true;
						}
						Game *copyGame = copy_game(game);
						SetMove(copyGame, a, b, c, d);

						possibleResult = alphaBetaMin(copyGame, alpha, beta,
								depthLeft - 1,cmd);
						destroy_game(copyGame);
						//score = possibleResult;
						if (possibleResult >= beta) {
							if(depthLeft==game->difficulty){
								cmd->move_args[0] = a;
								cmd->move_args[1] = b;
								cmd->move_args[2] = c;
								cmd->move_args[3] = d;
							}
							bestScore = beta;
							return bestScore;
						}
						if (possibleResult > alpha) {
							alpha = possibleResult;
							bestScore=possibleResult;
							if(depthLeft==game->difficulty){ //this is the root node
								cmd->move_args[0] = a;
								cmd->move_args[1] = b;
								cmd->move_args[2] = c;
								cmd->move_args[3] = d;
							}
						}
					}
				}
			}
		}
	}
	return bestScore;
}

int alphaBetaMin(Game *game, int alpha, int beta,
		int depthLeft, GameCommand *cmd) {
	int possibleResult=0, bestScore=0;
	int a,b,c,d;
	bool setAnyMove=false; //set the first legal move to GameCommand
	if (depthLeft == 0)
		return evaluateScore(game);
	bestScore = INT_MAX;
	for (a = 0; a < 8; a++) {
		for (b = 0; b < 8; b++) {
			for (c = 0; c < 8; c++) {
				for (d = 0; d < 8; d++) {
					if ((is_valid_move(game, a, b, c, d)) == GAME_SUCCESS) {
						if(game->difficulty==depthLeft && !setAnyMove){
							//makes sure that at least one move is set
							cmd->move_args[0]=a;
							cmd->move_args[1]=b;
							cmd->move_args[2]=c;
							cmd->move_args[3]=d;
							setAnyMove=true;
						}
						Game *copyGame = copy_game(game);
						SetMove(copyGame, a, b, c, d);

						possibleResult = alphaBetaMax(copyGame, alpha, beta,
								depthLeft - 1,cmd);
						destroy_game(copyGame);
						if (possibleResult <= alpha) {
							if(depthLeft==game->difficulty){
							cmd->move_args[0] = a;
							cmd->move_args[1] = b;
							cmd->move_args[2] = c;
							cmd->move_args[3] = d;
							}
							bestScore = alpha;
							return bestScore;
						}
						if (possibleResult < beta) {
							beta = possibleResult;
							bestScore=possibleResult;
							if(depthLeft==game->difficulty){ //this is the root node
								cmd->move_args[0] = a;
								cmd->move_args[1] = b;
								cmd->move_args[2] = c;
								cmd->move_args[3] = d;
							}
						}
					}
				}
			}
		}
	}
	return bestScore;
}

int evaluateScore(Game *game) { // white maximizes, black minimizes
	int i,j, score=0;
	int checkmate=isCheckMate(game);
	if(checkmate==BLACK_PLAYER)
		return INT_MIN; //black player won
	if(checkmate==WHITE_PLAYER)
		return INT_MAX; //white player won
	//else, calculate board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			switch (game->gameboard[i][j]) {
			case 'm':
				score++;
				break;
			case 'M':
				score--;
				break;
			case 'n':
				score += 3;
				break;
			case 'N':
				score -= 3;
				break;
			case 'b':
				score += 3;
				break;
			case 'B':
				score -= 3;
				break;
			case 'r':
				score += 5;
				break;
			case 'R':
				score -= 5;
				break;
			case 'q':
				score += 9;
				break;
			case 'Q':
				score -= 9;
				break;
			case 'k':
				score += 100;
				break;
			case 'K':
				score -= 100;
				break;
			}
		}
	}
	if (score > 10000){
		printf("\nErroneous score!!\n");
	}
	return score;
}
