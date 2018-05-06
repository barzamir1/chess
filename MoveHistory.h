#ifndef MOVEHISTORY_H_
#define MOVEHISTORY_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct history_element_t{
	bool isEmpty; //non-used element
	int *moveArgs; //moved <a,b> to <c,d>
	char pieceAtSource; //the piece used to be at <a,b> before the move (can not be BLANK)
	char pieceAtTardget; //the piece used to be at <c,d> before the move (can be BLANK)
} HistoryElement;

HistoryElement * CreateMoveHistory(int a,int b, int c, int d, char pieceAtSource, char pieceAtTarget);
HistoryElement * CopyMoveHistory(HistoryElement *src);
void DestroyMoveHistory(HistoryElement *src);

#endif /* MOVEHISTORY_H_ */
