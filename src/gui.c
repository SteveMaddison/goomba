#include <stdio.h>
#include <stdlib.h>
#include <goomba/gui.h>

static SDL_Surface *bg_original = NULL;
static SDL_Surface *bg = NULL;

struct goomba_gui *goomba_gui_create( void ) {
	struct goomba_gui *gui = malloc( sizeof(struct goomba_gui) );

	if( gui == NULL ) {
		fprintf( stderr, "Couldn't allocate memory for goomba_gui object.\n" );
	}
	else {
		goomba_config_init( &gui->config );
		gui->root = NULL;
	}

	return gui;
}

void goomba_gui_free( struct goomba_gui *gui ) {
	goomba_item_free( gui->root );
	free( gui );
	gui = NULL;	
}

void gui_event_loop( void ) {

}

int gui_start( struct goomba_gui *gui, SDL_Surface *screen ) {
	SDL_Surface *bg_fill = NULL;
	Uint32 bg_color = 0;

	/* Save the original screen. */
	bg_original = SDL_DisplayFormat( screen );
	if( bg_original == NULL ) {
		fprintf( stderr, "Couldn't copy goomba background surface.\n" );
		return -1;
	}

	/* Create our own background, using the original screen as a starting point. */
	bg = SDL_DisplayFormatAlpha( screen );
	if( bg == NULL ) {
		fprintf( stderr, "Couldn't create goomba background surface.\n" );
		return -1;
	}

	/* Surface used to fill the background. */
	bg_fill = SDL_DisplayFormatAlpha( screen );
	if( bg_fill == NULL ) {
		fprintf( stderr, "Couldn't create goomba background fill surface.\n" );
		return -1;
	}
	/* Apply the background colour and alpha value. */
	bg_color = SDL_MapRGBA( bg->format,
		gui->config.background.fill.red,
		gui->config.background.fill.green,
		gui->config.background.fill.blue,
		gui->config.background.alpha );
	SDL_FillRect( bg_fill, NULL, bg_color );

	/* Apply fill, surface can now be discarded */
	SDL_BlitSurface( bg_fill, NULL, bg, NULL );
	SDL_FreeSurface( bg_fill );

	SDL_BlitSurface( bg, NULL, screen, NULL );
	SDL_Flip( screen );

	/* Start the GUI running. */
	gui_event_loop();

	/* Restore the original screen. */
	SDL_BlitSurface( bg_original, NULL, screen, NULL );
	SDL_Flip( screen );

	/* Tidy up. */
	SDL_FreeSurface( bg );
	SDL_FreeSurface( bg_original );

	return 0;	
}

void goomba_gui_dump( struct goomba_gui *gui ) {
	if( gui && gui->root ) {
		goomba_item_dump( gui->root );
	}
}

