#include "MainAux.h"

int getSettingInput(Game *game, bool printErrors){
	char user_input[1024];
	SettingCommand settingCmd;
	int result=1;
	while(1){
		fgets(user_input,1024,stdin);
		settingCmd=ParseSettingLine(user_input,game->gameMode);
		if(settingCmd.msg!=NULL && printErrors)
			printf("%s", settingCmd.msg);
		if(settingCmd.validArg){
			result=executeSettingCommand(game, settingCmd);
			if(settingCmd.FileName)
				free(settingCmd.FileName);
			return result;
		}
	}
}
GAME_MESSAGE getGameInput(Game *game, bool printErrors){
	char user_input[1024];
	GameCommand gameCmd;
	GAME_MESSAGE result;
	while(1){
		printf("%s player - enter your move:\n",PlayerStirng(game->curr_player));
		fgets(user_input,1024,stdin);
		gameCmd=ParseGameLine(user_input);
		if(gameCmd.msg!=NULL && printErrors)
			printf("%s", gameCmd.msg);
		if(gameCmd.validArg){
			result=executeGameCommand(game, gameCmd,true);
			if(gameCmd.FileName)
				free(gameCmd.FileName);
			if(result>=GAME_INVALID_MOVE && result <=GAME_NO_HISTORY)
				continue; //Illegal command. stay in the loop
			if(!(gameCmd.cmd==GAME_SAVE_GAME))
				return result;
		}
	}
}
int executeSettingCommand(Game *game, SettingCommand cmd){
	//GAME_MESSAGE result;
	switch(cmd.cmd){
		case SETTING_DIFFICULTY: game->difficulty=cmd.arg;
			free(cmd.msg);
			break;
		case SETTING_GAME_MODE: game->gameMode=cmd.arg;
			break;
		case SETTING_USER_COLOR: game->user_color=cmd.arg;
			break;
		case SETTING_DEFAULT: initializeGameParam(game);
			break;
		case SETTING_PRINT_SETTINGS: print_settings(game);
			break;
		case SETTING_LOAD_SETTINGS:
			executeLoadFromXML(game,cmd.FileName);
			return 0;
			break;
		case SETTING_QUIT: Quit(game, true);
			return -1;
			break;
		case SETTING_START:
			return 1;
			break;
		default: break;
	}
	return 0;
}
GAME_MESSAGE executeGameCommand(Game *game, GameCommand cmd, bool printMsg){
	GAME_MESSAGE msg;
	switch(cmd.cmd){
	case GAME_MOVE: return executeMoveCommand(game,cmd,printMsg);
		break;
	case GAME_QUIT: Quit(game, true);
		msg=GAME_QUIT_SUCCESS;
		break;
	case GAME_RESET: Reset(game);
		msg=GAME_RESET_SUCCESS; //go to setting state
		break;
	case GAME_SAVE_GAME: return executeSaveXML(game,cmd.FileName);
		break;
	case GAME_UNDO: msg=undoMove(game,true);
		if(msg==GAME_NO_HISTORY)
			printf("Empty history, move cannot be undone\n");
		if(msg==GAME_INVALID_ARGUMENT)
			printf("Undo command not available in 2 players mode\n"); //copy past from PDF file, "available".
		break;
	default:
		msg=GAME_SUCCESS;
		break;
	}
	return msg;
}
GAME_MESSAGE executeMoveCommand(Game *game, GameCommand cmd, bool printMsg){
	GAME_MESSAGE msg;
	int threatenedKing, winner;
	msg=SetMove(game, cmd.move_args[0],cmd.move_args[1],
						cmd.move_args[2],cmd.move_args[3]);
	switch(msg){
	case GAME_INVALID_ARGUMENT:
		if(printMsg)
			printf("Invalid position on the board\n");
		break;
	case GAME_NO_PIECE:
		if(printMsg)
			printf("The specified position does not contain your piece\n");
		break;
	case GAME_INVALID_MOVE:
		if(printMsg)
			printf("Illegal move\n");
		break;
	case GAME_SUCCESS:
		winner=isCheckMate(game);
		if(winner == BLACK_PLAYER || winner==WHITE_PLAYER){//-1 --> no winner
			if(printMsg)
				printf("Checkmate! %s player wins the game\n", PlayerStirng(winner));
			msg=GAME_CHECKMATE;
		}
		else{ //no winner
			threatenedKing=isCheck(game);
			if(threatenedKing!=-1){ //-1 --> no check
				if(printMsg){
					if(game->curr_player==game->user_color && game->gameMode==1) //user's king is threatened
						printf("Check!\n");
					else
						printf("Check: %s King is threatened!\n", PlayerStirng(threatenedKing));
				}
				msg=(threatenedKing==WHITE_PLAYER ? GAME_WHITE_THREATEND : GAME_BLACK_THREATEND);
			}
			if(!hasValidMove(game)){
				if(printMsg)
					printf("The game is tied\n");
				msg=GAME_TIE;
			}
		}
		break;
	default: break;
	}
	return msg;
}
GAME_MESSAGE executeLoadFromXML(Game *game, char *srcFileName){
	int result=XMLreadFile(game,srcFileName);
	if(result==-1){
		printf("Error: File doesn't exist or cannot be opened\n"); //PDF file says "Eror:"
		return GAME_INVALID_ARGUMENT;
	}
	if(game->difficulty==5){
		printf("Expert level not supported\n");
		game->difficulty=2;
	}
	return GAME_SUCCESS;
}
GAME_MESSAGE executeSaveXML(Game *game, char *srcFileName){
	int result=XMLwriteFile(game,srcFileName);
	if(result==-1){
		printf("File cannot be created or modified\n");
		return GAME_INVALID_ARGUMENT;
	}
	return GAME_SUCCESS;
}
void Quit(Game *game, bool printMsg){
	if(printMsg) //only printed when user quits. not in checkmate.
		printf("Exiting...\n");
	destroy_game(game);
}
void Reset(Game *game){
	printf("Restarting...\n");
	initializeBoard(game);
	initializeGameParam(game);
}
void printComputerMove(Game *game, GameCommand *cmd){
	//Computer: move [pawn|bishop|knight|rook|queen] at <x,y> to <i,j>\n
	char piece=tolower(game->gameboard[cmd->move_args[0]][cmd->move_args[1]]);
	printf("Computer: move %s at <%d,%c> to <%d,%c>\n", getPieceName(piece),cmd->move_args[0]+1,
			cmd->move_args[1]+'A', cmd->move_args[2]+1, cmd->move_args[3]+'A');
}
