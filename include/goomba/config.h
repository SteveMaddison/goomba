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

#ifndef _GOOMBA_CONFIG_H_
#define _GOOMBA_CONFIG_H_ 1

#include <goomba/control.h>

typedef enum {
	GOOMBA_EVENT_UP,
	GOOMBA_EVENT_DOWN,
	GOOMBA_EVENT_LEFT,
	GOOMBA_EVENT_RIGHT,
	GOOMBA_EVENT_SKIP_F,
	GOOMBA_EVENT_SKIP_B,
	GOOMBA_EVENT_SELECT,
	GOOMBA_EVENT_BACK,
	GOOMBA_EVENT_QUIT,
	GOOMBA_EVENTS
} goomba_event_t;

struct goomba_config_rgb {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct goomba_config_background {
	struct goomba_config_rgb fill;
	char *image;
	unsigned char alpha;
};

struct goomba_config_font {
	char *name;
	int size;
	struct goomba_config_rgb color;
};

struct goomba_config_selector {
	struct goomba_config_rgb fill;
	unsigned char alpha;
};

struct goomba_config {
	struct goomba_config_background background;	
	struct goomba_config_font font;
	struct goomba_config_selector selector;
	struct goomba_control control[GOOMBA_EVENTS];
	int help;
};

#ifdef __cplusplus
extern "C" {
#endif

int goomba_config_init( struct goomba_config *config );

#ifdef __cplusplus
}
#endif

#endif

