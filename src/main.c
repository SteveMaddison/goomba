#include <stdio.h>
#include <goomba/gui.h>

int quit = 0;

int quit_callback( void ) {
	quit = 1;
}

int main( int argc, char *argv[] ) {
	struct goomba_gui *gui = goomba_gui_create();
	struct goomba_item *item_exit = goomba_item_create( GOOMBA_ACTION );
	struct goomba_item *item_number = goomba_item_create( GOOMBA_INT );
	struct goomba_item *item_enum = goomba_item_create( GOOMBA_ENUM );

	item_exit->action_data.callback = quit_callback;
	item_exit->text = "Exit";

	goomba_add_enum_option( item_enum, "FIRST", 1 );
	goomba_add_enum_option( item_enum, "SECOND", 2 );
	goomba_add_enum_option( item_enum, "LAST", 3 );

	gui->root = goomba_item_create( GOOMBA_MENU );
	goomba_append_child( gui->root, item_number );
	goomba_append_child( gui->root, item_enum );
	goomba_append_child( gui->root, item_exit );

	goomba_gui_dump( gui );

	return 0;
}

