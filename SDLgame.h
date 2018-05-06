#ifndef SDLGAME_H_
#define SDLGAME_H_
#include <stdbool.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Game.h"
#include "MiniMax.h"
#include "MainAux.h"
#include "Parser_game_state.h"
#include "SDLwindowsParameters.h"

void SDLgame_initialize(SDLwindowParameters *windowsParam);
int SDLgame_createWindow();
/*
 * loads bmp's and renders according to the gameBoard status.
 * @param Game *game - the current game
 * @param int ignoreI, int ingnoreJ - a square on the board that the method will not render.
 */
void SDLgame_buildboard(Game *game, int ignoreI, int ingnoreJ);
void SDLgame_createSideMenue(SDLwindowParameters *windowParam);
void SDLgame_UpdateUndoCurrColorButton(Game *game);
/*
 * frees the side menu buttons' texture and surface on SDL failure.
 */
void SDLgame_SDLError(SDL_Surface *surface, SDL_Texture *texture, char *filePath, bool printError);
bool SDLgame_motionEvents(SDLwindowParameters *windowParam);
void SDLgame_updateSquare(Game *game, int i, int j);
bool SDLgame_moveCommand(SDLwindowParameters *windowParam, GameCommand cmd);
bool SDLgame_ComputerMove(SDLwindowParameters *windowParam);
/*
 * executes operation depending on the button clicked on the side menu: load, save, etc.
 */
bool SDLgame_executeSideMenuClick(SDLwindowParameters *windowParam);
bool SDLgame_saveGame(SDLwindowParameters *windowParam);
void getSquare(int *squreY,int *squreX);
#endif /* SDLGAME_H_ */
