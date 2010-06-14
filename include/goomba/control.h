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

int goomba_control_string( char *buf, int size, struct goomba_control *control );

#endif

