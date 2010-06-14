#include <stdio.h>
#include <string.h>
#include <goomba/key.h>
#include <goomba/control.h>

const char *goomba_control_dir( goomba_direction_t dir ) {
	switch( dir ) {
		case GOOMBA_DIR_UP:    return "Up";
		case GOOMBA_DIR_DOWN:  return "Down";
		case GOOMBA_DIR_LEFT:  return "Left";
		case GOOMBA_DIR_RIGHT: return "Right";
		default: break;
	}
	return "";
}

int goomba_control_string( char *buf, int size, struct goomba_control *control ) {
	*buf = 0;

	if( control ) {
		switch( control->device_type ) {
			case GOOMBA_DEV_KEYBOARD:
				strncpy( buf, goomba_key_name( control->value ), size );
				return 0;
				break;
			case GOOMBA_DEV_JOYSTICK:
				switch ( control->control_type ) {
					case GOOMBA_CTRL_BUTTON:
						snprintf( buf, size, "Joy(%d) Button %d", control->device_id, control->value );
						break;
					case GOOMBA_CTRL_AXIS:
						snprintf( buf, size, "Joy(%d) Axis(%d) %c", control->device_id, control->control_id,
							(control->value < 0) ? '-' : '+' );
						break;
					case GOOMBA_CTRL_HAT:
						snprintf( buf, size, "Joy(%d) Hat(%d) %s", control->device_id, control->control_id,
							goomba_control_dir( control->value ) );
						break;
					case GOOMBA_CTRL_BALL:
						snprintf( buf, size, "Joy(%d) Ball(%d) %s", control->device_id, control->control_id,
							goomba_control_dir( control->value ) );
						break;
					default:
						break;
				}
				break;
			case GOOMBA_DEV_MOUSE:
				switch ( control->control_type ) {
					case GOOMBA_CTRL_BUTTON:
						snprintf( buf, size, "Mouse(%d) Button %d", control->device_id, control->value );
						break;
					case GOOMBA_CTRL_AXIS:
						snprintf( buf, size, "Mouse(%d) Axis(%d) %c", control->device_id, control->control_id,
							(control->value < 0) ? '-' : '+' );
						break;	
					default:
						break;
				}
				break;
			default:
				break;
		}
		return 0;
	}
	return -1;
}

