#ifndef XML_H_
#define XML_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

/*
 * writes game setting and board to XML file.
 * @param Game *game - the current game to be stored.
 * @param chare *fileName - the name or full path to the file to save in.
 * returns -1 if the file can't be opened or 1 on success.
 */
int XMLwriteFile(Game *game, char *fileName);
/*
 * reads game setting and board from XML file.
 * @param Game *game - the read data would be stored into.
 * @param chare *fileName - the name or full path to the file to read from.
 * returns -1 if the file can't be opened or 1 on success.
 */
int XMLreadFile(Game *game, char *fileName);
/*
 * parses the given line to a row of the gameboard.
 * @param char line[] - a line read from the XML file, in format <row_i>XXX</row_i>
 * @param Game *game - the row of the game board will be saved in.
 */
void XMLgetBoardRow(char line[], Game *game);

/*
 * a helper method to maintain a file with 5-newest saved games
 * @param char *newSavedGamePath - the path/file name of the last saved game.
 */
void UpdateSavedGameFiles(char *newSavedGamePath);
/*
 * a helper method that reads the file with 5-newest saved games an returns their paths in array.
 */
char **getSavedGameFile();
/*
 * a helper method that returns a new path to save game XML file in
 */
char *getNewFilePath();
/*
 * a helper method to free the allocated memory neened for array getSavedGameFile returned.
 * @param char **list - the list of file paths to be freed
 * @param int count - number of elements in the array. should be<=5.
 */
void destroy_savedGameList(char **list, int count);

#endif
