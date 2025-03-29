#ifndef UI
#define UI

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

///////////////////
// IMAGE
///////////////////

typedef struct {
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_FRect crop;
	bool cropped;
} Img;

Img new_img(SDL_Renderer* rend, char* filename);
Img new_cropped_img(SDL_Renderer* rend, char* file, int x, int y, int w, int h);
void render_img(SDL_Renderer* rend, Img* img, int x, int y, int w, int h);

///////////////////
// ANIMATIONS
///////////////////

typedef struct {
	Img* frames;
	float frame;
	int framecount;
} Anim;

Anim new_anim(SDL_Renderer* rend, char* filename, int framecount, int row, int w, int h);
void render_anim(SDL_Renderer* rend, Anim* anim, int x, int y, int w, int h, float framerate);
void del_anim(Anim* anim);

#endif
