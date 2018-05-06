#include "SDLmain.h"
//variables
SDL_Window *mainWindow=NULL;
SDL_Renderer *mainWindowRenderer=NULL;
//SDL_Surface* mainGameSurface = NULL;

SDL_Rect newGameButtonRec={.x=275, .y=100, .w=250, .h=60};
SDL_Rect loadButtonRec={.x=275, .y=170, .w=250, .h=60};
SDL_Rect exitButtonRec={.x=275, .y=240, .w=250, .h=60};
SDL_Event e;

void SDLmain_initialize(SDLwindowParameters *windowParam){
	windowParam->fromWindow=SDLmainWindow;
	SDLmain_Create_main_window();
	if(!SDLmain_LoadButtons()){
		windowParam->toWindow=SDLquitAllWindows;
		return;
	}
	SDLmain_buttons pressedButton=SDLmain_SetEvents();
	if(pressedButton==SDLmain_exitButton){
		SDLmain_DestroyWindow();
		windowParam->toWindow=SDLquitAllWindows;
		return;
	}
	if(pressedButton==SDLmain_loadButton){
		SDLmain_DestroyWindow();
		windowParam->toWindow=SDLloadGameWindow;
		return;
	}
	if(pressedButton==SDLmain_newGameButton){
		SDLmain_DestroyWindow();
		windowParam->toWindow=SDLsettingWindow;
		return;
	}
}
int SDLmain_Create_main_window(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
			printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
			return 1;
	}
	mainWindow=SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 800,600, SDL_WINDOW_OPENGL);
	if(mainWindow==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_Quit();
		exit(0);
		return -1;
	}
	mainWindowRenderer=SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	if(mainWindowRenderer==NULL){
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
		exit(0);
		return -1;
	}
	return 1;
}
bool SDLmain_LoadButtons(){
	//load bmp
	SDL_Surface* newGameButtonSurface = SDL_LoadBMP("bmp/mainWindow_buttonNewGame.bmp");
	SDL_Surface* loadButtonSurface = SDL_LoadBMP("bmp/mainWindow_buttonLoadGame.bmp");
	SDL_Surface* exitButtonSurface = SDL_LoadBMP("bmp/mainWindow_buttonExit.bmp");
	if( !newGameButtonSurface || !loadButtonSurface || !exitButtonSurface){
		printf( "ERROR: Unable to load image %s! SDL Error: %s\n", "bmp/mainWindow_buttonNewGame.bmp", SDL_GetError());
		SDLmain_DestroySurface(newGameButtonSurface, loadButtonSurface, exitButtonSurface);
		return false;
	}
	//set image transparent background
	SDL_SetColorKey(newGameButtonSurface,SDL_TRUE,SDL_MapRGB(newGameButtonSurface->format, 255, 255, 255));
	SDL_SetColorKey(loadButtonSurface,SDL_TRUE,SDL_MapRGB(loadButtonSurface->format, 255, 255, 255));
	SDL_SetColorKey(exitButtonSurface,SDL_TRUE,SDL_MapRGB(exitButtonSurface->format, 255, 255, 255));

	SDL_Texture* newGameButtonTexture=SDL_CreateTextureFromSurface(mainWindowRenderer,newGameButtonSurface);
	SDL_Texture* loadButtonTexture=SDL_CreateTextureFromSurface(mainWindowRenderer,loadButtonSurface);
	SDL_Texture* exitTexture=SDL_CreateTextureFromSurface(mainWindowRenderer,exitButtonSurface);
	if(!newGameButtonTexture || !loadButtonTexture || !exitTexture){
		printf( "ERROR: Unable to load image %s! SDL Error: %s\n", "test", SDL_GetError());
		SDLmain_DestroySurface(newGameButtonSurface, loadButtonSurface, exitButtonSurface);
		SDLmain_DestroyTexture(newGameButtonTexture, loadButtonTexture, exitTexture);
		return false;
	}
	SDL_SetRenderDrawColor(mainWindowRenderer,10,128,255,1); //blue background
	SDL_RenderClear(mainWindowRenderer);
	SDL_RenderCopy(mainWindowRenderer,newGameButtonTexture,NULL, &newGameButtonRec);
	SDL_RenderCopy(mainWindowRenderer,loadButtonTexture,NULL, &loadButtonRec);
	SDL_RenderCopy(mainWindowRenderer,exitTexture,NULL, &exitButtonRec);
	SDL_RenderPresent(mainWindowRenderer);
	//destroy texture and surface
	SDLmain_DestroySurface(newGameButtonSurface, loadButtonSurface, exitButtonSurface);
	SDLmain_DestroyTexture(newGameButtonTexture, loadButtonTexture, exitTexture);
	return true;
}

int SDLmain_DestroyWindow(){
	SDL_DestroyRenderer(mainWindowRenderer);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 1;
}
void SDLmain_DestroySurface(SDL_Surface *s1,SDL_Surface *s2,SDL_Surface *s3){
	SDL_FreeSurface(s1);
	SDL_FreeSurface(s2);
	SDL_FreeSurface(s3);

}
void SDLmain_DestroyTexture(SDL_Texture *t1,SDL_Texture *t2,SDL_Texture *t3){
	SDL_DestroyTexture(t1);
	SDL_DestroyTexture(t2);
	SDL_DestroyTexture(t3);
}
SDLmain_buttons SDLmain_SetEvents(){
	int mousX, mouseY;
	while(1){
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				exit(0);
			if (e.type == SDL_MOUSEBUTTONDOWN){
				SDL_GetMouseState(&mousX,&mouseY); //get mouse position

				if(mousX>newGameButtonRec.x && mousX<newGameButtonRec.x+newGameButtonRec.w){
					//new game button
					if(mouseY>newGameButtonRec.y && mouseY<newGameButtonRec.y+newGameButtonRec.h)
						return SDLmain_newGameButton;
					//load button
					if(mouseY>loadButtonRec.y && mouseY<loadButtonRec.y+loadButtonRec.h)
						return SDLmain_loadButton;
					//exit button
					if(mouseY>exitButtonRec.y && mouseY<exitButtonRec.y+exitButtonRec.h)
						return SDLmain_exitButton;
				}
			}
		}
	}
	return SDLmain_noButton;
}

