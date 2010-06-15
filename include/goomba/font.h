#ifndef _FONT_H_
#define _FONT_H_ 1

#include <SDL/SDL.h>
#include <goomba/config.h>

#ifdef __cplusplus
extern "C" {
#endif

int goomba_font_init( struct goomba_config *config );
void goomba_font_free( void );
SDL_Surface *goomba_font_render( const char *text );

#ifdef __cplusplus
}
#endif

#endif

