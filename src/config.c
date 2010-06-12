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

	for( i = 0; i < GOOMBA_CONTROLS ; i++ ) {
		config->control[i].device = GOOMBA_KEYBOARD;
	}
	config->control[GOOMBA_UP].button     = SDLK_UP;
	config->control[GOOMBA_DOWN].button   = SDLK_DOWN;
	config->control[GOOMBA_LEFT].button   = SDLK_LEFT;
	config->control[GOOMBA_RIGHT].button  = SDLK_RIGHT;
	config->control[GOOMBA_SELECT].button = SDLK_RETURN;
	config->control[GOOMBA_QUIT].button   = SDLK_ESCAPE;

	return 0;
}

