#include "SDLgame.h"
#define squreDim 60

SDL_Window *gameWindow=NULL;
SDL_Renderer *gameWindowRenderer=NULL;
SDL_Renderer *gameMovingSquareRenderer=NULL;
SDL_Surface* gameBoardSurface = NULL;
SDL_Texture* gameBoardTexture=NULL;
SDL_Rect gameBoardRec={.x=245, .y=65, .w=500, .h=500};
SDL_Rect uHorizontalPaddingRec={.x=215, .y=29, .w=560, .h=37};
SDL_Rect dHorizontalPaddingRec={.x=215, .y=560, .w=560, .h=37};
SDL_Rect lVerticalPaddingRec={.x=215, .y=35, .w=37, .h=560};
SDL_Rect rVerticalPaddingRec={.x=738, .y=35, .w=37, .h=560};
SDL_Rect squaresRect[8][8];
SDL_Rect sideMenueRect[7]; //menu buttons rect
bool gameIsSaved;
int a=6; //for debugging computer move
SDL_Event e;

void SDLgame_initialize(SDLwindowParameters *windowsParam){
	bool quit=false;
	gameIsSaved=true; //no changes in board yet
	SDLgame_createWindow();
	SDLgame_createSideMenue(windowsParam);
	SDLgame_buildboard(windowsParam->game,-1,-1);
	if(windowsParam->game->curr_player!=windowsParam->game->user_color) //computer's turn
		if(windowsParam->game->gameMode==1) // 1-player mode
			quit = SDLgame_ComputerMove(windowsParam); //make computer's first move.
	while(!quit)
		quit=SDLgame_motionEvents(windowsParam);
	SDL_Quit();
	return;
}
int SDLgame_createWindow(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
				printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
				return 1;
		}
	gameWindow=SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800,600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	if(gameWindow==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
		return -1;
	}
	gameWindowRenderer=SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gameWindowRenderer==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_DestroyWindow(gameWindow);
		SDL_Quit();
		exit(0);
		return -1;
	}
	SDL_RenderClear(gameWindowRenderer);
	SDL_RenderPresent(gameWindowRenderer);
	return 1;
}
void SDLgame_buildboard(Game *game,int dragI, int dragJ){
	int i,j;
	SDL_Surface *pieceSurface=NULL;
	SDL_Surface *uHorizontalPadding=NULL;
	SDL_Surface *dHorizontalPadding=NULL;
	SDL_Surface *lVerticalPadding=NULL;
	SDL_Surface *rVerticalPadding=NULL;
	SDL_Texture *pieceTexture=NULL;
	SDL_Texture *uHPaddingTexture=NULL;
	SDL_Texture *dHPaddingTexture=NULL;
	SDL_Texture *lVPaddingTexture=NULL;
	SDL_Texture *rVPaddingTexture=NULL;
	char pieceBmpPath[]="bmp/pieces/PC.bmp"; //generic path, P-piece symbol, C-color
	char currPiece;
	//load game board background
	pieceSurface = SDL_LoadBMP("bmp/boardBackground.bmp");
	pieceTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,pieceSurface);
	SDL_RenderCopy(gameWindowRenderer,pieceTexture,NULL, &gameBoardRec);
	uHorizontalPadding = SDL_LoadBMP("bmp/horizontalPad.bmp");
	dHorizontalPadding = SDL_LoadBMP("bmp/horizontalPad.bmp");
	lVerticalPadding = SDL_LoadBMP("bmp/verticalPad.bmp");
	rVerticalPadding = SDL_LoadBMP("bmp/verticalPad.bmp");
	uHPaddingTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,uHorizontalPadding);
	dHPaddingTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,dHorizontalPadding);
	lVPaddingTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,lVerticalPadding);
	rVPaddingTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,rVerticalPadding);
	SDL_RenderCopy(gameWindowRenderer,uHPaddingTexture,NULL, &uHorizontalPaddingRec);
	SDL_RenderCopy(gameWindowRenderer,dHPaddingTexture,NULL, &dHorizontalPaddingRec);
	SDL_RenderCopy(gameWindowRenderer,lVPaddingTexture,NULL, &lVerticalPaddingRec);
	SDL_RenderCopy(gameWindowRenderer,rVPaddingTexture,NULL, &rVerticalPaddingRec);
	//load pieces images
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(i==dragI && j==dragJ){ //this piece is being dragged
				squaresRect[i][j].x=e.motion.x-squreDim/2;
				squaresRect[i][j].y=e.motion.y-squreDim/2;
			}
			else{ //normal square position
				squaresRect[i][j].x=gameBoardRec.x+j*(squreDim+2);
				squaresRect[i][j].y=gameBoardRec.h-i*(squreDim+2);
			}
			squaresRect[i][j].w=squreDim;
			squaresRect[i][j].h=squreDim;

			//set square piece image
			currPiece=game->gameboard[i][j];
			if(currPiece==BLANK)
				continue;
			if(isupper(currPiece)){ //BLACK piece
				pieceBmpPath[11]=tolower(currPiece);
				pieceBmpPath[12]='0';
			}
			else{ //WHITE piece
				pieceBmpPath[11]=currPiece;
				pieceBmpPath[12]='1';
			}
			//load Bmp
			pieceSurface = SDL_LoadBMP(pieceBmpPath);
			if(!pieceSurface)
				printf( "Unable to load image %s! SDL Error: %s\n", pieceBmpPath, SDL_GetError());
	        //set image transparent background
			SDL_SetColorKey(pieceSurface, SDL_TRUE, SDL_MapRGB( pieceSurface->format, 255, 255, 255 ) );
			pieceTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,pieceSurface);
			SDL_RenderCopy(gameWindowRenderer,pieceTexture,NULL, &squaresRect[i][j]);
		}
	}
	SDLgame_UpdateUndoCurrColorButton(game);
	//free
	SDL_FreeSurface(pieceSurface);
	SDL_DestroyTexture(pieceTexture);
	SDL_RenderPresent(gameWindowRenderer);
}
void SDLgame_createSideMenue(SDLwindowParameters *windowParam){
	int i, y=60;
	char *bmpPaths[]={"bmp/SDLgame_restartEn.bmp","bmp/SDLgame_saveEn.bmp","bmp/SDLgame_loadEn.bmp",
			"bmp/SDLgame_undoDe.bmp","bmp/SDLgame_mainEn.bmp", "bmp/SDLgame_exitEn.bmp"};
	//enable undo button
	sideMenueRect[6].x=10;
	sideMenueRect[6].y=450;
	sideMenueRect[6].w=200;
	sideMenueRect[6].h=50;
	SDLgame_UpdateUndoCurrColorButton(windowParam->game);
	SDL_Surface *currSurface=NULL;
	SDL_Texture *currTexture=NULL;
	SDL_SetRenderDrawColor(gameWindowRenderer,10,128,255,0); //blue background
	SDL_RenderClear(gameWindowRenderer);
	for(i=0; i<6; i++){
		sideMenueRect[i].x=10;
		sideMenueRect[i].y=y;
		sideMenueRect[i].w=200;
		sideMenueRect[i].h=50;

		currSurface=SDL_LoadBMP(bmpPaths[i]);
		currTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,currSurface);
		if(!currTexture || ! currSurface){
			SDLgame_SDLError(currSurface, currTexture,bmpPaths[i], true);
		}
		SDL_RenderCopy(gameWindowRenderer,currTexture,NULL, &sideMenueRect[i]);
		y+=60; //set spacing between buttons
	}
	SDL_RenderPresent(gameWindowRenderer);
}
void SDLgame_UpdateUndoCurrColorButton(Game *game){
	SDL_Surface *undoSurface, *currColorSurface;
	SDL_Texture *undoTexture, *currColorTexture;
	char undoPaths[]="bmp/SDLgame_undoDe.bmp";
	char currColorPath[]="bmp/SDLgame_turn0.bmp"; //black turn
	if(game->curr_player==WHITE_PLAYER)
		currColorPath[16]='1'; //white turn
	//enable undo button
	if(game->history->actualSize>0 && game->gameMode==1){
		undoPaths[16]='E'; //change to "bmp/SDLgame_undoEn.bmp"
		undoPaths[17]='n';
	}

	SDL_SetRenderDrawColor(gameWindowRenderer,10,128,255,0); //blue background
	undoSurface=SDL_LoadBMP(undoPaths);
	currColorSurface=SDL_LoadBMP(currColorPath);
	if(!undoSurface || !currColorSurface){
		SDL_FreeSurface(undoSurface);
		SDL_FreeSurface(currColorSurface);
		return;
	}
	//SDL_SetColorKey(currColorSurface, SDL_TRUE, SDL_MapRGB(currColorSurface->format, 255, 255, 255));
	undoTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,undoSurface);
	currColorTexture=SDL_CreateTextureFromSurface(gameWindowRenderer,currColorSurface);

	SDL_RenderCopy(gameWindowRenderer,undoTexture,NULL, &sideMenueRect[3]);
	SDL_RenderCopy(gameWindowRenderer,currColorTexture,NULL, &sideMenueRect[6]);
	SDL_RenderPresent(gameWindowRenderer);

	//free resources
	SDLgame_SDLError(undoSurface, undoTexture, undoPaths, false);
	SDLgame_SDLError(currColorSurface,currColorTexture,currColorPath,false);
}
void SDLgame_SDLError(SDL_Surface *surface, SDL_Texture *texture,char *filePath, bool printError){
	if(printError)
		printf( "ERROR: Unable to load image %s! SDL Error: %s\n", filePath, SDL_GetError());
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
void SDLgame_DestroyWindow(){
	SDL_FreeSurface(gameBoardSurface);
	SDL_DestroyTexture(gameBoardTexture);
	SDL_DestroyRenderer(gameWindowRenderer);
	SDL_DestroyWindow(gameWindow);
}
bool SDLgame_motionEvents(SDLwindowParameters *windowParam){
	int mouseX, mouseY, delay=0;
	GameCommand cmd={.cmd=GAME_MOVE};
	bool isRightButtonDown=false, quit=false;
	while(!quit){
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				exit(0);
			if (e.type == SDL_MOUSEBUTTONDOWN){
				isRightButtonDown=true;
				SDL_GetMouseState(&mouseX,&mouseY); //get mouse position
				if(mouseY>gameBoardRec.y && mouseY<gameBoardRec.y+gameBoardRec.h){
					if(mouseX>gameBoardRec.x && mouseX<gameBoardRec.x+gameBoardRec.w){
						//mouse within the game board borders
						getSquare(&cmd.move_args[0], &cmd.move_args[1]); //get starting square
						continue;
					}
				}
				if(mouseX>=sideMenueRect[0].x && mouseX<=sideMenueRect[0].x+sideMenueRect[0].w){
					//mouse within the side menu borders
					quit=SDLgame_executeSideMenuClick(windowParam);
					if(quit)
						return quit;
				}
			}
			if(e.type==SDL_MOUSEMOTION && isRightButtonDown){
				SDL_GetMouseState(&mouseX,&mouseY);
				if(delay%10==0 &&
						mouseY>gameBoardRec.y && mouseY<gameBoardRec.y+gameBoardRec.h &&
						mouseX>gameBoardRec.x && mouseX<gameBoardRec.x+gameBoardRec.w){
					int i=cmd.move_args[0];
					int j=cmd.move_args[1];
					SDLgame_buildboard(windowParam->game,i,j);
				}
				delay++;
			}
			if(e.type==SDL_MOUSEBUTTONUP){
				SDLgame_buildboard(windowParam->game,-1,-1);
				isRightButtonDown=false;
				SDL_GetMouseState(&mouseX,&mouseY); //get mouse position
				if(mouseY>gameBoardRec.y && mouseY<gameBoardRec.y+gameBoardRec.h){
					if(mouseX>gameBoardRec.x && mouseX<gameBoardRec.x+gameBoardRec.w){
						//mouse within the game board borders
						quit=SDLgame_moveCommand(windowParam, cmd);
						if(quit)
							return true;
					}
				}
			}
		}
	}
	return false;
}

void getSquare(int *squreY,int *squreX){
	int window_hight, window_with;
	SDL_GetWindowSize(gameWindow,&window_with,&window_hight);
	int mouseX=e.button.x-gameBoardRec.x; //relative x position
	int mouseY=window_hight-e.button.y-gameBoardRec.y;//relative y position.
	*squreX=(mouseX/squreDim);
	*squreY=mouseY/squreDim;
}

bool SDLgame_moveCommand(SDLwindowParameters *windowParam, GameCommand cmd){
	GAME_MESSAGE result;
	Game *game=windowParam->game;
	int winner, threatenedKing;
	char *msg=(char*)malloc(sizeof(char)*40);
	gameIsSaved=false;
	if((game->curr_player==game->user_color && game->gameMode==1) ||
			game->gameMode==2){ //user's move
		getSquare(&cmd.move_args[2], &cmd.move_args[3]); //get end square
		cmd.validArg=true;
	}
	result=executeGameCommand(game,cmd, false);
	SDLgame_buildboard(game, -1,-1); //update game board

	if(result==GAME_WHITE_THREATEND || result==GAME_BLACK_THREATEND){
		threatenedKing=result==GAME_BLACK_THREATEND ? BLACK_PLAYER : WHITE_PLAYER;
		sprintf(msg, "Check: %s King is threatened!\n", PlayerStirng(threatenedKing));
		SDLmsg_OKMsg("check!", msg,gameWindow);
		free(msg);
		//return false; //don't quit the window
	}
	if(result==GAME_CHECKMATE){
		winner=isCheckMate(game);
		sprintf(msg, "Checkmate! %s player wins the game\n", PlayerStirng(winner));
		SDLmsg_OKMsg("Game Over!", msg,gameWindow);
		free(msg);
		windowParam->toWindow=SDLquitAllWindows;
		Quit(game,false);
		windowParam->game=NULL;
		return true; //quit
	}
	if(result==GAME_TIE){
		sprintf(msg, "The game is tied\n");
		SDLmsg_OKMsg("Game Over!", msg,gameWindow);
		free(msg);
		windowParam->toWindow=SDLquitAllWindows;
		Quit(windowParam->game,false);
		windowParam->game=NULL;
		return true; //quit
	}
	if(game->curr_player!=game->user_color)
		if(result==GAME_SUCCESS || result==GAME_BLACK_THREATEND || result==GAME_WHITE_THREATEND)
			if(windowParam->game->gameMode==1){ //make computer move
				return SDLgame_ComputerMove(windowParam);
		}
	return false; //don't quit the game
}
bool SDLgame_ComputerMove(SDLwindowParameters *windowParam){
	bool quit;
	GameCommand *cmd=calculateMove(windowParam->game);
	quit=SDLgame_moveCommand(windowParam, *cmd);
	if(quit){
		windowParam->fromWindow=SDLgameWindow;
		windowParam->toWindow=SDLmainWindow;
		windowParam->game=NULL;
	}
	free(cmd);
	return quit;
}
bool SDLgame_executeSideMenuClick(SDLwindowParameters *windowParam){
	int mouseY=e.button.y;
	int i, quitWithoutSaving=1;
	for(i=0; i<6; i++){ //find the pressed button rect
		if(mouseY>=sideMenueRect[i].y && mouseY<=sideMenueRect[i].y+sideMenueRect[i].h)
			break;
	}
	switch(i){
	case 0: //restart button
		initializeBoard(windowParam->game); //reset the game board.
		SDLgame_buildboard(windowParam->game, -1,-1);
		break;
	case 1: //save button
		gameIsSaved=SDLgame_saveGame(windowParam);
		break;
	case 2: //load button
		windowParam->fromWindow=SDLgameWindow;
		windowParam->toWindow=SDLloadGameWindow;
		SDLgame_DestroyWindow();
		return true; //quit window
	case 3: //Undo move
		undoMove(windowParam->game, false);
		SDLgame_buildboard(windowParam->game,-1,-1);
		return false; //don't quit the window
	case 4: //main menu
		if(!gameIsSaved)
			quitWithoutSaving=SDLmsg_YesNoMsg("do you want to save the game?");
		if(quitWithoutSaving==2) //user pressed "cancel"
			return false; //don't quit
		if(quitWithoutSaving==1) //user pressed "yes"
			gameIsSaved=SDLgame_saveGame(windowParam);

		windowParam->fromWindow=SDLgameWindow;
		windowParam->toWindow=SDLmainWindow;
		destroy_game(windowParam->game);
		windowParam->game=NULL;
		SDLgame_DestroyWindow();
		return true; //quit window
		break;
	case 5: //exit
		if(!gameIsSaved)
			quitWithoutSaving=SDLmsg_YesNoMsg("do you want to save the game?");
		if(quitWithoutSaving==2) //user pressed "cancel"
			return false; //don't quit
		if(quitWithoutSaving==1) //user pressed "yes"
			gameIsSaved=SDLgame_saveGame(windowParam);

		windowParam->fromWindow=SDLgameWindow;
		windowParam->toWindow=SDLquitAllWindows;
		destroy_game(windowParam->game);
		windowParam->game=NULL;
		SDLgame_DestroyWindow();
		return true; //quit window
		break;
	default:
		break;
	}
	return false;
}
bool SDLgame_saveGame(SDLwindowParameters *windowParam){
	char *filePath=getNewFilePath();
	GameCommand cmd={.cmd=GAME_SAVE_GAME, .FileName=filePath, .validArg=true};
	GAME_MESSAGE msg=executeGameCommand(windowParam->game,cmd,false);
	if(msg==GAME_INVALID_ARGUMENT){
		SDLmsg_OKMsg("Error", "File cannot be created or modified\n",gameWindow);
		return false;
	}
	UpdateSavedGameFiles(filePath); //updates the current path to be the first game slot.
	return true;
}
