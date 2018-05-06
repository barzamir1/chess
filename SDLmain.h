#ifndef SDLMAIN_H_
#define SDLMAIN_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_video.h>
#include "SDLwindowsParameters.h"
#include "Game.h"


typedef enum SDLmain_button {
	SDLmain_newGameButton,
	SDLmain_loadButton,
	SDLmain_exitButton,
	SDLmain_noButton
} SDLmain_buttons;


void SDLmain_initialize(SDLwindowParameters *windowPram);
int SDLmain_Create_main_window();
bool SDLmain_LoadButtons();
SDLmain_buttons SDLmain_SetEvents();
int SDLmain_DestroyWindow();
void SDLmain_DestroySurface(SDL_Surface *s1,SDL_Surface *s2,SDL_Surface *s3);
void SDLmain_DestroyTexture(SDL_Texture *t1,SDL_Texture *t2,SDL_Texture *t3);


#endif /* SDLMAIN_H_ */
