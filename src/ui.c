#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "ui.h"

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
