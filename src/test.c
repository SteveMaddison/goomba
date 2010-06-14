#include <stdio.h>
#include <SDL/SDL.h>
#include <goomba/control.h>
#include <goomba/gui.h>

#define SCREEN_X 800
#define SCREEN_Y 480
#define SCREEN_BPP 32
#define STRING_LEN 128
#define FILE_LEN 128

SDL_Surface *screen = NULL;
SDL_Surface *loaded = NULL;
SDL_Surface *optimized = NULL;

int quit = 0;

int quit_callback( void ) {
	quit = 1;
	return 0;
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
	if( loaded == NULL ) {
		fprintf(stderr, "Couldn't load test image: %s\n", SDL_GetError());
		return -1;
	}

	optimized = SDL_DisplayFormat( loaded );
	SDL_FreeSurface( loaded );

	offset.x = (SCREEN_X - optimized->w) / 2;
	offset.y = (SCREEN_Y - optimized->h) / 2;

	SDL_BlitSurface( optimized, NULL, screen, &offset ); 
	SDL_Flip( screen );

	return 0;
}

int main( int argc, char *argv[] ) {
	struct goomba_gui *gui = goomba_gui_create();
	struct goomba_item *item_exit = goomba_item_create( GOOMBA_ACTION );
	struct goomba_item *item_back = goomba_item_create( GOOMBA_ACTION );
	struct goomba_item *item_number = goomba_item_create( GOOMBA_INT );
	struct goomba_item *item_enum = goomba_item_create( GOOMBA_ENUM );
	struct goomba_item *item_submenu = goomba_item_create( GOOMBA_MENU );
	struct goomba_item *item_string = goomba_item_create( GOOMBA_STRING );
	struct goomba_item *item_ctrl = goomba_item_create( GOOMBA_CONTROL );
	struct goomba_item *item_file = goomba_item_create( GOOMBA_FILESEL );
	struct goomba_control control;

	int int_val = 0;
	int enum_val = 1;
	char buffer[STRING_LEN];
	char file[FILE_LEN];
	
	memset( buffer, 0, STRING_LEN-1 );
	memset( file, 0, FILE_LEN-1 );

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
	goomba_item_append_child( item_submenu, item_number );
	goomba_item_append_child( item_submenu, item_back );

	item_string->text = "Some string";
	item_string->string_data.value = &buffer[0];
	item_string->string_data.size = STRING_LEN;
	strcpy( buffer, "0123456789012345678901234567890123456789" );

	item_ctrl->text = "A control";
	control.device_type = GOOMBA_DEV_KEYBOARD;
	control.value = SDLK_PAGEUP;
	item_ctrl->control_data.control = &control;

	item_file->text = "File name";
	item_file->filesel_data.value = &file[0];
	item_file->filesel_data.size = FILE_LEN;

	gui->root = goomba_item_create( GOOMBA_MENU );
	gui->root->text = "Main Menu";

	goomba_item_append_child( gui->root, item_enum );
	goomba_item_append_child( gui->root, item_string );
	goomba_item_append_child( gui->root, item_ctrl );
	goomba_item_append_child( gui->root, item_file );
	goomba_item_append_child( gui->root, item_submenu );
	goomba_item_append_child( gui->root, item_exit );
	

	goomba_gui_dump( gui );

	sdl_init();
	goomba_gui_start( gui, screen );
	SDL_Quit();

	return 0;
}

