#include "GamePieces.h"

#define abs(x,y) ((x)-(y)<0 ? -((x)-(y)) : ((x)-(y)))
bool BishopValidMove(Game *game, int a, int b, int c, int d){
	if(abs(a,c) != abs(b,d)) //<c,d> is not diagonal to <a,b>
		return false;
	if(sameColorPieces(game,a,b,c,d))
		return false;

	//check that <a,b> piece doesn't leap over other pieces
	if(a<c && b<d)
		return checkDiagonalUpRight(game,a,b,c,d);
	if(a<c && b>d)
		return checkDiagonalUpLeft(game,a,b,c,d);
	if(a>c && b<d)
		return checkDiagonalDownRight(game,a,b,c,d);
	if(a>c && b>d)
		return checkDiagonalDownLeft(game,a,b,c,d);
	else
		return false;
}
bool KnightValidMove(Game *game, int a, int b, int c, int d){
	bool valid=false;
	if(sameColorPieces(game,a,b,c,d))
		return false;
	valid = checkKnightPattern(game,a,b,c,d); //jump 2 rows 1 col.
	if(!valid)
		valid=checkKnightPattern(game, b,a,d,c); //jump 1 row 2 col.
	return valid;

}
bool PawnValidMove(Game *game, int a, int b, int c, int d){
	//black pawn can only move downwards, white pawn - upwards.
	char pawn=game->gameboard[a][b];
	int oneStep=isWhitePiece(pawn) ? 1 : -1;
	if(sameColorPieces(game,a,b,c,d))
		return false;
	if(a+oneStep==c && b==d && game->gameboard[c][d]==BLANK) //one step forward
		return true;
	if(a+oneStep==c && abs(b,d)==1 && game->gameboard[c][d]!=BLANK) //one diagonal step
		return true;
	if(a==1 && game->curr_player==WHITE_PLAYER && a+2==c && b==d) //two steps forward for white pawn
		if(game->gameboard[c][d]==BLANK) //pawn can only capture one step diagonally.
			return true;
	if(a==6 && game->curr_player==BLACK_PLAYER && a-2==c && b==d) //two steps forward for black pawn
		if(game->gameboard[c][d]==BLANK) //pawn can only capture one step diagonally.
			return true;
	return false;
}
bool RookValidMove(Game *game, int a, int b, int c, int d){
	if(sameColorPieces(game,a,b,c,d))
		return false;
	if(a<c && b==d)
		return checkLineUp(game,a,b,c,d);
	if(a>c && b==d)
		return checkLineDown(game,a,b,c,d);
	if(a==c && b<d)
		return checkLineRight(game,a,b,c,d);
	if(a==c && b>d)
		return checkLineLeft(game,a,b,c,d);
	return false;
}
bool QueenValidMove(Game *game, int a, int b, int c, int d){
	if(sameColorPieces(game,a,b,c,d))
		return false;
	if(a<c){
		if(b<d)
			return checkDiagonalUpRight(game,a,b,c,d);
		if(b>d)
			return checkDiagonalUpLeft(game,a,b,c,d);
		if(b==d)
			return checkLineUp(game,a,b,c,d);
	}
	if(a>c){
		if(b<d)
			return checkDiagonalDownRight(game,a,b,c,d);
		if(b>d)
			return checkDiagonalDownLeft(game,a,b,c,d);
		if(b==d)
			return checkLineDown(game,a,b,c,d);
	}
	if(a==c){
		if(b<d)
			return checkLineRight(game,a,b,c,d);
		if(b>d)
			return checkLineLeft(game,a,b,c,d);
	}
	return false;
}
bool KingValidMove(Game *game, int a, int b, int c, int d){
	bool result=false;

	if(abs(a,c)>1) //can only move one step to each direction
		return false;
	if(abs(b,d)>1)
		return false;
	if(sameColorPieces(game,a,b,c,d))
		return false;

	//assume move is legal, make the move and check if the king is threatened.
	Game *cpyGame=copy_game(game);
	cpyGame->gameboard[c][d]=game->gameboard[a][b];
	cpyGame->gameboard[a][b]=BLANK;
	result=isKingThreatened(cpyGame,c,d);
	destroy_game(cpyGame);
	return !result;
}

bool checkDiagonalUpRight(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=c-a; i++){ //go over the cells between <a,b> to <c,d>
		if(a+i==c && b+i==d) //reached the desired position
			return true;
		if(game->gameboard[a+i][b+i]!=BLANK) //do not leap over other pieces
			return false;
	}
	return false;
}
bool checkDiagonalUpLeft(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=c-a; i++){ //go over the cells between <a,b> to <c,d>
		if(a+i==c && b-i==d)
			return true;
		if(game->gameboard[a+i][b-i]!=BLANK) //do not leap over other pieces
			return false;
	}
	return false;
}
bool checkDiagonalDownLeft(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=a-c; i++){ //go over the cells between <a,b> to <c,d>
		if(a-i==c && b-i==d)
			return true;
		if(game->gameboard[a-i][b-i]!=BLANK) //do not leap over other pieces
			return false;
	}
	return false;
}
bool checkDiagonalDownRight(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=a-c; i++){ //go over the cells between <a,b> to <c,d>
		if(a-i==c && b+i==d) //reached desired position
			return true;
		if(game->gameboard[a-i][b+i]!=BLANK) //do not leap over other pieces
			return false;
	}
	return false;
}
bool checkLineUp(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=c-a; i++){
		if(a+i==c && b==d)
			return true;
		if(game->gameboard[a+i][b]!=BLANK)
			return false;
	}
	return false;
}
bool checkLineDown(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=a-c-1; i++)
		if(game->gameboard[a-i][b]!=BLANK)
			return false;
	return true;
}
bool checkLineRight(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=d-b-1; i++)
		if(game->gameboard[a][b+i]!=BLANK)
			return false;
	return true;
}
bool checkLineLeft(Game *game, int a, int b, int c, int d){
	int i;
	for(i=1; i<=b-d-1; i++)
		if(game->gameboard[a][b-i]!=BLANK)
			return false;
	return true;
}
bool checkKnightPattern(Game *game, int a, int b, int c, int d){
	if(a+2==c && b+1==d) //UpRight
		return true;
	if(a+2==c && b-1==d) //UpLeft
		return true;
	if(a-2==c && b+1==d) //DowmRight
		return true;
	if(a-2==c && b-1==d) //DownLeft
		return true;
	return false;
}
bool sameColorPieces(Game *game, int a, int b, int c, int d){
	char abPiece=game->gameboard[a][b];
	char cdPiece=game->gameboard[c][d];
	if(cdPiece==BLANK)
		return false;
	if(abPiece>='A' && abPiece<='Z') //black piece
		if(cdPiece>='A' && cdPiece<='Z') //cd is black too
			return true;
	if(abPiece>='a' && abPiece<='z') //white piece
		if(cdPiece>='a' && cdPiece<='z') //cd is white too)
			return true;
	return false;
}
char *getPieceName(char piece){
	switch(piece){
		case 'm': return "pawn";
		case 'k': return "king";
		case 'q': return "queen";
		case 'b': return "bishop";
		case 'n': return "knight";
		case 'r': return "rook";
		default: return NULL;
	}
}
bool isKingThreatened(Game *game, int KingX, int KingY){
	int i,j;
	bool result;
	char piece;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(i==KingX && j==KingY) //Ignore the king's possition
				continue;
			if(sameColorPieces(game,KingX,KingY,i,j)) //not an opponent
				continue;

			piece=game->gameboard[i][j];
			if(piece==BLANK)
				continue;
			//in this point piece is an opponent
			//check if current piece can reach the king's position
			if(piece=='R' || piece=='r'){ //Rook
				result=RookValidMove(game,i,j,KingX,KingY);
				if(result)
					return true;
			}
			if(piece=='B' || piece=='b'){ //Bishop
				result=BishopValidMove(game,i,j,KingX,KingY);
				if(result)
					return true;
			}
			if(piece=='N' || piece=='n'){ //Knight
				result= KnightValidMove(game,i,j,KingX,KingY);
				if(result)
					return true;
			}
			if(piece=='Q' || piece=='q'){ //Queen
				result= QueenValidMove(game,i,j,KingX,KingY);
				if(result)
					return true;
			}
			if(piece=='M' || piece=='m'){ //Pawn
				result=PawnValidMove(game,i,j,KingX,KingY);
				if(result)
					return true;
			}
			if(piece=='K' || piece=='k'){ //King
				//if the opponent's king is one step away from the other king
				if((abs(i,KingX)<=1) && (abs(j,KingY)<=1))
					return true;
			}
		}
	}
	return false;
}
