#include <stdio.h>
#include <SDL/SDL_ttf.h>

#include <goomba/config.h>
#include <goomba/font.h>

#define DEFAULT_FONT_SIZE 24

static TTF_Font *font = NULL;
static SDL_Color col = { 255, 255, 255 };

int goomba_font_init( struct goomba_config *config ) {
	if( config->font.size < 1 ) {
		config->font.size = DEFAULT_FONT_SIZE;
	}

	col.r = config->font.color.red;
	col.g = config->font.color.green;
	col.b = config->font.color.blue;

	if( TTF_Init() != 0 ) {
		fprintf( stderr, "Couldn't initialise font library: %s\n", TTF_GetError() );
		return -1;
	}
		
	if( config->font.name && *config->font.name ) {
		font = TTF_OpenFont( config->font.name, config->font.size );
		if( font == NULL ) {
			fprintf( stderr, "Couldn't load font '%s': %s\n", config->font.name, TTF_GetError() );
			return -1;
		}
	}
	else {
		fprintf( stderr, "No font specified in goomba config.\n" );
		return -1;
	}

	return 0;
}

void goomba_font_free( void ) {
	if( font )
		TTF_CloseFont( font );
		
	TTF_Quit();
}

SDL_Surface *goomba_font_render( const char *text ) {
	if( text && *text )
		return TTF_RenderText_Blended( font, text, col );
	else 
		return NULL;
}

