#include <stdio.h>
#include <stdlib.h>
#include <goomba/gui.h>

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

void goomba_gui_dump( struct goomba_gui *gui ) {
	if( gui && gui->root ) {
		goomba_item_dump( gui->root );
	}
}

