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

#ifndef _CONTROL_H_
#define _CONTROL_H_ 1

typedef enum {
	GOOMBA_DEV_UNKNOWN,
	GOOMBA_DEV_KEYBOARD,
	GOOMBA_DEV_JOYSTICK,
	GOOMBA_DEV_MOUSE,
	GOOMBA_DEVS
} goomba_device_t;

typedef enum {
	GOOMBA_CTRL_UNKNOWN,
	GOOMBA_CTRL_BUTTON,
	GOOMBA_CTRL_AXIS,
	GOOMBA_CTRL_HAT,
	GOOMBA_CTRL_BALL,
	GOOMBA_CTRLS
} goomba_control_t;

typedef enum {
	GOOMBA_DIR_UNKNOWN,
	GOOMBA_DIR_UP,
	GOOMBA_DIR_DOWN,
	GOOMBA_DIR_LEFT,
	GOOMBA_DIR_RIGHT,
	GOOMBA_DIRS
} goomba_direction_t;

struct goomba_control {
	goomba_device_t device_type;
	int device_id;
	goomba_control_t control_type;
	int control_id;
	int value;
};

#ifdef __cplusplus
extern "C" {
#endif

int goomba_control_string( char *buf, int size, struct goomba_control *control );

#ifdef __cplusplus
}
#endif

#endif

