#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "ui.h"
#include "game.h"

int main(int argc, char* argv[]){
	Game game = new_game("Otto Game Template", 900, 600);
	while(game.running){
		game.frame_start = SDL_GetTicks();
		clear_game(&game, 0, 0, 255);

		while(get_game_events(&game)){
			switch(game.event.type){
				case SDL_EVENT_QUIT:
					game.running = false;
					break;
			}
		}
		render_img(game.rend, &game.icon, 100, 100, 256, 32);

		render_img(game.rend, &game.cursor, game.mouse_x, game.mouse_y, 32, 32);
		update_game(&game);
		cap_game_framerate(60, game.frame_start);
	}
	destroy_game(&game);
}
