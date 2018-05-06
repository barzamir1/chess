#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Game.h"
#include "GameMoves.h"
#include "GamePieces.h"
#include "MainAux.h"

#define isWhitePiece(x) (x>='a' && x<='z' ? 1 : 0)

typedef struct possible_move_t{
	int score;
	int coords[4];
}possibleMove;

GameCommand *calculateMove(Game *game);

int alphaBetaMax(Game *game, int alpha, int beta, int depthLeft, GameCommand *cmd);

int alphaBetaMin(Game *game, int alpha, int beta, int depthLeft, GameCommand *cmd);

int evaluateScore(Game *game);

#endif /* MINIMAX_H_ */
