#include "Parser_setting_state.h"

SettingCommand ParseSettingLine(char* str, int GameMode){
	char *str1=malloc(1024*sizeof(char));
	char *parsed=NULL;
	const char delim[4] = " \t\r\n";
	SettingCommand settingCmd; //initialize command
	settingCmd.cmd = SETTING_INVALID_LINE;
	settingCmd.validArg=false;
	settingCmd.arg = -1;
	settingCmd.FileName=NULL;
	settingCmd.msg="Invalid command.\n"; ///////Fine real message!!!

	strcpy(str1,str);
	parsed = strtok(str1, delim); //str Separated to strings by delimiters
	if(parsed==NULL){
		free(str1);
		return settingCmd;
	}
	settingCmd.cmd=getSettingCommand(parsed);
	if(GameMode==2){ //invalid commands in 2-players mode
		if(settingCmd.cmd==SETTING_DIFFICULTY ||settingCmd.cmd==SETTING_USER_COLOR)
			return settingCmd;
	}

	if(settingCmd.cmd==SETTING_GAME_MODE || settingCmd.cmd==SETTING_DIFFICULTY ||
			settingCmd.cmd==SETTING_USER_COLOR){
		numericArgument(&settingCmd,delim);
	}
	if(settingCmd.cmd==SETTING_LOAD_SETTINGS){
		parsed=strtok(NULL,delim); //file name
		settingCmd.FileName=(char*)malloc(sizeof(char)*strlen(parsed)+1);
		strcpy(settingCmd.FileName,parsed);
		settingCmd.validArg=true;
		settingCmd.msg=NULL;
	}
	if(settingCmd.cmd==SETTING_QUIT || settingCmd.cmd==SETTING_START ||
			settingCmd.cmd==SETTING_DEFAULT || settingCmd.cmd==SETTING_PRINT_SETTINGS){
		if(strtok(NULL,delim)==NULL){ //no other characters
			settingCmd.validArg=true;
			settingCmd.msg=NULL;
		}
	}
	free(str1);
	return settingCmd;
}
Setting_Command getSettingCommand(char *parsed_str){
	if(strcmp(parsed_str,"game_mode")==0)
		return SETTING_GAME_MODE;
	if(strcmp(parsed_str, "difficulty")==0)
		return SETTING_DIFFICULTY;
	if(strcmp(parsed_str, "user_color")==0)
		return SETTING_USER_COLOR;
	if(strcmp(parsed_str, "load")==0)
		return SETTING_LOAD_SETTINGS;
	if(strcmp(parsed_str, "default")==0)
		return SETTING_DEFAULT;
	if(strcmp(parsed_str, "print_setting")==0)
		return SETTING_PRINT_SETTINGS;
	if(strcmp(parsed_str, "quit")==0)
		return SETTING_QUIT;
	if(strcmp(parsed_str, "start")==0)
		return SETTING_START;
	else
		return SETTING_INVALID_LINE;
}
void numericArgument(SettingCommand *settingCmd, const char delim[4]){
	char *argument;
	argument=strtok(NULL,delim); //next token, should be a number
	if(IsInt(argument))
		settingCmd->arg=parseStringToInt(argument);
	if(settingCmd->cmd==SETTING_GAME_MODE){
		if(settingCmd->arg==1 || settingCmd->arg==2){ //game mode is either 1 or 2
			settingCmd->validArg=true;
			settingCmd->msg=malloc(29 *sizeof(char));
			char *players=(settingCmd->arg==1 ? "1 player" : "2 players");
			settingCmd->msg=strcat(settingCmd->msg, "Game mode is set to ");
			settingCmd->msg=strcat(settingCmd->msg, players);
			settingCmd->msg=strcat(settingCmd->msg, "\n");
		}
		else
			settingCmd->msg="Wrong game mode\n";
	}
	if(settingCmd->cmd==SETTING_DIFFICULTY){
		if(settingCmd->arg>0 && settingCmd->arg<6){ //difficulty is between 1 and 5
			if(settingCmd->arg<5){
				settingCmd->validArg=true;
				settingCmd->msg=NULL;
			}
			else
				settingCmd->msg="Expert level not supported, "
						"please choose a value between 1 to 4:\n";
		}
		else
			settingCmd->msg="Wrong difficulty level. The value should be between 1 to 5\n";
	}
	if(settingCmd->cmd==SETTING_USER_COLOR){
		if(settingCmd->arg==0 || settingCmd->arg==1){ //user color is either 0 or 1
			settingCmd->validArg=true;
			settingCmd->msg=NULL;
		}
	}
}
bool IsInt(const char* str){
	if(str==NULL)
		return false;
	int i=0;
	if(str[0]=='-') //negative number
		i=1;
	int len = strlen(str);
	for (; i < len; i++){
		if (str[i] < 48 || str[i] > 57)
			return false;
	}
	return true;
}
int parseStringToInt(char *str){
	int str_len=strlen(str);
	int num=0, i, decimal=1;
	for(i=str_len-1; i>=0; i--){
		num+=(str[i]-'0')*decimal;
		decimal*=10;
	}
	return num;
}
