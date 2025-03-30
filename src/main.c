#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "ui.h"
#include "game.h"

int main(int argc, char* argv[]){
	Game game = new_game("Otto Game Template", 900, 600);
	int offset_x = 0;
	int offset_y = 0;
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
		if(game.mouse_x < 900 / 2){
			offset_x--;
		} else {
			offset_x++;
		}
		if(game.mouse_y < 600 / 2){
			offset_y++;
		} else {
			offset_y--;
		}
		if(offset_x > 64){
			offset_x = 0;
		}
		if(offset_y > 64){
			offset_y = 0;
		}
		for(int y = -1; y < 61; y++){
			for(int x = -1; x < 91; x++){
				render_img(game.rend, &game.icon, (x * 64) + offset_x, (y * 64) + offset_y, 64, 64);
			}
		}

		render_game_cursor(&game, 32, 32);
		update_game(&game);
		cap_game_framerate(60, game.frame_start);
	}
	destroy_game(&game);
}
