/*
 * Goomba GUI/menu system
 * Copyright (c) 2010 Steve Maddison
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <goomba/control.h>
#include <goomba/font.h>
#include <goomba/gui.h>
#include <goomba/key.h>

#define HELP_LEN 128

static const int AXIS_THRESHOLD = 16000;
static const int BALL_THRESHOLD = 10;
static const int MOUSE_THRESHOLD = 10;

static SDL_Surface *screen = NULL;
static SDL_Surface *bg_original = NULL;
static SDL_Surface *bg = NULL;

static struct goomba_item *current_item = NULL;
static int max_items = 1;

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

int goomba_gui_hat_dir_value( int direction ) {
	switch( direction ) {
		case SDL_HAT_UP:
			return GOOMBA_DIR_UP;
			break;
		case SDL_HAT_DOWN:
			return GOOMBA_DIR_DOWN;
			break;
		case SDL_HAT_LEFT:
			return GOOMBA_DIR_LEFT;
			break;
		case SDL_HAT_RIGHT:
			return GOOMBA_DIR_RIGHT;
			break;
		default:
			break;
	}
	return 0;
}

int goomba_gui_draw_bar( int y ) {
	SDL_Rect offset;
	
	offset.x = bar.offset;
	offset.y = y;
	
	SDL_BlitSurface( bar.surface, NULL, screen, &offset );
	
	return 0;
}

int goomba_gui_draw_text( char *text_left, char *text_right, int y ) {
	SDL_Surface *sl = NULL;
	SDL_Surface *sr = NULL;
	SDL_Surface *dots = NULL;
	SDL_Rect offset;
	SDL_Rect chop;
	int end;
	
	sl = goomba_font_render( text_left, GOOMBA_FONT_LARGE );
	sr = goomba_font_render( text_right, GOOMBA_FONT_LARGE );

	chop.x = 0;
	chop.y = 0;
	offset.y = y;
	
	if( sl ) {
		int max_width = screen->w - (bar.offset * 2) - (bar.margin * 2);
		max_width = (max_width * 100) / 120;

		chop.w = sl->w > max_width ? max_width : sl->w;
		chop.h = sl->h;

		offset.x = bar.offset + bar.margin;
		SDL_BlitSurface( sl, &chop, screen, &offset );
		SDL_FreeSurface( sl );	
	}
	if( sr ) {
		/* Check if we need to truncate the value text. */
		if( sl ) {
			end = bar.offset + chop.w + (bar.margin * 3);
		} else {
			end = bar.offset + bar.margin;
		}

		chop.w = sr->w;
		chop.h = sr->h;

		offset.x = screen->w - bar.offset - bar.margin - sr->w;			
		if( offset.x < end ) {
			dots = goomba_font_render( "...", GOOMBA_FONT_LARGE );
			if( dots ) {
				chop.x = (end - offset.x);
				offset.x += (end - offset.x);

				SDL_BlitSurface( dots, NULL, screen, &offset );

				chop.x += dots->w;
				chop.w -= chop.x;
				
				offset.x += dots->w;
			}
		}

		SDL_BlitSurface( sr, &chop, screen, &offset );
		SDL_FreeSurface( sr );
		if( dots ) {
			SDL_FreeSurface( dots );
		}
	}

	return 0;
}

int goomba_gui_draw_item( struct goomba_item *item, int y, int stop ) {
	int sub_items = 0;
	int offset = y;
	char tmp[32] = "";

	if( item == NULL ) {
		fprintf( stderr, "Attempt to draw NULL goomba_gui object.\n" );
		return -1;
	}

	/* Offset of -1 indicate automatic placement. */
	if( y < 0 ) {
		if( item->type == GOOMBA_ITEM_MENU ) {
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
		case GOOMBA_ITEM_INT:
			if( item->int_data.value ) {
				snprintf( tmp, 32, "%d", *item->int_data.value );
			}
			goomba_gui_draw_text( item->text, tmp, offset + bar.margin );
			break;
		case GOOMBA_ITEM_ENUM:
			goomba_gui_draw_text( item->text,
				(item->enum_data.selected && item->enum_data.selected->name) ? item->enum_data.selected->name : "",
				offset + bar.margin );
			break;
		case GOOMBA_ITEM_STRING:
			goomba_gui_draw_text( item->text, item->string_data.value, offset + bar.margin );
			break;
		case GOOMBA_ITEM_CONTROL:
			goomba_control_string( tmp, 32, item->control_data.control );
			goomba_gui_draw_text( item->text, tmp, offset + bar.margin );
			break;
		case GOOMBA_ITEM_FILESEL:
			goomba_gui_draw_text( item->text,
				(item->filesel_data.value && *item->filesel_data.value) ? item->filesel_data.value : "...",
				offset + bar.margin );
			break;
		case GOOMBA_ITEM_FILE:
			goomba_gui_draw_text( item->text, item->file_data.dir ? "[DIR]" : NULL, offset + bar.margin );
			break;
		case GOOMBA_ITEM_ACTION:
			goomba_gui_draw_text( item->text, NULL, offset + bar.margin );
			break;
		case GOOMBA_ITEM_MENU: 
			if( stop ) {
				goomba_gui_draw_text( item->text, NULL, offset + bar.margin );
			}
			else {
				int count = 0;
				if( item->text && *item->text ) {
					/* Draw the title. */
					SDL_Rect menu_offset;
					SDL_Surface *s = goomba_font_render( item->text, GOOMBA_FONT_LARGE );
				
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
	return 0;
}

int goomba_gui_draw( void ) {
	/* Blit the background. */
	SDL_BlitSurface( bg, NULL, screen, NULL );
	goomba_gui_draw_item( current_item, -1, 0 );
		
	SDL_Flip( screen );
	return 0;
}

void goomba_gui_event_flush( void ) {
	SDL_Event sdl_event;
	while( SDL_PollEvent( &sdl_event ) );
}

int goomba_gui_capture_control( struct goomba_control *control ) {
	int timeout = 5000; /* Five seconds */
	SDL_Event sdl_event;

	goomba_gui_event_flush();

	while( timeout > 0 ) {
		SDL_PollEvent( &sdl_event );
		switch( sdl_event.type ) {
			case SDL_KEYDOWN:
				control->device_type = GOOMBA_DEV_KEYBOARD;
				control->device_id = sdl_event.key.which;
				control->value = sdl_event.key.keysym.sym;
				return 0;
			case SDL_JOYAXISMOTION:
				control->device_type = GOOMBA_DEV_JOYSTICK;
				control->device_id = sdl_event.jaxis.which;
				control->control_type = GOOMBA_CTRL_AXIS;
				control->control_id = sdl_event.jaxis.axis;
				if( sdl_event.jaxis.value > AXIS_THRESHOLD )
					control->value = 1;
				else if( sdl_event.jaxis.value < -AXIS_THRESHOLD )
					control->value = -1;
				else
					return -1;
				return 0;
			case SDL_JOYBUTTONDOWN:
				control->device_type = GOOMBA_DEV_JOYSTICK;
				control->device_id = sdl_event.jbutton.which;
				control->control_type = GOOMBA_CTRL_BUTTON;
				control->value = sdl_event.jbutton.button;
				break;
			case SDL_JOYHATMOTION:
				control->device_type = GOOMBA_DEV_JOYSTICK;
				control->device_id = sdl_event.jhat.which;
				control->control_type = GOOMBA_CTRL_HAT;
				control->control_id = sdl_event.jhat.hat;
				control->value = goomba_gui_hat_dir_value( sdl_event.jhat.value );
				return 0;
			case SDL_JOYBALLMOTION:
				control->device_type = GOOMBA_DEV_JOYSTICK;
				control->device_id = sdl_event.jball.which;
				control->control_type = GOOMBA_CTRL_BALL;
				control->control_id = sdl_event.jball.ball;
				if( sdl_event.jball.xrel > BALL_THRESHOLD )
					control->value = GOOMBA_DIR_LEFT;
				else if(  sdl_event.jball.xrel < -BALL_THRESHOLD )
					control->value = GOOMBA_DIR_RIGHT;
				else if( sdl_event.jball.yrel > BALL_THRESHOLD )
					control->value = GOOMBA_DIR_DOWN;
				else if(  sdl_event.jball.yrel < -BALL_THRESHOLD )
					control->value = GOOMBA_DIR_UP;
				else
					return -1;
				return 0;
			case SDL_MOUSEBUTTONDOWN:
				control->device_type = GOOMBA_DEV_MOUSE;
				control->device_id = sdl_event.button.which;
				control->control_type = GOOMBA_CTRL_BUTTON;
				control->value = sdl_event.button.button;
				return 0;
			case SDL_MOUSEMOTION:
				control->device_type = GOOMBA_DEV_MOUSE;
				control->device_id = sdl_event.motion.which;
				control->control_type = GOOMBA_CTRL_AXIS;
				if( sdl_event.motion.xrel > MOUSE_THRESHOLD )
					control->value = GOOMBA_DIR_LEFT;
				else if(  sdl_event.motion.xrel < -MOUSE_THRESHOLD )
					control->value = GOOMBA_DIR_RIGHT;
				else if( sdl_event.motion.yrel > MOUSE_THRESHOLD )
					control->value = GOOMBA_DIR_DOWN;
				else if(  sdl_event.motion.yrel < -MOUSE_THRESHOLD )
					control->value = GOOMBA_DIR_UP;
				else
					return -1;
				return 0;
			default:
				break;
		}
		SDL_Delay( 10 );
		timeout -= 10;
	}
	return -1;
}

void goomba_gui_event_loop( struct goomba_config *config ) {
	SDL_Event sdl_event;
	goomba_event_t event = -1;
	int i;
	int quit = 0;
	
	SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL * 2 );
	
	while( !quit ) {
		event = -1;
		if( SDL_PollEvent( &sdl_event ) ) {
			if( sdl_event.type == SDL_QUIT ) {
				quit = 1;
			}
			else {
				for( i = 0 ; i < GOOMBA_EVENTS ; i++ ) {
					if( sdl_event.type == SDL_KEYDOWN
					&& config->control[i].device_type == GOOMBA_DEV_KEYBOARD
					&& sdl_event.key.keysym.sym == config->control[i].value ) {
						event = i;
					}
				}
			}
		
			switch( event ) {
				case GOOMBA_EVENT_UP:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						goomba_item_retreat( current_item );
					}
					break;
				case GOOMBA_EVENT_DOWN:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						goomba_item_advance( current_item );
					}
					break;
				case GOOMBA_EVENT_LEFT:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						goomba_item_retreat( current_item->menu_data.selected );
					}
					else {
						goomba_item_retreat( current_item );
					}
					break;
				case GOOMBA_EVENT_RIGHT:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						goomba_item_advance( current_item->menu_data.selected );
					}
					else {
						goomba_item_advance( current_item );
					}
					break;
				case GOOMBA_EVENT_SKIP_F:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						int skip = max_items;
						
						if( current_item->text ) {
							skip--;
						}
						
						while( skip && current_item->menu_data.selected != current_item->menu_data.items->prev ) {
							goomba_item_advance( current_item );
							skip--;
						}
					}
					break;
				case GOOMBA_EVENT_SKIP_B:
					if( current_item->type == GOOMBA_ITEM_MENU ) {
						int skip = max_items;
						
						if( current_item->text ) {
							skip--;
						}
						
						while( skip && current_item->menu_data.selected != current_item->menu_data.items ) {
							goomba_item_retreat( current_item );
							skip--;
						}
					}
					break;
				case GOOMBA_EVENT_SELECT: {
						struct goomba_item *selected = current_item;
						
						if( current_item->type == GOOMBA_ITEM_MENU ) {
							selected = current_item->menu_data.selected;
						}
							
						switch( selected->type ) {
							case GOOMBA_ITEM_MENU:
								/* Nested menu */
								current_item = selected;
								/* Select first item. */
								current_item->menu_data.selected = current_item->menu_data.items;
								break;
								
							case GOOMBA_ITEM_FILESEL: {
									char *dir = selected->filesel_data.directory;
									if( selected->filesel_data.value && *selected->filesel_data.value ) {
										char *slash = strrchr( selected->filesel_data.value, '/' );
										if( slash ) {
											*slash = 0;
										}
										dir = selected->filesel_data.value;
									}
									current_item = goomba_item_file_selector(
										selected->filesel_data.value,
										selected->filesel_data.size,
										dir,
										selected );
								}
								break;
	
							case GOOMBA_ITEM_FILE: {
									struct goomba_item *menu = selected->parent;
									struct goomba_item *p = menu->menu_data.items;
									
									if( selected->file_data.dir ) {
										/* Directory was selected, build a new selector for the contents. */
										char *dir = NULL;
										
										if( strcmp( selected->text, ".." ) == 0 ) {
											char *slash = strrchr( menu->text, '/' );
											if( slash ) {
												*slash = 0;
												dir = malloc( strlen(menu->text) + 1 );
												strcpy( dir, menu->text );
											}
										}
										else {
											if( strcmp( menu->text, "/" ) == 0 ) {
												dir = malloc( strlen(selected->text) + 2 );
												sprintf( dir, "/%s", selected->text );
											}
											else {
												dir = malloc( strlen(menu->text) + strlen(selected->text) + 2 );
												sprintf( dir, "%s/%s", menu->text, selected->text );
											}
										}
								
										current_item = goomba_item_file_selector(
											menu->parent->filesel_data.value,
											menu->parent->filesel_data.size,
											dir,
											menu->parent );
										
										free( dir );
									}
									else {
										/* File was selected. */
										if( strcmp( menu->text, "/" ) == 0 ) {
											snprintf( menu->parent->filesel_data.value, menu->parent->filesel_data.size,
												"/%s", selected->text );
										}
										else {
											snprintf( menu->parent->filesel_data.value, menu->parent->filesel_data.size,
												"%s/%s", menu->text, selected->text );					
										}
										current_item = menu->parent->parent;
										
										/* File selector callback and action. */
										if( menu->parent->callback ) {
											menu->parent->callback( menu->parent );
										}
										if( menu->parent->action == GOOMBA_ACTION_EXIT ) {
											quit = 1;
										}
									}
									
									/* Free the old menu, and all associated buffers. */
									do {
										free( p->text );
										p = p->next;
									} while( p != menu->menu_data.items );
									free( menu->text );
									goomba_item_free( menu );
								}
								break;
	
							case GOOMBA_ITEM_ACTION:
								if( selected->action == GOOMBA_ACTION_BACK ) {
									current_item = selected->parent;
									if( current_item->type == GOOMBA_ITEM_MENU ) {
										current_item = current_item->parent;
									}
								}
								else if( selected->action == GOOMBA_ACTION_EXIT ) {
									quit = 1;
								}
								if( selected->callback ) {
									selected->callback( selected );
								}
								break;
	
							case GOOMBA_ITEM_CONTROL: {
									int original = selected->control_data.control->device_type;
						
									/* Redraw item/menu with empty value. */
									selected->control_data.control->device_type = GOOMBA_DEV_UNKNOWN;
									goomba_gui_draw();
									
									if( goomba_gui_capture_control( selected->control_data.control ) != 0 ) {
										/* Error or timeout: restore original control. */
										selected->control_data.control->device_type = original;
									}				

									if( selected->callback ) {
										selected->callback( selected );
									}
								}
								break;
	
							default:
								break;
						}
					}
					break;

				case GOOMBA_EVENT_BACK:
					current_item = current_item->parent;
					while( current_item && current_item->type != GOOMBA_ITEM_MENU ) {
						current_item = current_item->parent;
					}
					break;

				case GOOMBA_EVENT_QUIT:
					quit = 1;
					break;
				default:
					break;
			}

			if( current_item == NULL ) {
				quit = 1;
			}

			if( event >= 0 && event < GOOMBA_EVENT_QUIT ) {
				if( !quit ) {
					goomba_gui_draw();
				}
			}
		}
	}
}

int goomba_gui_start( struct goomba_gui *gui, SDL_Surface *s ) {
	SDL_Surface *tmp = NULL;
	SDL_Rect offset;
	Uint32 color = 0;
	char help[HELP_LEN] = "";
	char help_ctrl[4][HELP_LEN/4];

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
	max_items = (screen->h / bar.height) - 3;
	if( max_items < 1 ) max_items = 1;
	
	if( gui->config.help ) {
		/* Build the help text. */
		goomba_control_string( help_ctrl[0], HELP_LEN/4, &gui->config.control[GOOMBA_EVENT_SELECT] );
		goomba_control_string( help_ctrl[1], HELP_LEN/4, &gui->config.control[GOOMBA_EVENT_BACK] );
		goomba_control_string( help_ctrl[2], HELP_LEN/4, &gui->config.control[GOOMBA_EVENT_SKIP_B] );
		goomba_control_string( help_ctrl[3], HELP_LEN/4, &gui->config.control[GOOMBA_EVENT_SKIP_F] );
		
		snprintf( help, HELP_LEN, "%s = Select/OK    %s = Back/Cancel    %s/%s = Skip",
			help_ctrl[0], help_ctrl[1], help_ctrl[2], help_ctrl[3] );
	
		/* Blit text to the bottom of the background. */	
		tmp = goomba_font_render( help, GOOMBA_FONT_SMALL );
		offset.x = (screen->w - tmp->w) / 2;
		offset.y = screen->h - tmp->h - (tmp->h/2);
		SDL_BlitSurface( tmp, NULL, bg, &offset );
		SDL_FreeSurface( tmp );
		tmp = NULL;
	}
	
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

