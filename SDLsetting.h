#ifndef SDLSETTING_H_
#define SDLSETTING_H_

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_test_images.h>

#include <stdbool.h>
#include "SDLwindowsParameters.h"
typedef enum SDLsetting_button {
	SDLsetting_onePlayer,
	SDLsetting_twoPlayer,
	SDLsetting_difficulty1,
	SDLsetting_difficulty2,
	SDLsetting_difficulty3,
	SDLsetting_difficulty4,
	SDLsetting_userColorBlack,
	SDLsetting_userColorWhite,
	SDLsetting_back,
	SDLsetting_next,
	SDLsetting_start,
	SDLsetting_noButton
} SDLsetting_buttons;
typedef enum SDLsetting_step {
	SDLsetting_gameModeStep,
	SDLsetting_difficultyStep,
	SDLsetting_userColorStep,
	SDLsetting_startStep,
	SDLsetting_exitStep
} SDLsetting_step;

void SDLsetting_initialize(SDLwindowParameters *windowParam);
int SDLsetting_Create_Window();
/*
 * generic method to load bmp files.
 * @param buttonPath[][] - an array of strings contains the paths to the bmp files
 * @param SDL_Rect *rectArray - an array of Rects contains the desired position on the screen of each image.
 * @param  int numOfButtons - the number of button (bmps) to load.
 * @param bool renderClear - clear the renderer (will delete previous images loaded).
 */
int SDLsetting_loadButtons(char buttonPath[][33], SDL_Rect *rectArray, int numOfButtons, bool renderClear);
/*
 * method to load the bmp of the Back and Next/Start button.
 * @param SDLsetting_buttons startOrNext - load the Next button (go the next step) or the Start button (start the game)
 * @param bool enabled - the Next/Start button is enabled. notice that Back button is always enabled.
 */
int SDLsetting_loadBackNextButtons(bool enabled, SDLsetting_buttons startOrNext);
/*
 * method to load the bmp of the game mode buttons.
 * @param SDLsetting_buttons pressedButton - if any of the buttons is pressed by user, the method
 * 											 would load the enabled image of that button.
 */
int SDLsetting_loadGameModeButtons(SDLsetting_buttons pressedButton);
/*
 * method to load the bmp of the difficulty buttons.
 * @param SDLsetting_buttons pressedButton - if any of the buttons is pressed by user, the method
 * 											 would load the enabled image of that button.
 */
int SDLsetting_loadDifficultyButtons(SDLsetting_buttons pressedButton);
/*
 * method to load the bmp of the user color buttons.
 * @param SDLsetting_buttons pressedButton - if any of the buttons is pressed by user, the method
 * 											 would load the enabled image of that button.
 */
int SDLsetting_loadUserColorButtons(SDLsetting_buttons pressedButton);
/*
 * destroys the window's SDL_Renderer and SDL_Window
 */
void SDLsetting_DestroyWindow();
/*
 * updates the game mode according to the pressed button.
 * when the button Back is pressed returns to the main window
 * when the button Next is pressed goes to the difficulty step.
 */
SDLsetting_step SDLsetting_SetGameModeEvent(SDLwindowParameters *windowParam);
/*
 * updates the game difficulty according to the pressed button.
 * when the button Back is pressed returns to the the game mode step.
 * when the button Next is pressed goes to the user color step.
 */
SDLsetting_step SDLsetting_SetDifficultyEvent(SDLwindowParameters *windowParam);
/*
 * updates the game user color according to the pressed button.
 * when the button Back is pressed returns to the the difficulty step.
 * when the button Start is pressed goes to the game window.
 */
SDLsetting_step SDLsetting_SetUserColorEvent(SDLwindowParameters *windowParam);
#endif /* SDLSETTING_H_ */
