#ifndef PARSER_SETTING_STATE_H_
#define PARSER_SETTING_STATE_H_
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
typedef enum {
	SETTING_DIFFICULTY,
	SETTING_GAME_MODE,
	SETTING_USER_COLOR,
	SETTING_LOAD_SETTINGS,
	SETTING_DEFAULT,
	SETTING_PRINT_SETTINGS,
	SETTING_QUIT,
	SETTING_START,
	SETTING_INVALID_LINE
} Setting_Command;

typedef struct command_t{
	Setting_Command cmd;
	bool validArg;
	int arg;
	char *FileName; //load command only
	char *msg;
} SettingCommand;

/*
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command should contain a number (game_mode, user_color etc.)
 *   arg      - the integer argument in case validArg is set to true
 *   FileName - would contain the file name\path given by the user to load settings from.
 */
SettingCommand ParseSettingLine(char* str, int GameMode);
/*
 * Parses the first token of the line to one of the Setting command.
 * returns
 * Setting_Command (user_color, start etc.) or SETTING_INVALID_LINE on failure
 */
Setting_Command getSettingCommand(char *parsed_str);
/*
 * sets the SettingCommand's message and argument validation depending on the
 * correctness of the numeric argument.
 * @param SettingCommand *settingCmd - the current command including command type
 * @param char delim[4] - used to get the next token of the user input
 */
void numericArgument(SettingCommand *settingCmd, const char delim[4]);
/*
 * returns true if the given string represents a number.
 */
bool IsInt(const char* str);
/*
 * Parses a string representing a number to int. assumes the string is a valid number.
 */
int parseStringToInt(char *str);


#endif /* PARSER_SETTING_STATE_H_ */
