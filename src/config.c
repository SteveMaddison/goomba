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
	config->control[GOOMBA_UP].value     = SDLK_UP;
	config->control[GOOMBA_DOWN].value   = SDLK_DOWN;
	config->control[GOOMBA_LEFT].value   = SDLK_LEFT;
	config->control[GOOMBA_RIGHT].value  = SDLK_RIGHT;
	config->control[GOOMBA_SELECT].value = SDLK_RETURN;
	config->control[GOOMBA_QUIT].value   = SDLK_ESCAPE;

	return 0;
}

