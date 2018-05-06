#include "XMLfile.h"
const char *savedGameFileName="saveList/savedGames.txt";

int XMLwriteFile(Game *game, char *fileName){
	int len,i;
	char *outputStr;
	FILE *out_file=fopen(fileName, "w");
	if(!game || !out_file)
		return -1;
	outputStr=(char*)malloc(1024*sizeof(char));
	sprintf(outputStr, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					   "<game>\n"
					   "\t<current_turn>%d</current_turn>\n"
					   "\t<game_mode>%d</game_mode>\n"
					   "\t<difficulty>%d</difficulty>\n"
					   "\t<user_color>%d</user_color>\n"
					   "\t<board>\n",game->curr_player, game->gameMode, game->difficulty,
					   game->user_color);

	//print board to String
	len=strlen(outputStr);
	for(i=7; i>=0; i--){
		sprintf(outputStr+len, "\t\t<row_%d>%-.8s</row_%d>\n", i+1, game->gameboard[i], i+1);
		len+=26; //length of a board row
	}
	sprintf(outputStr+len, "\t</board>\n");
	len=strlen(outputStr);

	//general label
	sprintf(outputStr+len, "\t<general>\n"
			"\t\t<black_position_x>%d</black_position_x>\n" //kings' positions
			"\t\t<black_position_y>%d</black_position_y>\n"
			"\t\t<white_position_x>%d</white_position_x>\n"
			"\t\t<white_position_y>%d</white_position_y>\n",
			game->black_king_pos[0], game->black_king_pos[1],
			game->white_king_pos[0], game->white_king_pos[1]);
	len=strlen(outputStr);

	//XML closing tags
	sprintf(outputStr+len, "\t</general>\n </game>");

	//write to file
	len=strlen(outputStr);
	fprintf(out_file,"%s", outputStr);
	free(outputStr);
	fclose(out_file);
	return 1;

}

int XMLreadFile(Game *game, char *fileName){
	FILE *in_file=fopen(fileName, "r");
	if(!game || !in_file)
		return -1;
	char line[50];
	char *tagStart=NULL;
	char *tagEnd=NULL;
	fgets(line,50,in_file); //ignore first line

	while(fgets(line,50,in_file)>0){
		tagStart=strstr(line,"<current_turn>");
		if(tagStart){
			tagEnd=strstr(line, "</current_turn>");
			if(tagEnd){
				game->curr_player=line[tagEnd-line-1]-'0';
				continue;
			}
		}
		tagStart=strstr(line,"<game_mode>");
		if(tagStart){
			tagEnd=strstr(line, "</game_mode>");
			if(tagEnd){
				game->gameMode=line[tagEnd-line-1]-'0';
				continue;
			}
		}
		tagStart=strstr(line,"<difficulty>");
		if(tagStart){
			tagEnd=strstr(line, "</difficulty>");
			if(tagEnd){
				game->difficulty=line[tagEnd-line-1]-'0';
				continue;
			}
		}
		tagStart=strstr(line,"<user_color>");
		if(tagStart){
			tagEnd=strstr(line, "</user_color>");
			if(tagEnd){
				game->user_color=line[tagEnd-line-1]-'0';
				continue;
			}
		}
		tagStart=strstr(line,"<row_");
		if(tagStart)
			XMLgetBoardRow(line,game);
		tagStart=strstr(line, "<black_position_x>");
		if(tagStart){
			tagEnd=strstr(line, "</black_position_x>");
			if(tagEnd){
				game->black_king_pos[0]=line[tagEnd-tagStart+1]-'0';
			}
		}
		tagStart=strstr(line, "<black_position_y>");
		if(tagStart){
			tagEnd=strstr(line, "</black_position_y>");
			if(tagEnd){
				game->black_king_pos[1]=line[tagEnd-tagStart+1]-'0';
			}
		}
		tagStart=strstr(line, "<white_position_x>");
		if(tagStart){
			tagEnd=strstr(line, "</white_position_x>");
			if(tagEnd){
				game->white_king_pos[0]=line[tagEnd-tagStart+1]-'0';
			}
		}
		tagStart=strstr(line, "<white_position_y>");
		if(tagStart){
			tagEnd=strstr(line, "</white_position_y>");
			if(tagEnd){
				game->white_king_pos[1]=line[tagEnd-tagStart+1]-'0';
			}
		}
	}
	fclose(in_file);
	//in case the XML file doesn't contain the kings' positions
	updateKingsPosition(game);
	return 1;
}

void XMLgetBoardRow(char line[], Game *game){
	int i;
	char *tagStart=strstr(line,"<row_");
	int rowNum=line[tagStart-line+5]-'0'-1;  //5 is for |<row_i>|
	for(i=0; i<8; i++)
		game->gameboard[rowNum][i]=line[(tagStart-line)+7+i]; //copy line to gameboard
}

void UpdateSavedGameFiles(char *newSavedGamePath){
	int i;
	char **fileList=getSavedGameFile(); //list of last 5 file names/paths
	if(!fileList)
		return;
	FILE *savedGamesFile=fopen(savedGameFileName, "w");
	fprintf(savedGamesFile,"%s\n", newSavedGamePath); //write the game path in the first line
	//write 4 newest game paths
	for(i=0; i<4; i++)
		if(fileList[i]!=NULL)
			fprintf(savedGamesFile,"%s\n", fileList[i]);
	fclose(savedGamesFile);
	destroy_savedGameList(fileList,5);
}
char **getSavedGameFile(){
	int currLen;
	FILE *savedGamesFile=fopen(savedGameFileName, "r"); //list of last 5 file names/paths
	if(!savedGamesFile)
		return NULL;
	char *currLine=(char*)malloc(50*sizeof(char));
	char **fileNames=(char**)calloc(5, sizeof(char*)); //only 5 slots
	if(!fileNames){
		fclose(savedGamesFile);
		return NULL;
	}
	int i=0;
	while(i<5){
		if(fgets(currLine,50,savedGamesFile)>0){
			currLen=strlen(currLine);
			fileNames[i]=(char*)calloc(currLen+1,sizeof(char));
			if(!fileNames[i]){ //malloc failed
				destroy_savedGameList(fileNames, i+1);
				fclose(savedGamesFile);
				return NULL;
			}
			if(currLine[currLen-1]=='\n') //remove new line character.
				currLine[currLen-1]=0;
			sprintf(fileNames[i], "%s", currLine);

			//strcpy(fileNames[i], currLine); //copy file name to list
		}
		else
			fileNames[i]=NULL;
		i++;
	}
	free(currLine);
	fclose(savedGamesFile);
	return fileNames;
}
char *getNewFilePath(){
	int i;
	char *filePath=(char*)malloc(sizeof(char)*20);
	strcpy(filePath, "saveList/game0.XML");
	char **fileList=getSavedGameFile(); //list of last 5 file names/paths
	for(i=0; i<5; i++){
		if(fileList[i]==NULL){
			filePath[13]=i+1+'0';
			destroy_savedGameList(fileList, i+1);
			return filePath;
		}
	}
	//here we know all 5 slots are taken. return oldest file path
	strcpy(filePath, fileList[4]);
	destroy_savedGameList(fileList, 5);
	return filePath;
}
void destroy_savedGameList(char **list, int count){
	int i;
	if(!list)
		return;
	for(i=0; i<count; i++)
		if(list[i]!=NULL)
			free(list[i]);
	free(list);
}
