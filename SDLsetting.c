#include "SDLsetting.h"
//variables
SDL_Window *settingWindow=NULL;
SDL_Renderer *settingWindowRenderer=NULL;
SDL_Rect setting_nextButtonRec={.x=130, .y=510, .w=250, .h=60};
SDL_Rect setting_backButtonRec={.x=420, .y=510, .w=250, .h=60};
SDL_Rect stepTitleRect={.x=150, .y=40, .w=524, .h=120};
SDL_Rect onePlayerButtonRect={.x=150, .y=180, .w=250, .h=80};
SDL_Rect twoPlayerButtonRect={.x=450, .y=180, .w=250, .h=80};
SDL_Rect difficultyRect[]={{.x=250, .y=160, .w=250, .h=70},{.x=250, .y=240, .w=250, .h=70},
						   {.x=250, .y=320, .w=250, .h=70},{.x=250, .y=400, .w=250, .h=70}};
SDL_Rect userColorBlackRect={.x=150, .y=250, .w=200, .h=160};
SDL_Rect userColorWhiteRect={.x=400, .y=250, .w=200, .h=160};
SDL_Event e;
bool nextEnabled, startEnabled;

void SDLsetting_initialize(SDLwindowParameters *windowParam){
	int result;
	bool quit=false;
	nextEnabled=false;
	startEnabled=false;
	SDLsetting_step currStep=SDLsetting_gameModeStep;
	SDLsetting_Create_Window();

	while(!quit){
		switch(currStep){
			case SDLsetting_gameModeStep:
				SDLsetting_loadBackNextButtons(false,SDLsetting_next);
				result=SDLsetting_loadGameModeButtons(SDLsetting_noButton);
				if(result==-1){
					quit=true;
					break;
				}
				currStep=SDLsetting_SetGameModeEvent(windowParam);
				nextEnabled=false;
				startEnabled=false;
				break;
			case SDLsetting_difficultyStep:
				SDLsetting_loadBackNextButtons(false,SDLsetting_next);
				result=SDLsetting_loadDifficultyButtons(SDLsetting_noButton);
				if(result==-1){
					quit=true;
					break;
				}
				currStep=SDLsetting_SetDifficultyEvent(windowParam);
				nextEnabled=false;
				startEnabled=false;
				break;
			case SDLsetting_userColorStep:
				SDLsetting_loadBackNextButtons(false,SDLsetting_start);
				SDLsetting_loadUserColorButtons(SDLsetting_noButton);
				if(result==-1){
					quit=true;
					break;
				}
				currStep=SDLsetting_SetUserColorEvent(windowParam);
				nextEnabled=false;
				break;
			case SDLsetting_startStep:
				windowParam->fromWindow=SDLsettingWindow;
				windowParam->toWindow=SDLgameWindow;
				quit=true;
				break;
			case SDLsetting_exitStep:
				windowParam->fromWindow=SDLsettingWindow;
				windowParam->toWindow=SDLmainWindow;
				quit=true;
				break;
		}
	}
	SDLsetting_DestroyWindow();
}
int SDLsetting_Create_Window(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	settingWindow=SDL_CreateWindow("Settings", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800,600, SDL_WINDOW_OPENGL);
	if(settingWindow==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
		return -1;
	}
	settingWindowRenderer=SDL_CreateRenderer(settingWindow, -1, SDL_RENDERER_ACCELERATED);
	if(settingWindowRenderer==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_DestroyWindow(settingWindow);
		SDL_Quit();
		exit(0);
		return -1;
	}
	SDL_SetRenderDrawColor(settingWindowRenderer,10,128,255,0); //blue background
	SDL_RenderClear(settingWindowRenderer);
	SDL_RenderPresent(settingWindowRenderer);
	return 1;
}

int SDLsetting_loadBackNextButtons(bool enabled, SDLsetting_buttons startOrNext){
	char BackNextButtonPath[][33]={"bmp/SDLsetting_nextDe.bmp","bmp/SDLsetting_backEn.bmp"};
	if(startOrNext==SDLsetting_start){ //show start button
		if(enabled) //enable start button
			strcpy(BackNextButtonPath[0], "bmp/SDLsetting_startEn.bmp");
		else
			strcpy(BackNextButtonPath[0], "bmp/SDLsetting_startDe.bmp");
	}
	if(startOrNext==SDLsetting_next){ //enable next button
		if(enabled){
			BackNextButtonPath[0][19]='E';
			BackNextButtonPath[0][20]='n';
		}
	}
	SDL_Rect rects[2];
	rects[0]=setting_nextButtonRec;
	rects[1]=setting_backButtonRec;
	return SDLsetting_loadButtons(BackNextButtonPath, rects, 2,true);
}
int SDLsetting_loadGameModeButtons(SDLsetting_buttons pressedButton){
	char gameModeButtonPath[][33]={"bmp/SDLsetting_gameMode.bmp", "bmp/SDLsetting_oneDe.bmp","bmp/SDLsetting_twoDe.bmp"};
	if(pressedButton==SDLsetting_onePlayer){ //load enable button
		gameModeButtonPath[1][18]='E';
		gameModeButtonPath[1][19]='n';
	}
	if(pressedButton==SDLsetting_twoPlayer){ //load enable button
		gameModeButtonPath[2][18]='E';
		gameModeButtonPath[2][19]='n';
	}
	SDL_Rect rects[3];
	rects[0]=stepTitleRect;
	rects[1]=onePlayerButtonRect;
	rects[2]=twoPlayerButtonRect;
	return SDLsetting_loadButtons(gameModeButtonPath, rects, 3,false);
}
int SDLsetting_loadDifficultyButtons(SDLsetting_buttons pressedButton){
	char difficultyButtonPath[5][33]={"bmp/SDLsetting_Difficulty.bmp"};
	strcpy(difficultyButtonPath[1],"bmp/SDLsetting_dif1De.bmp");
	strcpy(difficultyButtonPath[2],"bmp/SDLsetting_dif2De.bmp");
	strcpy(difficultyButtonPath[3],"bmp/SDLsetting_dif3De.bmp");
	strcpy(difficultyButtonPath[4],"bmp/SDLsetting_dif4De.bmp");
	if(pressedButton>=SDLsetting_difficulty1 && pressedButton<=SDLsetting_difficulty4){ //load enabled button
		difficultyButtonPath[pressedButton-SDLsetting_difficulty1+1][19]='E';
		difficultyButtonPath[pressedButton-SDLsetting_difficulty1+1][20]='n';
	}
	SDL_Rect rects[5]={stepTitleRect, difficultyRect[0],difficultyRect[1],difficultyRect[2],difficultyRect[3]};
	//SDLsetting_loadBackNextButtons(true,SDLsetting_next);
	return SDLsetting_loadButtons(difficultyButtonPath,rects, 5, false);
}
int SDLsetting_loadUserColorButtons(SDLsetting_buttons pressedButton){
	char userColorButtonPath[][33]={"bmp/SDLsetting_chooseColor.bmp", "bmp/SDLsetting_colorBlackDe.bmp","bmp/SDLsetting_colorWhiteDe.bmp"};
	if(pressedButton==SDLsetting_userColorBlack){ //load enable button
		userColorButtonPath[1][25]='E';
		userColorButtonPath[1][26]='n';
	}
	if(pressedButton==SDLsetting_userColorWhite){ //load enable button
		userColorButtonPath[2][25]='E';
		userColorButtonPath[2][26]='n';
	}
	SDL_Rect rects[3];
	rects[0]=stepTitleRect;
	rects[1]=userColorBlackRect;
	rects[2]=userColorWhiteRect;
	return SDLsetting_loadButtons(userColorButtonPath, rects, 3,false);
}
int SDLsetting_loadButtons(char buttonPath[][33], SDL_Rect *rectArray, int numOfButtons, bool renderClear){
	int i;
	SDL_Surface* currSurface;
	SDL_Texture* currTexture;
	if(renderClear)
		SDL_RenderClear(settingWindowRenderer);
	for(i=0; i<numOfButtons; i++){
		//load bmp
		currSurface=SDL_LoadBMP(buttonPath[i]);
		if(!currSurface){
			printf( "ERROR: Unable to load image! SDL Error: %s\n", SDL_GetError());
			return -1;
		}
		//set image transparent background
		SDL_SetColorKey(currSurface, SDL_TRUE, SDL_MapRGB( currSurface->format, 255, 255, 255 ) );
		currTexture=SDL_CreateTextureFromSurface(settingWindowRenderer,currSurface);
		if(!currTexture){
			printf( "ERROR: Unable to load image! SDL Error: %s\n", SDL_GetError());
			SDL_FreeSurface(currSurface);
			return -1;
		}
		SDL_RenderCopy(settingWindowRenderer,currTexture,NULL, &rectArray[i]);
		//free
		SDL_FreeSurface(currSurface);
		SDL_DestroyTexture(currTexture);
	}
	SDL_RenderPresent(settingWindowRenderer);
	return 1;
}

void SDLsetting_DestroyWindow(){
	SDL_DestroyRenderer(settingWindowRenderer);
	SDL_DestroyWindow(settingWindow);
}

SDLsetting_step SDLsetting_SetGameModeEvent(SDLwindowParameters *windowParam){
	int mouseX, mouseY;

	while(1){
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT)
			exit(0);
		if (e.type == SDL_MOUSEBUTTONDOWN){
			mouseX=e.button.x;
			mouseY=e.button.y;
			if(mouseY>onePlayerButtonRect.y && mouseY<onePlayerButtonRect.y+onePlayerButtonRect.h){
				if(mouseX>onePlayerButtonRect.x && mouseX<onePlayerButtonRect.x+onePlayerButtonRect.w){
					//onePlayer button
					windowParam->game->gameMode=1;
					SDLsetting_loadBackNextButtons(true,SDLsetting_next);
					SDLsetting_loadGameModeButtons(SDLsetting_onePlayer); //enable button
					nextEnabled=true;
					startEnabled=false;
				}
				if(mouseX>twoPlayerButtonRect.x && mouseX<twoPlayerButtonRect.x+twoPlayerButtonRect.w){
					//twoPlayer button
					windowParam->game->gameMode=2;
					SDLsetting_loadBackNextButtons(true,SDLsetting_start);
					SDLsetting_loadGameModeButtons(SDLsetting_twoPlayer); //enable button
					startEnabled=true;
					nextEnabled=false;
				}
			}
			if(mouseY>setting_nextButtonRec.y && mouseY<=setting_nextButtonRec.y+setting_nextButtonRec.h){
				if(mouseX>setting_nextButtonRec.x && mouseX<=setting_nextButtonRec.x+setting_nextButtonRec.w){
					// next/start button pressed
					if(nextEnabled || startEnabled){
						if(windowParam->game->gameMode==1)
							return SDLsetting_difficultyStep; //go to difficulty step
						else
							return SDLsetting_startStep; //start game
					}
				}
				if(mouseX>setting_backButtonRec.x && mouseX<=setting_backButtonRec.x+setting_backButtonRec.w){
					//back button
					return SDLsetting_exitStep; //go back to main menu
				}
			}
		}
	}
	}
}
SDLsetting_step SDLsetting_SetDifficultyEvent(SDLwindowParameters *windowParam){
	int mouseX, mouseY;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				exit(0);
			if (e.type == SDL_MOUSEBUTTONDOWN){
				mouseX=e.button.x;
				mouseY=e.button.y;
				bool dificultyPressed=false;
				if(mouseX>difficultyRect[0].x && mouseX<difficultyRect[0].x+difficultyRect[0].w){
					if(mouseY>difficultyRect[0].y && mouseY<difficultyRect[0].y+difficultyRect[0].h){
						windowParam->game->difficulty=1;
						dificultyPressed=true;
					}
					if(mouseY>difficultyRect[1].y && mouseY<difficultyRect[1].y+difficultyRect[1].h){
						windowParam->game->difficulty=2;
						dificultyPressed=true;
					}
					if(mouseY>difficultyRect[2].y && mouseY<difficultyRect[2].y+difficultyRect[2].h){
						windowParam->game->difficulty=3;
						dificultyPressed=true;
					}
					if(mouseY>difficultyRect[3].y && mouseY<difficultyRect[3].y+difficultyRect[3].h){
						windowParam->game->difficulty=4;
						dificultyPressed=true;
					}
					startEnabled=false;
					if(dificultyPressed){
						//render buttons
						SDLsetting_loadBackNextButtons(true,SDLsetting_next);
						SDLsetting_loadDifficultyButtons(SDLsetting_difficulty1+windowParam->game->difficulty-1); //enable button
						nextEnabled=true;
					}
				}
				if(mouseY>setting_nextButtonRec.y && mouseY<=setting_nextButtonRec.y+setting_nextButtonRec.h){
					if(mouseX>setting_nextButtonRec.x && mouseX<=setting_nextButtonRec.x+setting_nextButtonRec.w){
						// next/start button pressed
						if(nextEnabled || startEnabled)
							return SDLsetting_userColorStep;
					}
					if(mouseX>setting_backButtonRec.x && mouseX<=setting_backButtonRec.x+setting_backButtonRec.w){
						//back button
						return SDLsetting_gameModeStep; //go back game mode step
					}
				}
			}
		}
	}
}
SDLsetting_step SDLsetting_SetUserColorEvent(SDLwindowParameters *windowParam){
	int mouseX, mouseY;

	while(1){
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				exit(0);
			if (e.type == SDL_MOUSEBUTTONDOWN){
				mouseX=e.button.x;
				mouseY=e.button.y;
				if(mouseY>userColorBlackRect.y && mouseY<userColorBlackRect.y+userColorBlackRect.h){
					if(mouseX>userColorBlackRect.x && mouseX<userColorBlackRect.x+userColorBlackRect.w){
						//black button
						windowParam->game->user_color=BLACK_PLAYER;
						SDLsetting_loadBackNextButtons(true,SDLsetting_start); //show start button
						SDLsetting_loadUserColorButtons(SDLsetting_userColorBlack); //enable black button
						startEnabled=true;
						nextEnabled=false;
					}
					if(mouseX>userColorWhiteRect.x && mouseX<userColorWhiteRect.x+userColorWhiteRect.w){
						//white button
						windowParam->game->user_color=WHITE_PLAYER;
						SDLsetting_loadBackNextButtons(true,SDLsetting_start); //show start button
						SDLsetting_loadUserColorButtons(SDLsetting_userColorWhite); //enable white button
						startEnabled=true;
						nextEnabled=false;
					}
					break;
				}
				if(mouseY>setting_nextButtonRec.y && mouseY<=setting_nextButtonRec.y+setting_nextButtonRec.h){
					if(mouseX>setting_nextButtonRec.x && mouseX<=setting_nextButtonRec.x+setting_nextButtonRec.w){
						// next/start button pressed
						if(startEnabled || nextEnabled)
							return SDLsetting_startStep; //start game
					}
					if(mouseX>setting_backButtonRec.x && mouseX<=setting_backButtonRec.x+setting_backButtonRec.w){
						//back button
						return SDLsetting_difficultyStep; //go back to difficulty step
					}
				}
			}
		}
	}
}
