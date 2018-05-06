#include "MoveHistory.h"

HistoryElement * CreateMoveHistory(int a,int b, int c, int d,char pieceAtSource, char pieceAtTardget){
	HistoryElement *history=(HistoryElement*)malloc(sizeof(HistoryElement));
	if(!history) //malloc failed
		return NULL;
	history->moveArgs=(int*)malloc(4*sizeof(int)); //4 arguments
	if(!history->moveArgs){
		free(history);
		return NULL;
	}
	history->isEmpty=false;
	history->moveArgs[0]=a;
	history->moveArgs[1]=b;
	history->moveArgs[2]=c;
	history->moveArgs[3]=d;
	history->pieceAtSource=pieceAtSource;
	history->pieceAtTardget=pieceAtTardget;
	return history;
}
HistoryElement * CopyMoveHistory(HistoryElement *src){
	int a=src->moveArgs[0];
	int b=src->moveArgs[1];
	int c=src->moveArgs[2];
	int d=src->moveArgs[3];
	HistoryElement *copy=CreateMoveHistory(a,b,c,d,src->pieceAtSource, src->pieceAtTardget);
	copy->isEmpty=src->isEmpty;
	return copy;
}
void DestroyMoveHistory(HistoryElement *src){
	if(src!=NULL){
		if(src->moveArgs)
			free(src->moveArgs);
		free(src);
	}
}
void PrintHistoryMove(HistoryElement *element){ ////for DEBUG only
	int *args=element->moveArgs;
	if(element->isEmpty)
		return;
	printf("\t [moved <%d,%d> to <%d,%d>. old piece in <c,d>:%c]\n",args[0],args[1],args[2],args[3],element->pieceAtTardget);
}
