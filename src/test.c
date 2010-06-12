#include <stdio.h>
#include <SDL/SDL.h>
#include <goomba/gui.h>

#define SCREEN_X 800
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
	struct goomba_item *item_back = goomba_item_create( GOOMBA_ACTION );
	struct goomba_item *item_number = goomba_item_create( GOOMBA_INT );
	struct goomba_item *item_enum = goomba_item_create( GOOMBA_ENUM );
	struct goomba_item *item_submenu = goomba_item_create( GOOMBA_MENU );

	int int_val = 0;
	int enum_val = 1;

	gui->config.font.name = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";

	item_exit->callback = quit_callback;
	item_exit->text = "Exit";
	
	item_back->text = "Back";

	item_number->text = "INT option";
	item_number->int_data.max = 9;
	item_number->int_data.value = &int_val;

	item_enum->text = "My enum";
	goomba_add_enum_option( item_enum, "FIRST", 1 );
	goomba_add_enum_option( item_enum, "SECOND", 2 );
	goomba_add_enum_option( item_enum, "LAST", 3 );
	item_enum->enum_data.value = &enum_val;

	item_submenu->text = "Sub Menu";
	goomba_append_child( item_submenu, item_number );
	goomba_append_child( item_submenu, item_back );

	gui->root = goomba_item_create( GOOMBA_MENU );
	gui->root->text = "Main Menu";

	goomba_append_child( gui->root, item_enum );
	goomba_append_child( gui->root, item_submenu );
	goomba_append_child( gui->root, item_exit );
	

	goomba_gui_dump( gui );

	sdl_init();
	goomba_gui_start( gui, screen );
	SDL_Quit();

	return 0;
}

