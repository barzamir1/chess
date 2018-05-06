#include "Parser_game_state.h"
#define isValidCol(x) (x>='A' && x<='H' ? 1 : 0)
#define isValidRow(x) (x>='1' && x<='8' ? 1 : 0)

GameCommand ParseGameLine(char* str){
	char *str1=malloc(1024*sizeof(char));
	char *parsed=NULL;
	const char delim[4] = " \t\r\n";
	GameCommand gameCmd; //initialize command
	gameCmd.cmd = GAME_INVALID_ARGUMENT;
	gameCmd.validArg=false;
	gameCmd.FileName=NULL;
	gameCmd.msg="Invalid command.\n"; ///////Find real message!!!

	strcpy(str1,str);
	parsed = strtok(str1, delim); //str Separated to strings by delimiters
	if(parsed==NULL){
		free(str1);
		return gameCmd;
	}
	gameCmd.cmd=getGameCommand(parsed);
	if(gameCmd.cmd==GAME_MOVE){
		parsed = strtok(NULL, delim); //next token, should be "<a,b>"
		if(parseMoveCommand(&gameCmd,parsed, 0)){
			parsed = strtok(NULL, delim); //should be "to"
			if(strcmp(parsed, "to")==0){
				parsed = strtok(NULL, delim); //should be "<c,d>"
				if(parseMoveCommand(&gameCmd,parsed, 2)){
					if(gameCmd.move_args[0]==gameCmd.move_args[2] &&
					   gameCmd.move_args[1]==gameCmd.move_args[3]){ //a=c, b=d
							gameCmd.msg="Illegal move\n";
							return gameCmd;
					}
					gameCmd.validArg=true;
					gameCmd.msg=NULL;
				}
			}
		}
	}
	if(gameCmd.cmd==GAME_SAVE_GAME){
		parsed = strtok(NULL, delim); //should be file name
		gameCmd.FileName=(char*)malloc(sizeof(char)*strlen(parsed)+1);
		strcpy(gameCmd.FileName, parsed);
		gameCmd.msg=NULL;
		gameCmd.validArg=true;
	}
	if(gameCmd.cmd==GAME_RESET || gameCmd.cmd==GAME_QUIT || gameCmd.cmd==GAME_UNDO){
		parsed = strtok(NULL, delim); //should be NULL
		if(parsed==NULL){
			gameCmd.validArg=true;
			gameCmd.msg=NULL;
		}
	}
	free(str1);
	return gameCmd;
}

Game_Command getGameCommand(char *parsed_str){
	if(strcmp(parsed_str,"move")==0)
		return GAME_MOVE;
	if(strcmp(parsed_str, "save")==0)
		return GAME_SAVE_GAME;
	if(strcmp(parsed_str, "undo")==0)
		return GAME_UNDO;
	if(strcmp(parsed_str, "reset")==0)
		return GAME_RESET;
	if(strcmp(parsed_str, "quit")==0)
		return GAME_QUIT;
	else
		return GAME_INVALID_LINE;
}
bool parseMoveCommand(GameCommand *gameCmd,char *position, int arrIndex){
	int len;
	if(strstr(position,",")==NULL) //position doesn't contain ","
		return false;
	len=strlen(position);
	if(position[0]!= '<' || position[len-1]!='>')
		return false;
	if(len<5 || !isValidRow(position[1]) || !isValidCol(position[3])){
		gameCmd->msg="Invalid position on the board\n";
		return false;
	}
	gameCmd->move_args[arrIndex]=position[1]-'0'-1; //a or c
	gameCmd->move_args[arrIndex+1]=position[3]-'A'; //b or d
	return true;
}
