#ifndef PARSER_GAME_STATE_H_
#define PARSER_GAME_STATE_H_
#include <stdbool.h>
#include "Game.h"

typedef enum {
	GAME_MOVE,
	GAME_SAVE_GAME,
	GAME_UNDO,
	GAME_QUIT,
	GAME_RESET,
	GAME_INVALID_LINE
} Game_Command;
typedef struct game_command_t {
	Game_Command cmd;
	bool validArg;
	int move_args[4]; //move command, <a,b> to <c,d>
	char *FileName; //save command only
	char *msg;
} GameCommand;

/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and saved in the field arg. the
 * field validArg is set to true. In any other case 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command contains a legal argument
 *   move_args      - in move command the array represents moving a piece from <move_args[0],move_args[1]>
 *   				  to <move_args[2],move_args[3]>.
 *   FileName - would contain the file name\path given by the user to save the current game.
 */
GameCommand ParseGameLine(char* str);
Game_Command getGameCommand(char *parsed_str);
bool parseMoveCommand(GameCommand *gameCmd,char *position, int arrIndex);
#endif /* PARSER_GAME_STATE_H_ */
