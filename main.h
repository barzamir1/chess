/*
 * main.h
 *
 *  Created on: Aug 16, 2017
 *      Author: bar82
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "Game.h"
#include "MainAux.h"
#include "SDLwindowsSwitch.h"
#include "MiniMax.h"

int mainLoop();
void gameState(Game *game, int *result, int *quit, int *isSettingState);
void computerMove(Game *game, int *quit);

#endif /* MAIN_H_ */
