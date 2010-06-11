#include <string.h>
#include <goomba/config.h>

#define GOOMBA_DEFAULT_FONT_SIZE 12

int goomba_config_init( struct goomba_config *config ) {
	memset( config, 0, sizeof(struct goomba_config) );

	config->background.alpha = 128;

	config->font.size = GOOMBA_DEFAULT_FONT_SIZE;
	config->font.color.red   = 255;
	config->font.color.green = 255;
	config->font.color.blue  = 255;

	config->selector.fill.red = 128;
	config->selector.alpha = 128;

	return 0;
}

