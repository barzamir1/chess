#ifndef MAINAUX_H_
#define MAINAUX_H_
#include <ctype.h>
#include "Game.h"
#include "GameMoves.h"
#include "Parser_setting_state.h"
#include "Parser_game_state.h"
#include "XMLfile.h"

int getSettingInput(Game *game, bool printErrors);
GAME_MESSAGE getGameInput(Game *game, bool printErrors);
int executeSettingCommand(Game *game, SettingCommand cmd);
GAME_MESSAGE executeGameCommand(Game *game, GameCommand cmd, bool printMsg);
GAME_MESSAGE executeMoveCommand(Game *game, GameCommand cmd, bool printMsg);
GAME_MESSAGE executeLoadFromXML(Game *game, char *srcFileName);
GAME_MESSAGE executeSaveXML(Game *game, char *srcFileName);
void printComputerMove(Game *game, GameCommand *cmd);
void Quit(Game *game, bool printMsg);
void Reset(Game *game);
#endif /* MAINAUX_H_ */
