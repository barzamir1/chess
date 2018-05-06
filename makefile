CC = gcc
OBJS = main.o Game.o GameMoves.o GamePieces.o MainAux.o \
	MiniMax.o MoveHistory.o Parser_game_state.o \
	Parser_setting_state.o SDLgame.o SDLloadGame.o \
	SDLmain.o SDLsetting.o SDLwindowsSwitch.o \
	SDLwindowsParameters.o SPArrayList.o XMLfile.o
 
CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

EXEC = chessprog
all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.h Game.h MainAux.h SDLwindowsSwitch.h MiniMax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Game.o: Game.h SPArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameMoves.o: GameMoves.h Game.h GamePieces.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GamePieces.o: GamePieces.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MainAux.o: MainAux.h Game.h GameMoves.h Parser_setting_state.h Parser_game_state.h XMLfile.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MiniMax.o: MiniMax.h Game.h GameMoves.h GamePieces.h MainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MoveHistory.o: MoveHistory.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Parser_game_state.o: Parser_game_state.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Parser_setting_state.o: Parser_setting_state.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLgame.o: Game.h MainAux.h Parser_game_state.h SDLwindowsParameters.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLloadGame.o: SDLloadGame.h SDLsetting.c MainAux.h Parser_setting_state.h SDLwindowsParameters.h XMLfile.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLmain.o: SDLmain.h SDLwindowsParameters.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLsetting.o: SDLsetting.h SDLwindowsParameters.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLwindowsParameters.o: SDLwindowsParameters.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SDLwindowsSwitch.o: SDLwindowsSwitch.h SDLloadGame.h SDLgame.h SDLmain.h SDLsetting.h Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h MoveHistory.h
XMLfile.o: XMLfile.h Game.h
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
