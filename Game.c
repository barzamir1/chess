#include "Game.h"

Game *create_game(int history_size){
	Game *game=(Game*)malloc(sizeof(Game));
	if(!game) //malloc failed
		return NULL;
	game->history=spArrayListCreate(2 * history_size); //2 (MoveHistory*) for each players' move
	if(!game->history){
		free(game);
		return NULL;
	}
	initializeBoard(game);
	initializeGameParam(game);
	return game;
}
Game * copy_game(Game *srcGame){
	Game *cpyGame=create_game(srcGame->history->maxSize);
	if(!cpyGame)
		return NULL;
	cpyGame->curr_player=srcGame->curr_player;
	cpyGame->difficulty=srcGame->difficulty;
	cpyGame->gameMode=srcGame->gameMode;
	cpyGame->user_color=srcGame->user_color;
	cpyGame->black_king_pos[0]=srcGame->black_king_pos[0];
	cpyGame->black_king_pos[1]=srcGame->black_king_pos[1];
	cpyGame->white_king_pos[0]=srcGame->white_king_pos[0];
	cpyGame->white_king_pos[1]=srcGame->white_king_pos[1];
	copyGameBoad(srcGame,cpyGame);
	spArrayListDestroy(cpyGame->history); //destroy the ArrayList malloced in create_game
	cpyGame->history=spArrayListCopy(srcGame->history); //malloc new ArrayList
	return cpyGame;
}
void destroy_game(Game *game){
	if(!game)
		return;
	spArrayListDestroy(game->history);
	free(game);
}

void print_settings(Game *game){
	printf("SETTINGS:\nGAME_MODE: %d\n", game->gameMode);
	if(game->gameMode==1)
		printf("DIFFICULTY_LVL: %d\nUSER_CLR: %s\n",
			game->difficulty, PlayerStirngToCapital(game->user_color));
}
void print_board(Game *game){
	int i,j;
	for(i=7; i>=0; i--){ //print top to bottom
		printf("%d| ", i+1);
		for(j=0; j<8; j++){
			printf("%c ",game->gameboard[i][j]);
		}
		printf("|\n");
	}
	printf("  -----------------\n");
	printf("   A B C D E F G H\n");
}

void initializeBoard(Game *game){
	int i, j;
	//white pieces
	strcpy(game->gameboard[0], "rnbqkbnr");
	strcpy(game->gameboard[1], "mmmmmmmm");
	//black pieces
	strcpy(game->gameboard[6], "MMMMMMMM");
	strcpy(game->gameboard[7], "RNBQKBNR");
	//empty cells
	for(i=2; i<6; i++){ //rows 2-5
		for(j=0; j<8; j++){
			game->gameboard[i][j]=BLANK;
		}
	}
	game->white_king_pos[0]=0; //starting kings' positions
	game->white_king_pos[1]=4;
	game->black_king_pos[0]=7;
	game->black_king_pos[1]=4;
	spArrayListClear(game->history); //clear history
	game->curr_player=WHITE_PLAYER; //white always starts
}
void initializeGameParam(Game *game){
	game->gameMode=1; //1-player
	game->difficulty=2;
	game->user_color=WHITE_PLAYER;
	game->curr_player=WHITE_PLAYER; //white player always starts
}
void copyGameBoad(Game *src, Game *target){
	int i;
	for(i=0; i<8; i++){
		strcpy(target->gameboard[i], src->gameboard[i]);
	}
}
