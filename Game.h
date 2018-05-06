#ifndef GAME_H_
#define GAME_H_
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SPArrayList.h"


//Definitions
#define GAME_BOARD_SIZE 8
#define BLACK_PLAYER 0
#define WHITE_PLAYER 1
#define BLANK '_'
#define PlayerStirng(x) ((x)==WHITE_PLAYER ? "white":"black")
#define PlayerStirngToCapital(x) ((x)==WHITE_PLAYER ? "WHITE":"BLACK")
#define isWhitePiece(x) (x>='a' && x<='z' ? 1 : 0)
#define isOutOfRange(x) (x<0 || x>7 ? 1 : 0)
//#define PlayerStringToLower(x) ((x)==WHITE_PLAYER ? "white":"black")
typedef struct game_t{
	int gameMode; //1 or 2 players
	int difficulty; //1-player mode only
	int user_color; //1-player mode only.
	int black_king_pos[2]; //king's current position
	int white_king_pos[2];
	char gameboard[GAME_BOARD_SIZE][GAME_BOARD_SIZE];
	int curr_player; //black or white
	SPArrayList *history;
	//history *history;
}Game;

typedef enum game_message_t {
	GAME_INVALID_MOVE,
	GAME_INVALID_ARGUMENT,
	GAME_NO_PIECE,
	GAME_NO_HISTORY,
	GAME_SUCCESS,
	GAME_WHITE_THREATEND,
	GAME_BLACK_THREATEND,
	GAME_TIE,
	GAME_CHECKMATE,
	GAME_QUIT_SUCCESS,
	GAME_RESET_SUCCESS
} GAME_MESSAGE;

Game * create_game(int history_size);
Game * copy_game(Game *srcGame);
void destroy_game(Game *game);

void print_settings(Game *game);
void print_board(Game *game);
/*
 * initialize the game board to a starting position
 * @param Game *game - the source game
 */
void initializeBoard(Game *game);
/*
 * initialize the game to default values
 * @param Game *game - the source game
 */
void initializeGameParam(Game *game);
/*
 * copy the game board of src to the game board of target.
 * @param Game *src - the source game
 * @param Game *target - the target game
 */
void copyGameBoad(Game *src, Game *target);

#endif /* GAME_H_ */
