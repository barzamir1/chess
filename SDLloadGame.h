#ifndef SDLLOADGAME_H_
#define SDLLOADGAME_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

#include "XMLfile.h"
#include "Parser_setting_state.h"
#include "MainAux.h"
#include "SDLwindowsParameters.h"

typedef enum SDLload_button {
	SDLload_backButton,
	SDLload_loadButton,
	SDLload_Gameslot1,
	SDLload_Gameslot2,
	SDLload_Gameslot3,
	SDLload_Gameslot4,
	SDLload_Gameslot5,
	SDLload_noButton
} SDLload_buttons;

void SDLload_initialize(SDLwindowParameters *windowParam);
int SDLload_Create_loadGame_window();
bool SDLload_LoadButtons();
/*
 * loads buttons' bmp and renders them.
 */
int SDLload_LoadGameslotButtons(int enabledSlot);
/*
 * destroys the SDL_Window and SDL_Renderer of this window
 */
int SDLload_DestroyWindow();
void SDLload_DestroySurface(SDL_Surface *s1, SDL_Surface *s2);
void SDLload_DestroyTexture(SDL_Texture *t1, SDL_Texture *t2);
SDLload_buttons SDLload_SetEvents();
SDLload_buttons SDLload_SetGameSlotEvents(int mouseX, int mouseY);
/*
 * executes the command assigned to the pressed button.
 * @param SDLwindowParameters *windowParam - contains the destination window and a pointer to game.
 * @param SDLload_buttons pressedButton - the button the user clicked on
 * returns true if the operation suicided and the program should close this window.
 * 		   false if the operation failed or if the program should not close this window.
 */
bool executeButtonClick(SDLwindowParameters *windowParam, SDLload_buttons pressedButton);

#endif /* SDLLOADGAME_H_ */
