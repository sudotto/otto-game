#ifndef GAME
#define GAME

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "ui.h"

typedef struct {
	SDL_Window* win;
	SDL_Renderer* rend;
	SDL_Event event;
	Img icon;
	Img cursor;
	const bool* keystates;
	float mouse_x;
	float mouse_y;
	float old_mouse_x;
	float old_mouse_y;
	Uint32 frame_start;
	bool running;
} Game;

Game new_game(char* title, int w, int h);
void cap_game_framerate(Uint8 fps, Uint32 frame_start);
bool get_game_events(Game* game);
void render_game_cursor(Game* game, int w, int h);
void clear_game(Game* game, Uint8 r, Uint8 g, Uint8 b);
void update_game(Game* game);
void destroy_game(Game* game);

#endif
