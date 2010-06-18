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

#include <string.h>
#include <SDL/SDL.h>
#include <goomba/config.h>

int goomba_config_init( struct goomba_config *config ) {
	int i = 0;
	
	memset( config, 0, sizeof(struct goomba_config) );

	config->background.alpha  = 128;

	config->font.color.red    = 255;
	config->font.color.green  = 255;
	config->font.color.blue   = 255;

	config->selector.fill.red = 128;
	config->selector.alpha    = 128;

	for( i = 0; i < GOOMBA_EVENTS ; i++ ) {
		config->control[i].device_type = GOOMBA_DEV_KEYBOARD;
	}
	config->control[GOOMBA_EVENT_UP].value     = SDLK_UP;
	config->control[GOOMBA_EVENT_DOWN].value   = SDLK_DOWN;
	config->control[GOOMBA_EVENT_LEFT].value   = SDLK_LEFT;
	config->control[GOOMBA_EVENT_RIGHT].value  = SDLK_RIGHT;
	config->control[GOOMBA_EVENT_SKIP_F].value = SDLK_PAGEDOWN;
	config->control[GOOMBA_EVENT_SKIP_B].value = SDLK_PAGEUP;
	config->control[GOOMBA_EVENT_SELECT].value = SDLK_RETURN;
	config->control[GOOMBA_EVENT_BACK].value   = SDLK_BACKSPACE;
	config->control[GOOMBA_EVENT_QUIT].value   = SDLK_ESCAPE;

	return 0;
}

