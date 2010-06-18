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

#ifndef _FONT_H_
#define _FONT_H_ 1

#include <SDL/SDL.h>
#include <goomba/config.h>

typedef enum {
	GOOMBA_FONT_LARGE,
	GOOMBA_FONT_SMALL
} goomba_font_size_t;

#ifdef __cplusplus
extern "C" {
#endif

int goomba_font_init( struct goomba_config *config );
void goomba_font_free( void );
SDL_Surface *goomba_font_render( const char *text, goomba_font_size_t size  );

#ifdef __cplusplus
}
#endif

#endif

