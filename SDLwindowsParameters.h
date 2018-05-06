
#ifndef SDLWINDOWSPARAMETERS_H_
#define SDLWINDOWSPARAMETERS_H_
#include <SDL.h>
#include "Game.h"

typedef enum SDLwindowsSwitch {
	SDLmainWindow,
	SDLloadGameWindow,
	SDLgameWindow,
	SDLsettingWindow,
	SDLquitAllWindows
} SDLwindows;
typedef struct window_parameters_t{
	SDLwindows fromWindow;
	SDLwindows toWindow;
	Game *game;
}SDLwindowParameters;
int SDLmsg_YesNoMsg(char *msg);
int SDLmsg_OKMsg(char *title, char * msg, SDL_Window *window);
#endif /* SDLWINDOWSPARAMETERS_H_ */
