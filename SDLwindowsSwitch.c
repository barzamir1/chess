#include "SDLwindowsSwitch.h"

void GUIstart(){
	SDLwindowParameters windowsParam;
	windowsParam.game=NULL;
	windowsParam.toWindow=SDLmainWindow;
	while(windowsParam.toWindow!=SDLquitAllWindows){
		switch(windowsParam.toWindow){
			case SDLmainWindow:
				SDLmain_initialize(&windowsParam); //open main window
				break;
			case SDLsettingWindow:
				if(windowsParam.game!=NULL)
					destroy_game(windowsParam.game);
				windowsParam.game=create_game(3);
				SDLsetting_initialize(&windowsParam);
				break;
			case SDLgameWindow:
				SDLgame_initialize(&windowsParam); //open game window
				break;
			case SDLloadGameWindow:
				SDLload_initialize(&windowsParam); //open load game window
				break;
			case SDLquitAllWindows:
				return;
		}
	}
}
