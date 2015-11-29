
#include <stdio.h>
#include <sdl\SDL.h>
#include <sdl\SDL_ttf.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2Main.lib")
#pragma comment(lib, "SDL2_TTF.lib")

SDL_Window    *_window;
SDL_Renderer  *_renderer;
TTF_Font      *_monoFont;
unsigned char _run = 1;

static void load(); // loads assets
static void draw();
static void update();

int *create_text_texture(SDL_Renderer *renderer, const char *s, TTF_Font *font, SDL_Color fgColor, struct sprite_s *sprite);

struct sprite_s {
	int x;
	int y;
	int w;
	int h;
	SDL_Texture *texture;
};

struct sprite_s _text_sprite;

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "error: Unable to initialise SDL: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0) {
		fprintf(stderr, "error: Unable to initialise TTF support: %s\n", TTF_GetError());
		return 1;
	}

	_window = SDL_CreateWindow(
		"Hello World",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		0);
	if (_window == NULL) {
		fprintf(stderr, "error: Unable to create window: %s\n", SDL_GetError());
		return 1;
	}

	_renderer = SDL_CreateRenderer(
		_window,
		-1,
		SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL) {
		fprintf(stderr, "error: Unable to create renderer: %s\n", SDL_GetError());
		return 1;
	}

	load();

	while (_run != 0) {
		draw();
		update();
	}

	TTF_Quit();
	SDL_Quit();

	return 0;
}

int *create_text_texture(SDL_Renderer *renderer, const char *s, TTF_Font *font, SDL_Color fgColor, struct sprite_s *sprite) {

	SDL_Surface *surface = TTF_RenderText_Solid(
													font, 
													s, 
													fgColor);
	sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	sprite->x = 0;
	sprite->y = 0;
	TTF_SizeText(
		font,
		s,
		&sprite->w,
		&sprite->h);

	return 0;
}

static void draw() {

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);

	SDL_Rect dst = {
		_text_sprite.x,
		_text_sprite.y,
		_text_sprite.w,
		_text_sprite.h
	};

	SDL_RenderCopy(_renderer, _text_sprite.texture, NULL, &dst);

	SDL_RenderPresent(_renderer);

}

static void update() {

	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT)
			_run = 0;

		if (evt.type == SDL_KEYDOWN){
			if (evt.key.keysym.sym == SDLK_w)
				_text_sprite.y -= 10;
			if (evt.key.keysym.sym == SDLK_s)
				_text_sprite.y += 10;
		}

	}

}

// load assets
static void load() {

	_monoFont = TTF_OpenFont("C:\\Windows\\Fonts\\Consola.ttf", 14);
	if (_monoFont == NULL) {
		// failed to load font
		fprintf(stderr, "error: Unable to load font: %s\n", TTF_GetError());
		// TODO: return fail code?
	}

	SDL_Color white = { 0xFF, 0xFF, 0xFF };

	create_text_texture(_renderer, "Hello World", _monoFont, white, &_text_sprite);

}