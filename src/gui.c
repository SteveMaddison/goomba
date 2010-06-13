#include <stdio.h>
#include <stdlib.h>

#include <goomba/font.h>
#include <goomba/gui.h>

typedef enum {
	GOOMBA_ALIGN_LEFT,
	GOOMBA_ALIGN_RIGHT
} goomba_align;

static SDL_Surface *screen = NULL;
static SDL_Surface *bg_original = NULL;
static SDL_Surface *bg = NULL;

static struct goomba_item *current_item = NULL;
static int max_items = 1;
static int file_selector = 0;

static struct {
	int height;
	int width;
	int margin;
	int offset;
	SDL_Surface *surface;
} bar = { 0, 0, 0, 0, NULL };

struct goomba_gui *goomba_gui_create( void ) {
	struct goomba_gui *gui = malloc( sizeof(struct goomba_gui) );

	if( gui == NULL ) {
		fprintf( stderr, "Couldn't allocate memory for goomba_gui object.\n" );
	}
	else {
		goomba_config_init( &gui->config );
		gui->root = NULL;
	}

	return gui;
}

void goomba_gui_free( struct goomba_gui *gui ) {
	goomba_item_free( gui->root );
	free( gui );
	gui = NULL;	
}

int goomba_gui_draw_bar( int y ) {
	SDL_Rect offset;
	
	offset.x = bar.offset;
	offset.y = y;
	
	SDL_BlitSurface( bar.surface, NULL, screen, &offset );
	
	return 0;
}

int goomba_gui_draw_text( char *text, int x, int y, goomba_align align ) {
	SDL_Surface *s = NULL;
	SDL_Rect offset;
	
	s = goomba_font_render( text );
	if( s == NULL ) {
		return -1;
	}

	offset.y = y;
	if( align == GOOMBA_ALIGN_LEFT ) {
		offset.x = x;
	}
	else {
		offset.x = x - s->w;
	}
	
	SDL_BlitSurface( s, NULL, screen, &offset );
	SDL_FreeSurface( s );
	
	return 0;
}

int goomba_gui_draw_item( struct goomba_item *item, int y, int stop ) {
	int sub_items = 0;
	int offset = y;

	if( item == NULL ) {
		fprintf( stderr, "Attempt to draw NULL goomba_gui object.\n" );
		return -1;
	}

	/* Offset of -1 indicate automatic placement. */
	if( y < 0 ) {
		if( item->type == GOOMBA_MENU ) {
			sub_items = goomba_item_child_count( item );
			if( item->text && *item->text ) {
				/* Hack to make space for the title. */
				sub_items ++;
			}

			if( sub_items > max_items ) {
				offset = (screen->h - (bar.height * max_items)) / 2;
			}
			else {
				offset = (screen->h - (bar.height * sub_items)) / 2;
			}
		}
		else {
			offset = (screen->h - bar.height) / 2;
		}
	}

	switch( item->type ) {
		case GOOMBA_INT:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			if( item->int_data.value ) {
				char tmp[32];
				snprintf( tmp, 32, "%d", *item->int_data.value );
				goomba_gui_draw_text( tmp, screen->w - bar.offset - bar.margin,
					offset + bar.margin, GOOMBA_ALIGN_RIGHT );
			}
			break;
		case GOOMBA_ENUM:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			if( item->enum_data.selected && item->enum_data.selected->name ) {
				goomba_gui_draw_text( item->enum_data.selected->name, screen->w - bar.offset - bar.margin,
					offset + bar.margin, GOOMBA_ALIGN_RIGHT );
			}
			break;
		case GOOMBA_STRING:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			if( item->string_data.value && item->string_data.value ) {
				goomba_gui_draw_text( item->string_data.value, screen->w - bar.offset - bar.margin,
					offset + bar.margin, GOOMBA_ALIGN_RIGHT );
			}
			break;
		case GOOMBA_FILESEL:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			goomba_gui_draw_text( item->filesel_data.value && *item->filesel_data.value ? item->filesel_data.value : "...",
					screen->w - bar.offset - bar.margin, offset + bar.margin, GOOMBA_ALIGN_RIGHT );
			break;
		case GOOMBA_FILE:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			if( item->file_data.dir ) {
				goomba_gui_draw_text( "[DIR]", screen->w - bar.offset - bar.margin, offset + bar.margin,
					GOOMBA_ALIGN_RIGHT );
			}
			break;
		case GOOMBA_ACTION:
			goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin, GOOMBA_ALIGN_LEFT );
			break;
		case GOOMBA_MENU: 
			if( stop ) {
				goomba_gui_draw_text( item->text, bar.offset + bar.margin, offset + bar.margin,
					GOOMBA_ALIGN_LEFT );
			}
			else {
				int count = 0;
				if( item->text && *item->text ) {
					/* Draw the title. */
					SDL_Rect menu_offset;
					SDL_Surface *s = goomba_font_render( item->text );
				
					if( s == NULL ) {
						fprintf( stderr, "Couldn't render menu title text \"%s\".\n", item->text );
					}
					else {
						/* Blit twice with small offset for bold effect. */
						menu_offset.y = offset + bar.margin;
					
						menu_offset.x = (screen->w - s->w) / 2;
						SDL_BlitSurface( s, NULL, screen, &menu_offset );
					
						menu_offset.x ++;
						SDL_BlitSurface( s, NULL, screen, &menu_offset );

						SDL_FreeSurface( s );
						offset += bar.height;
						count++;
					}
				}
				if( item->menu_data.items ) {
					struct goomba_item *p = item->menu_data.items;				
					int selpos = 0;
					
					if( sub_items > max_items ) {
						struct goomba_item *selected = item->menu_data.items;

						do {
							if( selected == item->menu_data.selected ) {
								break;
							}
							selpos++;
							selected = selected->next;
						} while( selected != item->menu_data.items );
		
						if( item->text ) {
							selpos += 2;
						}
						
						while( selpos > max_items ) {
							p = p->next;
							selpos--;
						}
		
						sub_items = max_items;
					}				

					do {
						if( p == item->menu_data.selected ) {
							goomba_gui_draw_bar( offset );
						}
						goomba_gui_draw_item( p, offset, 1 );
						count++;
						offset += bar.height;
						p = p->next;
					} while ( p != item->menu_data.items && count < max_items );
				}
			break;
		}
		default:
			break;
	}
}

int goomba_gui_draw( void ) {
	SDL_Rect offset;

	/* Blit the background. */
	SDL_BlitSurface( bg, NULL, screen, NULL );
	goomba_gui_draw_item( current_item, -1, 0 );
		
	SDL_Flip( screen );
}

void goomba_gui_event_flush( void ) {
	SDL_Event sdl_event;
	while( SDL_PollEvent( &sdl_event ) );
}

void goomba_gui_event_loop( struct goomba_config *config ) {
	SDL_Event sdl_event;
	goomba_control event = -1;
	int c;
	int quit = 0;
	
	while( !quit ) {
		event = -1;
		if( SDL_PollEvent( &sdl_event ) ) {
			if( sdl_event.type == SDL_QUIT ) {
				quit = 1;
			}
			else {
				for( c = 0 ; c < GOOMBA_CONTROLS ; c++ ) {
					if( sdl_event.type == SDL_KEYDOWN
					&& config->control[c].device == GOOMBA_KEYBOARD
					&& sdl_event.key.keysym.sym == config->control[c].button ) {
						event = c;
					}
				}
			}
		
			switch( event ) {
				case GOOMBA_UP:
					if( current_item->type == GOOMBA_MENU ) {
						goomba_item_retreat( current_item );
					}
					break;
				case GOOMBA_DOWN:
					if( current_item->type == GOOMBA_MENU ) {
						goomba_item_advance( current_item );
					}
					break;
				case GOOMBA_LEFT:
					if( current_item->type == GOOMBA_MENU ) {
						goomba_item_retreat( current_item->menu_data.selected );
					}
					else {
						goomba_item_retreat( current_item );
					}
					break;
				case GOOMBA_RIGHT:
					if( current_item->type == GOOMBA_MENU ) {
						goomba_item_advance( current_item->menu_data.selected );
					}
					else {
						goomba_item_advance( current_item );
					}
					break;
				case GOOMBA_SELECT:
					current_item = goomba_item_select( current_item );
					if( current_item == NULL ) {
						quit = 1;
					}
					break;
				case GOOMBA_QUIT:
					quit = 1;
					break;
				default:
					break;
			}
			
			if( event >= 0 && event < GOOMBA_QUIT ) {
				if( !quit ) {
					goomba_gui_draw();
				}
			}
		}
	}
}

int goomba_gui_start( struct goomba_gui *gui, SDL_Surface *s ) {
	SDL_Surface *tmp = NULL;
	Uint32 color = 0;

	screen = s;
	
	/* Set up our font. */
	if( goomba_font_init( &gui->config ) != 0 ) {
		fprintf( stderr, "Couldn't set up goomba font.\n" );
		return -1;
	}

	/* Save the original screen. */
	bg_original = SDL_DisplayFormat( screen );
	if( bg_original == NULL ) {
		fprintf( stderr, "Couldn't copy goomba background surface.\n" );
		return -1;
	}

	/* Create our own background, using the original screen as a starting point. */
	bg = SDL_DisplayFormatAlpha( screen );
	if( bg == NULL ) {
		fprintf( stderr, "Couldn't create goomba background surface.\n" );
		return -1;
	}

	/* Surface used to fill the background. */
	tmp = SDL_DisplayFormatAlpha( screen );
	if( tmp == NULL ) {
		fprintf( stderr, "Couldn't create goomba background fill surface.\n" );
		return -1;
	}
	/* Apply the background colour and alpha value. */
	color = SDL_MapRGBA( bg->format,
		gui->config.background.fill.red,
		gui->config.background.fill.green,
		gui->config.background.fill.blue,
		gui->config.background.alpha );
	SDL_FillRect( tmp, NULL, color );

	/* Apply fill, surface can now be discarded */
	SDL_BlitSurface( tmp, NULL, bg, NULL );
	SDL_FreeSurface( tmp );
	tmp = NULL;

	/* Calculate these now for later use. */
	bar.width = (screen->w / 8) * 6;
	bar.height = (gui->config.font.size * 100) / 80;
	bar.margin = (bar.height - gui->config.font.size) / 2;
	bar.offset = (screen->w - bar.width) / 2;
	max_items = (screen->h / bar.height) - 2;
	if( max_items < 1 ) max_items = 1;
	
	/* Surface used for the menu selection bar. */
	tmp = SDL_CreateRGBSurface( SDL_SWSURFACE|SDL_SRCALPHA, bar.width,
		bar.height, bg->format->BitsPerPixel, 0, 0, 0, 0 );
	if( tmp == NULL ) {
		fprintf( stderr, "Couldn't create goomba selector bar fill surface.\n" );
		return -1;
	}
	bar.surface = SDL_DisplayFormatAlpha( tmp );
	if( bar.surface == NULL ) {
		fprintf( stderr, "Couldn't convert goomba selector bar fill surface.\n" );
		return -1;
	}
	SDL_FreeSurface( tmp );
	tmp = NULL;

	/* Fill selection bar surface. */
	color = SDL_MapRGBA( bar.surface->format,
		gui->config.selector.fill.red,
		gui->config.selector.fill.green,
		gui->config.selector.fill.blue,
		gui->config.selector.alpha );
	SDL_FillRect( bar.surface, NULL, color );

	/* Start the GUI running. */
	current_item = gui->root;
	goomba_gui_draw();
	goomba_gui_event_flush();
	goomba_gui_event_loop( &gui->config );

	/* Restore the original screen. */
	SDL_BlitSurface( bg_original, NULL, screen, NULL );
	SDL_Flip( screen );

	/* Tidy up. */
	SDL_FreeSurface( bg );
	SDL_FreeSurface( bg_original );
	bg = NULL;
	bg_original = NULL;

	return 0;	
}

void goomba_gui_dump( struct goomba_gui *gui ) {
	if( gui && gui->root ) {
		goomba_item_dump( gui->root );
	}
}

