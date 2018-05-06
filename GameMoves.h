#ifndef GAMEMOVES_H_
#define GAMEMOVES_H_
#include <stdbool.h>
#include "Game.h"
#include "GamePieces.h"

/*
 * a method that moves a game piece from <a,b> to <c,d> if the move is legal.
 * @param Game *game - the source game
 * @param int a,b,c,d - a,b are the source row and column, c,d are the target.
 * @return
 * GAME_INVALID_ARGUMENT - an invalid position on the board
 * GAME_INVALID_MOVE - if the move is illegal
 * GAME_NO_PIECE - if the current user doesn't have a piece in position <a,b>
 * GAME_SUCCESS - successfully committed this move
 */
GAME_MESSAGE SetMove(Game *game, int a, int b, int c, int d);

/*
 * a method to determine if the given move is legal.
 * @param Game *game - the source game
 * @param int a,b,c,d - a,b are the source row and column, c,d are the target.
 * @return
 * GAME_INVALID_ARGUMENT - an invalid position on the board
 * GAME_INVALID_MOVE - if the move is illegal
 * GAME_NO_PIECE - if the current user doesn't have a piece in position <a,b>
 * GAME_SUCCESS - successfully committed this move
 */
GAME_MESSAGE is_valid_move(Game *game, int a, int b, int c, int d);

/*
 * a method that goes over the game board to see if either king is threatened (Check).
 * @param Game *game - the source game
 * @return
 * int BLACK_PLAYER (0) if black king is threatened
 * int WHITE_PLAYER (1) if white king is threatened
 */
int isCheck(Game *game);

/*
 * a method that goes over the game board to see if current player has any
 * legal moves.
 * @param Game *game - the source game
 * @return
 * int BLACK_PLAYER (0) if black player has no legal moves.
 * int WHITE_PLAYER (1) if white player has no legal moves.
 */bool hasValidMove(Game *game);

/*
 * a method that determines whether the game is over, and returns the end state
 * of the game.
 * @param Game *game - the source game
 * @return
 * int BLACK_PLAYER (0) if black player wins.
 * int WHITE_PLAYER (1) if white player wins.
 * int 2 if the game is a tie.
 * int -1 if the game is not over.
 */
int isCheckMate(Game *game);

/*
 * undo the last move of the black and white players.
 * @param Game *game - the source game
 * @return
 * GAME_NO_HISTORY - if the given game has no history or the undo option operated more than (history_size) times
 * GAME_SUCCESS - if no error occurred.
 */
GAME_MESSAGE undoMove(Game *game, bool printMsg);

/*
 * updates the kings' positions field of the game after
 * loading game from XML file.
 * @param Game *game - the source game
 */
void updateKingsPosition(Game *game);
#endif /* GAMEMOVES_H_ */
