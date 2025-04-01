#ifndef OTTOGAME
#define OTTOGAME

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_keyboard.h>
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

///////////////////
// GAME
///////////////////

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
