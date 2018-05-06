#include "GameMoves.h"
#define intToCharCol(x) (x+'A')

GAME_MESSAGE SetMove(Game *game, int a, int b, int c, int d) {
	GAME_MESSAGE msg = is_valid_move(game, a, b, c, d);
	if (msg == GAME_SUCCESS) {
		//save move history
		HistoryElement *currMove=CreateMoveHistory(a,b,c,d,game->gameboard[a][b],game->gameboard[c][d]);
		if(game->history->actualSize==game->history->maxSize) //list is full
			spArrayListRemoveLast(game->history);
		spArrayListAddFirst(game->history, currMove);
		//make the move on the board
		game->gameboard[c][d] = game->gameboard[a][b];
		game->gameboard[a][b] = BLANK;
		game->curr_player ^= 1; //curr Xor 1 turn 0 to 1 and 1 to 0.
		//update king's position if necessary
		if (game->gameboard[c][d] == 'K') {
			game->black_king_pos[0] = c;
			game->black_king_pos[1] = d;
		}
		if (game->gameboard[c][d] == 'k') {
			game->white_king_pos[0] = c;
			game->white_king_pos[1] = d;
		}
	}
	return msg;
}
GAME_MESSAGE is_valid_move(Game *game, int a, int b, int c, int d) {
	char piece = game->gameboard[a][b];
	GAME_MESSAGE msg;
	if (isOutOfRange(a) || isOutOfRange(b) || isOutOfRange(c) || isOutOfRange(d)
			|| (a == c && b == d))
		return GAME_INVALID_ARGUMENT; //position is out of range or target is the same.
	if (piece == BLANK)
		return GAME_NO_PIECE;
	//if <a,b> color != current player's color
	if (isWhitePiece(piece) != game->curr_player)
		return GAME_NO_PIECE;
	if (sameColorPieces(game, a, b, c, d))
		return GAME_INVALID_MOVE;

	//check if the piece can move by its type
	msg = GAME_INVALID_MOVE;
	if (piece == 'R' || piece == 'r') { //Rook
		if (RookValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	if (piece == 'B' || piece == 'b') { //Bishop
		if (BishopValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	if (piece == 'N' || piece == 'n') { //Knight
		if (KnightValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	if (piece == 'Q' || piece == 'q') { //Queen
		if (QueenValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	if (piece == 'M' || piece == 'm') { //Pawn
		if (PawnValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	if (piece == 'K' || piece == 'k') { //King
		if (KingValidMove(game, a, b, c, d))
			msg = GAME_SUCCESS;
	}
	//check the king isn't threatened after this move.
	int kingX, kingY;
	if (msg == GAME_SUCCESS) { // && (piece!='k'&&piece!='K')){
		Game *cpy = copy_game(game);
		cpy->gameboard[c][d] = cpy->gameboard[a][b]; //make the move on the copy board
		cpy->gameboard[a][b] = BLANK;
		if (piece == 'k') {
			cpy->white_king_pos[0] = c;
			cpy->white_king_pos[1] = d;
		}
		if (piece == 'K') {
			cpy->black_king_pos[0] = c;
			cpy->black_king_pos[1] = d;
		}
		if (game->curr_player == BLACK_PLAYER) { //black player
			kingX = cpy->black_king_pos[0];
			kingY = cpy->black_king_pos[1];
		} else {
			kingX = cpy->white_king_pos[0];
			kingY = cpy->white_king_pos[1];
		}
		cpy->curr_player ^= 1; //change current player to opponent.
		if (isKingThreatened(cpy, kingX, kingY)) //check that the king isn't threatened
			msg = GAME_INVALID_MOVE;
		destroy_game(cpy);
	}
	return msg;
}

int isCheck(Game *game) {
	if (isKingThreatened(game, game->black_king_pos[0],
			game->black_king_pos[1]))
		return BLACK_PLAYER;
	if (isKingThreatened(game, game->white_king_pos[0],
			game->white_king_pos[1]))
		return WHITE_PLAYER;
	return -1;
}
bool hasValidMove(Game *game) {   //check if current player has a valid move
	int row, col, i, j;
	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (game->gameboard[row][col] == BLANK
					|| ((isWhitePiece(game->gameboard[row][col])
							^ game->curr_player) == 1))
				continue;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					if (is_valid_move(game, row, col, i, j) == GAME_SUCCESS) {
						return true;
					}
				}
			}
		}
	}
	return false;
}
int isCheckMate(Game *game) {
	if (!hasValidMove(game)) {
		if (isCheck(game) == game->curr_player)
			return game->curr_player ^ 1;
		return 2;
	}
	return -1;
}
GAME_MESSAGE undoMove(Game *game, bool printMsg) {
	int i;
	HistoryElement *lastMove;
	int last_player=game->history->actualSize%2==0 ? BLACK_PLAYER : WHITE_PLAYER;
	int a,b,c,d;
	if(game->gameMode==2)
		return GAME_INVALID_ARGUMENT; //not valid in 2-player mode
	if(game->history->actualSize < 2)
		return GAME_NO_HISTORY;

	for(i=0; i<2; i++){
		lastMove=spArrayListGetFirst(game->history);
		//undo move
		a=lastMove->moveArgs[0];
		b=lastMove->moveArgs[1];
		c=lastMove->moveArgs[2];
		d=lastMove->moveArgs[3];
		game->gameboard[a][b]=lastMove->pieceAtSource;
		game->gameboard[c][d]=lastMove->pieceAtTardget;
		if(printMsg)
			printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n", PlayerStirng(last_player),
						c+1,intToCharCol(d),a+1,intToCharCol(b));
		spArrayListRemoveFirst(game->history);
		last_player^=1; //the opponent
	}
	updateKingsPosition(game);
	return GAME_SUCCESS;
}

void updateKingsPosition(Game *game){
	int i, j;
	bool blackFound=false;
	bool whiteFound=false;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(game->gameboard[i][j]=='k'){
				game->white_king_pos[0]=i;
				game->white_king_pos[1]=j;
				whiteFound=true;
			}
			if(game->gameboard[i][j]=='K'){
				game->black_king_pos[0]=i;
				game->black_king_pos[1]=j;
				blackFound=true;
			}
			if(blackFound && whiteFound)
				return;
		}
	}
}
