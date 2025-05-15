#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_image/SDL_image.h>

#include "otto-game.h"
#include "otto-game_info.h"

///////////////////
// BAR
///////////////////

void render_bar(SDL_Renderer* rend, float val, int max, int x, int y, int w, int h){
	int scaled = max * (w / max);
	SDL_FRect empty = {x, y, scaled, h};
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderFillRect(rend, &empty);
	scaled = val * (w / max);
	SDL_FRect time = {x, y, scaled, h};
	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
	SDL_RenderFillRect(rend, &time);
}

///////////////////
// UTILITY
///////////////////

int gen_rand(int min, int max){
	return rand() % (max - min + 1);
}

///////////////////
// CAMERA
///////////////////

Camera camera = {0, 0, 1};

///////////////////
// IMAGE
///////////////////

Img new_img(SDL_Renderer* rend, char* file, bool camera_affected){
	Img img;
	img.surf = IMG_Load(file);
	img.tex = SDL_CreateTextureFromSurface(rend, img.surf);
	img.cropped = false;
	img.flipped = false;
	img.camera_affected = camera_affected;
	img.angle = 0;
	return img;
}

void crop_img(Img* img, int x, int y, int w, int h){
	img->crop.x = x;
	img->crop.y = y;
	img->crop.w = w;
	img->crop.h = h;
	img->cropped = true;
}

void flip_img(Img* img, bool flipped){
	img->flipped = flipped;
}

void rotate_img(Img* img, int angle){
	img->angle = angle;
}

void recolor_img(Img* img, SDL_Renderer* rend, SDL_Color target, SDL_Color replace){
	const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(img->surf->format);
	Uint32 replaceRGB = SDL_MapRGB(format_details, NULL, replace.r, replace.g, replace.b);
	Uint32 targetRGB = SDL_MapRGB(format_details, NULL, target.r, target.g, target.b);
	Uint32 *pixels = img->surf->pixels;
	int total = img->surf->w * img->surf->h;
	for(int i = 0; i < total; i++){
		if(pixels[i] == targetRGB){
			pixels[i] = replaceRGB;
		}
	}
	img->cropped = false;
	img->tex = SDL_CreateTextureFromSurface(rend, img->surf);
}

void render_img(SDL_Renderer* rend, Img *img, int x, int y, int w, int h){
	SDL_FRect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	if(img->camera_affected){
		dest.x -= camera.x;
		dest.y -= camera.y;
		dest.x *= camera.scale;
		dest.y *= camera.scale;
		dest.w *= camera.scale;
		dest.h *= camera.scale;
	}
	if(x < 0 && y < 0 && x > GAME_W && y > GAME_H){
		return;
	}
	if(img->cropped){
		SDL_RenderTextureRotated(rend, img->tex, &img->crop, &dest, img->angle, NULL, img->flipped);
	} else {
		SDL_RenderTextureRotated(rend, img->tex, NULL, &dest, img->angle, NULL, img->flipped);
	}
}

///////////////////
// ANIMATION
///////////////////

Anim new_anim(SDL_Renderer* rend, char* filename, int framecount, int row, int w, int h, bool camera_affected){
	Anim anim;
	anim.frames = malloc(sizeof(Img) * framecount);
	anim.frame = 0;
	anim.framecount = framecount;

	for(int x = 0; x < framecount; x++){
		anim.frames[x] = new_img(rend, filename, camera_affected);
		crop_img(&anim.frames[x], x * w, row * h, w, h);
	}
	return anim;
}

void render_anim(SDL_Renderer* rend, Anim* anim, int x, int y, int w, int h, float framerate){
	anim->frame += framerate;
	if(anim->frame >= anim->framecount){
		anim->frame = 0;
	}
	int frame = (int)floor(anim->frame);
	render_img(rend, &anim->frames[(int)floor(anim->frame)], x, y, w, h);
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

	game.icon = new_img(game.rend, "assets/icon.png", false);
	game.cursor = new_img(game.rend, "assets/cursor.png", false);
	SDL_SetWindowIcon(game.win, game.icon.surf);

	game.keystates = SDL_GetKeyboardState(NULL);
	game.mousestates = SDL_GetMouseState(&game.mouse_x, &game.mouse_y);
	SDL_HideCursor();

	game.frame_start;
	game.running = true;
	return game;
}

void cap_game_framerate(Game* game, Uint8 fps){
	Uint32 frame_time = SDL_GetTicks() - game->frame_start;
	if(frame_time < 1000/fps){
		SDL_Delay(1000/fps - frame_time);
	}
}

bool get_game_events(Game* game){
	game->keystates = SDL_GetKeyboardState(NULL);
	game->mousestates = SDL_GetMouseState(&game->mouse_x, &game->mouse_y);
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
