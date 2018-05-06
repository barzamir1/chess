#include "SDLwindowsParameters.h"

int SDLmsg_YesNoMsg(char *msg){
	int buttonid;
	const SDL_MessageBoxButtonData buttons[] = {
		{ /* .flags, .buttonid, .text */        0, 0, "no" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "cancel" }
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL, /* .window */
		"warning", /* .title */
		msg, /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons, /* .buttons */
	};
	SDL_ShowMessageBox(&messageboxdata, &buttonid);
	return buttonid;
}

int SDLmsg_OKMsg(char *title, char * msg, SDL_Window *window){
	const SDL_MessageBoxButtonData buttons[]={{0, 0, "ok" }};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, window,	title, msg, SDL_arraysize(buttons), buttons,};
	int buttonPressed;
	SDL_ShowMessageBox(&messageboxdata, &buttonPressed);
		return buttonPressed;
}
