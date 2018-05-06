#ifndef GAMEPIECES_H_
#define GAMEPIECES_H_

#include <stdbool.h>
#include "Game.h"
/*
 * returns true if the bishop in position <a,b> can move to <c,d>
 */
bool BishopValidMove(Game *game, int a, int b, int c, int d);
/*
 * returns true if the knight in position <a,b> can move to <c,d>
 */
bool KnightValidMove(Game *game, int a, int b, int c, int d);
/*
 * returns true if the pawn in position <a,b> can move to <c,d>
 */
bool PawnValidMove(Game *game, int a, int b, int c, int d);
/*
 * returns true if the rook in position <a,b> can move to <c,d>
 */
bool RookValidMove(Game *game, int a, int b, int c, int d);
/*
 * returns true if the queen in position <a,b> can move to <c,d>
 */
bool QueenValidMove(Game *game, int a, int b, int c, int d);
/*
 * returns true if the king in position <a,b> can move to <c,d>
 */
bool KingValidMove(Game *game, int a, int b, int c, int d);

/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in diagonal, up-right direction
 */
bool checkDiagonalUpRight(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in diagonal, up-left direction
 */
bool checkDiagonalUpLeft(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in diagonal, down-left direction
 */
bool checkDiagonalDownLeft(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in diagonal, down-right direction
 */
bool checkDiagonalDownRight(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in a straight up direction
 */
bool checkLineUp(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in a straight down direction
 */
bool checkLineDown(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in a straight right direction
 */
bool checkLineRight(Game *game, int a, int b, int c, int d);
/*
 * returns true if the the cells between <a,b> and <c,d> are empty,
 * going in a straight left direction
 */
bool checkLineLeft(Game *game, int a, int b, int c, int d);
/*
 * returns true if the pattern between <a,b> to <c,d> is a legal knight pattern
 */
bool checkKnightPattern(Game *game, int a, int b, int c, int d);
/*
 * returns true if the piece in <a,b> is the same color as the piece in <c,d>
 */
bool sameColorPieces(Game *game, int a, int b, int c, int d);
/*
 * returns the name of the piece
 * @param char piece - the representing character of the piece in lower case.
 */
char *getPieceName(char piece);
/*
 * returns true if the king in position <KingX,KingY> is threatened by the opponent.
 * the function assumes there is a King piece in the specified position.
 * @ param Game *game - the source game
 * @ param int KingX, KingY - the possition of the king to be checked.
 */
bool isKingThreatened(Game *game, int KingX, int KingY);
#endif /* GAMEPIECES_H_ */
