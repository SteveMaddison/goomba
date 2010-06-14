#include <stdio.h>
#include <string.h>
#include <goomba/key.h>
#include <goomba/control.h>

static const char *device_str[]	= {
	"Unknown",
	"Key",
	"joy",
	"Mouse"
};
static const char *ctrl_str[] = {
	"Unknown",
	"Button",
	"Axis",
	"Hat",
	"Ball"
};
static const char *dir_str[] = {
	"Unknown",
	"Up",
	"Down",
	"Left",
	"Right"
};
static const char *axis_dir_plus = "+";
static const char *axis_dir_minus = "-";

const char *goomba_device_name( goomba_device_t device ) {
	if( device < 0 || device > GOOMBA_DEVS )
		return NULL;
	else
		return device_str[device];
}

const char *goomba_control_name( goomba_control_t control ) {
	if( control < 0 || control > GOOMBA_CTRLS )
		return NULL;
	else
		return ctrl_str[control];
}

const char *goomba_direction_name( goomba_direction_t dir ) {
	if( dir < 0 || dir > GOOMBA_DIRS )
		return NULL;
	else
		return dir_str[dir];
}

const char *goomba_axis_dir_name( int axis_dir ) {
	if( axis_dir < 0 )
		return axis_dir_minus;
	else if ( axis_dir > 0 )
		return axis_dir_plus;
	return NULL;
}

int goomba_control_string( char *buf, int size, struct goomba_control *control ) {
	if( control ) {
		switch( control->device ) {
			case GOOMBA_DEV_KEYBOARD:
				strncpy( buf, goomba_key_name( control->value ), size );
				return 0;
				break;
			case GOOMBA_DEV_JOYSTICK:
				
				break;
			case GOOMBA_DEV_MOUSE:
				
				break;
			default:
				break;
		}
		return 0;
	}
	return -1;
}

