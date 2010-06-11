#include <stdio.h>
#include <SDL/SDL.h>
#include <goomba/gui.h>

#define SCREEN_X 640
#define SCREEN_Y 480
#define SCREEN_BPP 32

SDL_Surface *screen = NULL;
SDL_Surface *loaded = NULL;
SDL_Surface *optimized = NULL;

int quit = 0;

int quit_callback( void ) {
	quit = 1;
}

int sdl_init( void ) {
	int mode = SDL_SWSURFACE;
	SDL_Rect offset;

	if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
		fprintf(stderr, "Error: Unable to initialise SDL: %s\n", SDL_GetError());
		return -1;
	}

	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_SetVideoMode( SCREEN_X, SCREEN_Y, SCREEN_BPP, mode );
	if( screen == NULL ) {
		fprintf(stderr, "Error: Unable to set video mode: %s\n", SDL_GetError());
		return -1;
	}

	loaded = SDL_LoadBMP( "test.bmp" );
	if( loaded != NULL ) {
		optimized = SDL_DisplayFormat( loaded );
		SDL_FreeSurface( loaded );
	}

	offset.x = (SCREEN_X - optimized->w) / 2;
	offset.y = (SCREEN_Y - optimized->h) / 2;

	SDL_BlitSurface( optimized, NULL, screen, &offset ); 
	SDL_Flip( screen );
}

int main( int argc, char *argv[] ) {
	struct goomba_gui *gui = goomba_gui_create();
	struct goomba_item *item_exit = goomba_item_create( GOOMBA_ACTION );
	struct goomba_item *item_number = goomba_item_create( GOOMBA_INT );
	struct goomba_item *item_enum = goomba_item_create( GOOMBA_ENUM );

	gui->config.background.fill.red = 255;

	item_exit->callback = quit_callback;
	item_exit->text = "Exit";

	goomba_add_enum_option( item_enum, "FIRST", 1 );
	goomba_add_enum_option( item_enum, "SECOND", 2 );
	goomba_add_enum_option( item_enum, "LAST", 3 );

	gui->root = goomba_item_create( GOOMBA_MENU );
	goomba_append_child( gui->root, item_number );
	goomba_append_child( gui->root, item_enum );
	goomba_append_child( gui->root, item_exit );

	goomba_gui_dump( gui );

	sdl_init();
	sleep( 1 );

	gui_start( gui, screen );
	sleep( 1 );

	SDL_Quit();

	return 0;
}

