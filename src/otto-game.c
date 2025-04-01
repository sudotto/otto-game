#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"

///////////////////
// IMAGE
///////////////////

Img new_img(SDL_Renderer* rend, char* file){
	Img img;
	img.surf = IMG_Load(file);
	img.tex = SDL_CreateTextureFromSurface(rend, img.surf);
	img.cropped = false;
	return img;
}

Img new_cropped_img(SDL_Renderer* rend, char* file, int x, int y, int w, int h){
	Img img;
	img.surf = IMG_Load(file);
	img.tex = SDL_CreateTextureFromSurface(rend, img.surf);
	img.crop.x = x;
	img.crop.y = y;
	img.crop.w = w;
	img.crop.h = h;
	img.cropped = true;
	return img;
}

void render_img(SDL_Renderer* rend, Img *img, int x, int y, int w, int h){
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	if(img->cropped){
		SDL_RenderTexture(rend, img->tex, &img->crop, &dest);
	} else {
		SDL_RenderTexture(rend, img->tex, NULL, &dest);
	}
}

///////////////////
// ANIMATION
///////////////////

Anim new_anim(SDL_Renderer* rend, char* filename, int framecount, int row, int w, int h){
	Anim anim;
	anim.frames = malloc(sizeof(Img) * framecount);
	anim.frame = 0;
	anim.framecount = framecount;

	for(int x = 0; x < framecount; x++){
		anim.frames[x] = new_cropped_img(rend, filename, x * w, row * h, w, h);
	}
	return anim;
}

void render_anim(SDL_Renderer* rend, Anim* anim, int x, int y, int w, int h, float framerate){
	anim->frame += framerate;
	if(anim->frame >= anim->framecount){
		anim->frame = 0;
	}
	Img test = new_img(rend, "assets/frank/idle.png");
	int frame = (int)floor(anim->frame);
	//printf("%i\n", frame);
	render_img(rend, &anim->frames[frame], x, y, w, h);
}

void del_anim(Anim* anim){
	free(anim->frames);
}

///////////////////
// GAME
///////////////////

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
