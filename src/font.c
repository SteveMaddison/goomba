#include <stdio.h>
#include <SDL/SDL_ttf.h>

#include <goomba/config.h>
#include <goomba/font.h>

#define DEFAULT_FONT_SIZE 24

static TTF_Font *font_large = NULL;
static TTF_Font *font_small = NULL;
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
		font_large = TTF_OpenFont( config->font.name, config->font.size );
		if( font_large == NULL ) {
			fprintf( stderr, "Couldn't load font '%s' (%d): %s\n", config->font.name, config->font.size, TTF_GetError() );
			return -1;
		}
		font_small = TTF_OpenFont( config->font.name, (config->font.size * 60) / 80 );
		if( font_small == NULL ) {
			fprintf( stderr, "Couldn't load font '%s' (%d): %s\n", config->font.name, (config->font.size * 60) / 100, TTF_GetError() );
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
	if( font_large )
		TTF_CloseFont( font_large );

	if( font_small )
		TTF_CloseFont( font_small );
		
	TTF_Quit();
}

SDL_Surface *goomba_font_render( const char *text, goomba_font_size_t size ) {
	if( text && *text ) {
		if( size == GOOMBA_FONT_LARGE ) {
			return TTF_RenderText_Blended( font_large, text, col );
		}
		else {
			return TTF_RenderText_Blended( font_small, text, col );
		}
	}
	
	return NULL;
}

