#include "SDLloadGame.h"
#define slotNum 5

//variables
SDL_Window *loadWindow=NULL;
SDL_Renderer *loadWindowRenderer=NULL;
SDL_Rect load_loadButtonRec={.x=130, .y=500, .w=250, .h=60};
SDL_Rect backButtonRec={.x=420, .y=500, .w=250, .h=60};
SDL_Rect gameSlotRect[slotNum];
char **savedGameList=NULL;
SDLload_buttons selectedSlot=SDLload_noButton;
SDL_Event e;

void SDLload_initialize(SDLwindowParameters *windowParam){
	SDLload_buttons pressedButton;
	selectedSlot=SDLload_noButton;
	bool quit=false;
	SDLload_Create_loadGame_window();
	if(!SDLload_LoadButtons()){ //failed to render back and load buttons
		windowParam->toWindow=SDLquitAllWindows;
		SDLload_DestroyWindow();
		return;
	}
	savedGameList=getSavedGameFile(); //load list of XML files
	if(savedGameList)
		SDLload_LoadGameslotButtons(slotNum+1); //update game slot buttons
	while(!quit){
		pressedButton=SDLload_SetEvents(); //the button pressed
		quit=executeButtonClick(windowParam,pressedButton);
	}
	SDLload_DestroyWindow();
	return;
}
int SDLload_Create_loadGame_window(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	loadWindow=SDL_CreateWindow("Load Game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800,600, SDL_WINDOW_OPENGL);
	if(loadWindow==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
		return -1;
	}
	loadWindowRenderer=SDL_CreateRenderer(loadWindow, -1, SDL_RENDERER_ACCELERATED);
	if(loadWindowRenderer==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_DestroyWindow(loadWindow);
		SDL_Quit();
		exit(0);
		return -1;
	}
	SDL_SetRenderDrawColor(loadWindowRenderer,10,128,255,0); //blue background
	SDL_RenderClear(loadWindowRenderer);
	SDL_RenderPresent(loadWindowRenderer);
	return 1;
}
bool SDLload_LoadButtons(){
	char loadButtonPath[]="bmp/loadWindow_loadDe.bmp";
	if(selectedSlot!=SDLload_noButton){ //enable load button
		loadButtonPath[19]='E';
		loadButtonPath[20]='n';
	}
	//load bmp
	SDL_Surface* load_loadButtonSurface = SDL_LoadBMP(loadButtonPath);
	SDL_Surface* backButtonSurface = SDL_LoadBMP("bmp/loadWindow_backEnabled.bmp");
	if(!load_loadButtonSurface || !backButtonSurface){
		printf( "ERROR: Unable to load image %s! SDL Error: %s\n", loadButtonPath, SDL_GetError());
		SDLload_DestroySurface(load_loadButtonSurface, backButtonSurface);
		return false;
	}
	SDL_Texture* load_loadButtonTexture=SDL_CreateTextureFromSurface(loadWindowRenderer,load_loadButtonSurface);
	SDL_Texture* backButtonTexture=SDL_CreateTextureFromSurface(loadWindowRenderer,backButtonSurface);
	if(!load_loadButtonTexture || !backButtonTexture){
		printf( "ERROR: Unable to load image %s! SDL Error: %s\n", loadButtonPath, SDL_GetError());
		SDLload_DestroySurface(load_loadButtonSurface, backButtonSurface);
		SDLload_DestroyTexture(load_loadButtonTexture, backButtonTexture);
		return false;
	}
	SDL_RenderCopy(loadWindowRenderer,load_loadButtonTexture,NULL, &load_loadButtonRec);
	SDL_RenderCopy(loadWindowRenderer,backButtonTexture,NULL, &backButtonRec);
	SDL_RenderPresent(loadWindowRenderer);
	//free
	SDLload_DestroySurface(load_loadButtonSurface, backButtonSurface);
	SDLload_DestroyTexture(load_loadButtonTexture, backButtonTexture);
	return true;
}
int SDLload_LoadGameslotButtons(int enabledSlot){
	int i, slotCount=0;
	SDL_Surface *slotSurface=NULL;
	SDL_Texture*slotTexture=NULL;
	char bmpPath[]="bmp/loadWindow_gameSlot1De.bmp"; //De=disabled buttons, En=enabled
	for(i=0; i<slotNum; i++){
		if(savedGameList[i]==NULL)
			continue;
		//create Rect
		gameSlotRect[i].x=275;
		gameSlotRect[i].y=10+70*i;
		gameSlotRect[i].h=50;
		gameSlotRect[i].w=250;
		slotCount++;
		//load bmp
		bmpPath[23]=(i+1+'0'); //slot number
		if(enabledSlot==i){ //choose the enabled button bmp
			bmpPath[24]='E';
			bmpPath[25]='n';
		}
		slotSurface = SDL_LoadBMP(bmpPath);
		bmpPath[24]='D';
		bmpPath[25]='e';
		if(!slotSurface)
			printf( "ERROR: Unable to load image %s! SDL Error: %s\n", bmpPath, SDL_GetError());
		//create texture
		slotTexture=SDL_CreateTextureFromSurface(loadWindowRenderer,slotSurface);
		SDL_RenderCopy(loadWindowRenderer,slotTexture,NULL, &gameSlotRect[i]);
	}
	SDL_RenderPresent(loadWindowRenderer);

	//free
	SDL_FreeSurface(slotSurface);
	SDL_DestroyTexture(slotTexture);
	if(slotCount==0)
		SDLmsg_OKMsg("Warning","there are no saved games. press Back and start a new game", loadWindow);
	return slotCount;
}
int SDLload_DestroyWindow(){
	SDL_DestroyRenderer(loadWindowRenderer);
	SDL_DestroyWindow(loadWindow);
	if(savedGameList!=NULL){
		destroy_savedGameList(savedGameList, slotNum);
}
	SDL_Quit();
	return 1;
}
void SDLload_DestroySurface(SDL_Surface *s1, SDL_Surface *s2){
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);
}
void SDLload_DestroyTexture(SDL_Texture *t1, SDL_Texture *t2){
	SDL_DestroyTexture(t1);
	SDL_DestroyTexture(t2);
}
SDLload_buttons SDLload_SetEvents(){
	int mouseX, mouseY;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				exit(0);
			if (e.type == SDL_MOUSEBUTTONDOWN){
				SDL_GetMouseState(&mouseX,&mouseY); //get mouse position

				if(mouseY>load_loadButtonRec.y && mouseY<load_loadButtonRec.y+load_loadButtonRec.h){
					//load button
					if(mouseX>load_loadButtonRec.x && mouseX<load_loadButtonRec.x+load_loadButtonRec.w)
						if(selectedSlot!=SDLload_noButton)
							return SDLload_loadButton;
					//back button
					if(mouseX>backButtonRec.x && mouseX<backButtonRec.x+backButtonRec.w)
						return SDLload_backButton;
				}
				//slot buttons
				return SDLload_SetGameSlotEvents(mouseX,mouseY);
			}
		}
	}
	return SDLload_noButton;
}
SDLload_buttons SDLload_SetGameSlotEvents(int mouseX, int mouseY){
	int i;
	if(savedGameList==NULL)
		return SDLload_noButton;
	if(mouseX>gameSlotRect[0].x && mouseX<gameSlotRect[0].x+gameSlotRect[0].w){
		for(i=0; i<5; i++){
			if(savedGameList[i]!=NULL){
				if(mouseY>gameSlotRect[i].y &&  mouseY<gameSlotRect[i].y+gameSlotRect[i].h)
					return SDLload_Gameslot1+i; //return the pressed button
			}
		}
	}
	return SDLload_noButton;
}

bool executeButtonClick(SDLwindowParameters *windowParam, SDLload_buttons pressedButton){
	if(pressedButton==SDLload_backButton){
		windowParam->toWindow=windowParam->fromWindow;  //go back to caller window
		windowParam->fromWindow=SDLloadGameWindow;
		return true; //quit window
	}
	//pressed button is one of the 5 slot buttons
	if(pressedButton>=SDLload_Gameslot1 && pressedButton<=SDLload_Gameslot5){
		//update selected slot button
		SDLload_LoadGameslotButtons(pressedButton-SDLload_Gameslot1);
		selectedSlot=pressedButton;
		SDLload_LoadButtons();
		return false; //don't quit window
	}
	//pressed button is LOAD button
	if(pressedButton==SDLload_loadButton){
		//load game from XML file
		char *FileName=savedGameList[selectedSlot-SDLload_Gameslot1];
		if(windowParam->game==NULL)
			windowParam->game=create_game(3);
		GAME_MESSAGE msg=executeLoadFromXML(windowParam->game, FileName);
		if(msg==GAME_INVALID_ARGUMENT){
			//failed to open file
			SDLmsg_OKMsg("Error", "Error: File doesn’t exist or cannot be opened\n",loadWindow);
			return false; //don't quit window
		}
		//load Succeeded. go to game window
		windowParam->fromWindow=SDLloadGameWindow;
		windowParam->toWindow=SDLgameWindow;
		return true;
	}
	return false;
}

