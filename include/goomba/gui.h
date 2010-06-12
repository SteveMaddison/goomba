#ifndef _GOOMBA_GUI_H_
#define _GOOMBA_GUI_H_ 1

#include <SDL/SDL.h>
#include <goomba/config.h>
#include <goomba/item.h>

struct goomba_gui {
	struct goomba_config config;
	struct goomba_item *root;
};

struct goomba_gui *goomba_gui_create( void );
void goomba_gui_free( struct goomba_gui *gui );

int goomba_gui_start( struct goomba_gui *gui, SDL_Surface *s );

void goomba_gui_dump( struct goomba_gui *gui );

#endif

