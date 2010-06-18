/*
 * Goomba GUI/menu system
 * Copyright (c) 2010 Steve Maddison
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _GOOMBA_GUI_H_
#define _GOOMBA_GUI_H_ 1

#include <SDL/SDL.h>
#include <goomba/config.h>
#include <goomba/item.h>

struct goomba_gui {
	struct goomba_config config;
	struct goomba_item *root;
};

#ifdef __cplusplus
extern "C" {
#endif

struct goomba_gui *goomba_gui_create( void );
void goomba_gui_free( struct goomba_gui *gui );

int goomba_gui_start( struct goomba_gui *gui, SDL_Surface *s );
int goomba_gui_draw( void );
int goomba_gui_capture_control( struct goomba_control *control );
void goomba_gui_dump( struct goomba_gui *gui );

#ifdef __cplusplus
}
#endif

#endif

