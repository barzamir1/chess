#include "main.h"


int main(int argc, char **argv){
	if(argc>2){
		printf("wrong argument");
		return 0;
	}
	if(argc==1 || strcmp(argv[1],"-c")==0){
		mainLoop();
		return 0;
	}
	if(strcmp(argv[1],"-g")==0){
		GUIstart();
	}
	return 0;
}
int aAa=6;
int mainLoop(){
	Game *game=create_game(3);
	//print_board(game);
	int result=0, isSettingState=1, quit=0;
	printf("Specify game setting or type "
				"'start' to begin a game with the current setting:\n");
	while(quit==0){
		//setting state
		if(isSettingState){
			result=getSettingInput(game,true);
			if(result==-1) //user quit
				quit=1;
			if(result==1) //go to game state
				isSettingState=0;
		}
		//game state
		else{
			gameState(game, &result, &quit, &isSettingState);
		}
	}
	return 0;
}
void gameState(Game *game, int *result, int *quit, int *isSettingState){
	GAME_MESSAGE msg;
	if((game->gameMode==1 && game->curr_player==game->user_color) || game->gameMode==2){ //user's turn
		print_board(game);
		msg=getGameInput(game,true);
		if(msg==GAME_QUIT_SUCCESS ) //quit
			*quit=1;
		if(msg==GAME_RESET_SUCCESS){ //rest
			*isSettingState=1; //go to setting state
			printf("Specify game setting or type "
						"'start' to begin a game with the current setting:\n");
		}
		if(msg==GAME_CHECKMATE || msg==GAME_TIE){
			Quit(game, false);
			*quit=1;
		}
		return;
	}
	if(game->gameMode==1 && game->curr_player!=game->user_color){ //computer's turn
		computerMove(game, quit);
	}
}
void computerMove(Game *game, int *quit){
	GAME_MESSAGE msg;
	GameCommand *cmd = calculateMove(game);
	printComputerMove(game, cmd);
	msg=executeGameCommand(game,*cmd,true);
	free(cmd);
	if(msg==GAME_CHECKMATE || msg==GAME_TIE){
		Quit(game, false);
		*quit=1;
	}
}
