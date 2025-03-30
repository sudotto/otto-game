#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "ui.h"

#include "game.h"

Game new_game(char* title, int w, int h){
	Game game;

	SDL_Init(SDL_INIT_VIDEO);
	game.win = SDL_CreateWindow(title, w, h, 0);
	game.rend = SDL_CreateRenderer(game.win, NULL);

	game.icon = new_img(game.rend, "assets/icon.png");
	game.cursor = new_img(game.rend, "assets/cursor.png");
	SDL_SetWindowIcon(game.win, game.icon.surf);

	game.keystates = SDL_GetKeyboardState(NULL);
	game.mouse_x;
	game.mouse_y;
	SDL_HideCursor();

	game.frame_start;
	game.running = true;
	return game;
}

void cap_game_framerate(Uint8 fps, Uint32 frame_start){
	Uint32 frame_time = SDL_GetTicks() - frame_start;
	if(frame_time < 1000/fps){
		SDL_Delay(1000/fps - frame_time);
	}
}

bool get_game_events(Game* game){
	game->keystates = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&game->mouse_x, &game->mouse_y);
	if(SDL_PollEvent(&game->event)){
		return true;
	}
	return false;
}

void render_game_cursor(Game* game, int w, int h){
	render_img(game->rend, &game->cursor, game->mouse_x, game->mouse_y, w, h);
}

void clear_game(Game* game, Uint8 r, Uint8 g, Uint8 b){
	SDL_SetRenderDrawColor(game->rend, r, g, b, 1);
	SDL_RenderClear(game->rend);
}

void update_game(Game* game){
	SDL_RenderPresent(game->rend);
}

void destroy_game(Game* game){
	SDL_DestroyRenderer(game->rend);
	SDL_DestroyWindow(game->win);
}
