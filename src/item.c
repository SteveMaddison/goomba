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
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <goomba/item.h>
#include <goomba/gui.h>

struct goomba_item *goomba_item_create( goomba_item_t type ) {
	struct goomba_item *item = malloc( sizeof(struct goomba_item) );

	if( item == NULL ) {
		fprintf( stderr, "Couldn't allocate memory for goomba_item.\n" );
	}
	else {
		memset( item, 0, sizeof(struct goomba_item) );
		item->type = type;

		switch( type ) {
			case GOOMBA_ITEM_INT:
				item->int_data.min = 0;
				item->int_data.max = 32768;
				item->int_data.step = 1;
				break;
			case GOOMBA_ITEM_ENUM:
			case GOOMBA_ITEM_STRING:
			case GOOMBA_ITEM_CONTROL:
			case GOOMBA_ITEM_FILESEL:
			case GOOMBA_ITEM_FILE:
			case GOOMBA_ITEM_MENU:
			case GOOMBA_ITEM_ACTION:
				/* Zeroing is enough */
				break;
			default:
				fprintf( stderr, "Unknown goomba item type %d.\n", type );
				free( item );
				item = NULL;
				break;
		}	
	}
	
	return item;
}

void goomba_item_free( struct goomba_item *item ) {
	struct goomba_item *next = NULL;
	
	if( item ) {
		next = item->next;
		
		switch( item->type ) {
			case GOOMBA_ITEM_INT:
			case GOOMBA_ITEM_STRING:
			case GOOMBA_ITEM_CONTROL:
			case GOOMBA_ITEM_ACTION:
				free( item );
				item = NULL;
				break;

			case GOOMBA_ITEM_ENUM:
				if( item->enum_data.options ) {
					struct goomba_item_enum_option *p = item->enum_data.options;
					struct goomba_item_enum_option *n = NULL;
					/* Break the circular list. */
					p->prev->next = NULL;
					while( p != NULL ) {
						n = p->next;
						free( p );
						p = n;
					}
				}
				free( item );
				item = NULL;
				break;

			case GOOMBA_ITEM_FILESEL:
				if( item->filesel_data.filters ) {
					struct goomba_file_filter *p = item->filesel_data.filters;
					struct goomba_file_filter *n = NULL;
					while( p != NULL ) {
						n = p->next;
						free( p );
						p = n;
					}
				}
				free( item );
				item = NULL;
				break;

			case GOOMBA_ITEM_MENU:
				if( item->menu_data.items ) {
					struct goomba_item *p = item->menu_data.items;
					struct goomba_item *n = NULL;
					/* Break the circular list. */
					p->prev->next = NULL;
					while( p != NULL ) {
						n = p->next;
						goomba_item_free( p );
						p = n;
					}
				}
				free( item );
				break;

			default:
				break;
		}
	}
}

int goomba_item_append_child( struct goomba_item *parent, struct goomba_item *child ) {
	switch( parent->type ) {
		case GOOMBA_ITEM_MENU:
			if( parent->menu_data.items == NULL ) {
				parent->menu_data.items = child; 
				child->next = child;
				child->prev = child;
				parent->menu_data.selected = child;
			}
			else {
				struct goomba_item *head = parent->menu_data.items;
				struct goomba_item *tail = parent->menu_data.items->prev;
				child->next = head;
				child->prev = tail;
				head->prev = child;
				tail->next = child;
			}
			child->parent = parent;
			break;
		default:
			fprintf( stderr, "Can't add child to goomba item type %d.\n", parent->type );
			return -1;
			break;
	}
	
	return 0;
}

int goomba_item_add_child_sorted( struct goomba_item *parent, struct goomba_item *child ) {
	switch( parent->type ) {
		case GOOMBA_ITEM_MENU:
			if( parent->menu_data.items == NULL ) {
				parent->menu_data.items = child; 
				child->next = child;
				child->prev = child;
				parent->menu_data.selected = child;
			}
			else {
				struct goomba_item *after = parent->menu_data.items->prev;
				struct goomba_item *before = NULL;
				
				while( strcmp( after->text, child->text ) > 0 ) {
					after = after->prev;
					if( after == parent->menu_data.items->prev ) {
						break;
					}
				}
				
				before = after->next;
				child->next = before;
				child->prev = after;
				after->next = child;
				before->prev = child;
				
				if( strcmp( after->text, child->text ) > 0 ) {
					parent->menu_data.items = child;
					parent->menu_data.selected = child;
				}
			}
			child->parent = parent;
			break;
		default:
			fprintf( stderr, "Can't add child to goomba item type %d.\n", parent->type );
			return -1;
			break;
	}
	
	return 0;
}

int goomba_add_enum_option( struct goomba_item *enum_item, char *name, int value ) {
	if( enum_item->type != GOOMBA_ITEM_ENUM ) {
		fprintf( stderr, "Can't add enum value to non-enum goomba item!\n" );
		return -1;
	}
	else {
		struct goomba_item_enum_option *option = malloc( sizeof(struct goomba_item_enum_option) );

		if( option == NULL ) {
			fprintf( stderr, "Couldn't allocate memory for goomba enum option.\n" );
			return -1;
		}
		else {
			option->name = name;
			option->value = value;
			if( enum_item->enum_data.options == NULL ) {
				enum_item->enum_data.options = option;
				option->next = option;
				option->prev = option;
				enum_item->enum_data.selected = option;
				if( enum_item->enum_data.value ) {
					*enum_item->enum_data.value = option->value;
				}
			}
			else {
				struct goomba_item_enum_option *head = enum_item->enum_data.options;
				struct goomba_item_enum_option *tail = enum_item->enum_data.options->prev;
				option->next = head;
				option->prev = tail;
				head->prev = option;
				tail->next = option;
			}
		}
	}

	return 0;
}

void goomba_item_advance( struct goomba_item *item ) {
	if( item ) {
		switch( item->type ) {
			case GOOMBA_ITEM_INT:
				*item->int_data.value += item->int_data.step;
				if( *item->int_data.value > item->int_data.max ) {
					*item->int_data.value = item->int_data.max;
				}
				if( item->callback ) {
					item->callback( item );
				}
				break;
			case GOOMBA_ITEM_ENUM:
				item->enum_data.selected = item->enum_data.selected->next;
				*item->enum_data.value = item->enum_data.selected->value;
				if( item->callback ) {
					item->callback( item );
				}
				break;
			
			case GOOMBA_ITEM_MENU:
				item->menu_data.selected = item->menu_data.selected->next;
				break;

			default:
				/* No effect */
				break;
		}
	}
}

void goomba_item_retreat( struct goomba_item *item ) {
	if( item ) {
		switch( item->type ) {
			case GOOMBA_ITEM_INT:
				*item->int_data.value -= item->int_data.step;
				if( *item->int_data.value < item->int_data.min ) {
					*item->int_data.value = item->int_data.min;
				}
				if( item->callback ) {
					item->callback( item );
				}
				break;
			case GOOMBA_ITEM_ENUM:
				item->enum_data.selected = item->enum_data.selected->prev;
				*item->enum_data.value = item->enum_data.selected->value;
				if( item->callback ) {
					item->callback( item );
				}
				break;
			
			case GOOMBA_ITEM_MENU:
				item->menu_data.selected = item->menu_data.selected->prev;
				break;

			default:
				/* No effect */
				break;
		}
	}
}

int goomba_item_child_count( struct goomba_item *item ) {
	int count = 0;

	if( item ) {
		switch( item->type ) {
			case GOOMBA_ITEM_INT:
			case GOOMBA_ITEM_STRING:
			case GOOMBA_ITEM_ACTION:
				break;
				
			case GOOMBA_ITEM_ENUM:
				if( item->enum_data.options ) {
					struct goomba_item_enum_option *p = item->enum_data.options;
					do {
						count++;
						p = p->next;
					} while( p != item->enum_data.options );
				}
				break;

			case GOOMBA_ITEM_FILESEL:
				if( item->filesel_data.filters ) {
					struct goomba_file_filter *p = item->filesel_data.filters;
					while( p != NULL ) {
						count++;
						p = p->next;
					}
				}
				break;

			case GOOMBA_ITEM_MENU:
				if( item->menu_data.items ) {
					struct goomba_item *p = item->menu_data.items;
					do {
						count++;
						p = p->next;
					} while( p != item->menu_data.items );
				}
				break;

			default:
				fprintf( stderr, "Unknown goomba item type %d.\n", item->type );
				break;
		}
	}

	return count;
}

struct goomba_item *goomba_item_file_selector( char *buffer, int size, char *start, struct goomba_item *parent ) {
	struct goomba_item *menu = NULL;
	DIR *dir = NULL;
	struct dirent *dentry;
	char *dirname = start;
	struct goomba_item *item = NULL; 
	char *buf = NULL;
	
	if( dirname && *dirname ) {
		dir = opendir( dirname );
		if( !dir ) {
			fprintf( stderr, "Couldn't open \"%s\" for goomba file selector: %s.\n", dirname, strerror( errno ) );
		}
	}

	if( !dir ) {
		dirname = "/";
		dir = opendir( dirname );
		if( !dir ) {
			fprintf( stderr, "Couldn't open / for goomba file selector: %s.\n", strerror( errno ) );
			return NULL;
		}
	}

	menu = goomba_item_create( GOOMBA_ITEM_MENU );
	menu->parent = parent;
	menu->text = malloc( strlen(dirname) + 1 );
	if( menu->text == NULL ) {
		fprintf( stderr, "Couldn't allocate buffer for directory name.\n" );
		menu->text = "";
	}
	else {
		strcpy( menu->text, dirname );
	}
	
	while( (dentry = readdir( dir )) ) {
		if( strcmp( dentry->d_name, "." ) == 0 ) {
			continue;
		}

		if( parent->filesel_data.show_hidden == 0
		&& *dentry->d_name == '.' 
		&& strcmp( dentry->d_name, ".." ) != 0 ) {
			continue;
		}

		item = goomba_item_create( GOOMBA_ITEM_FILE );
		buf = malloc( strlen(dentry->d_name) + 1 );
		
		if( buf == NULL ) {
			fprintf( stderr, "Couldn't allocate buffer for file name.\n" );
			goomba_item_free( item );
		}
		else {
			item->parent = menu;
			strcpy( buf, dentry->d_name );
			item->text = buf;
			if( dentry->d_type == DT_DIR ) {
				item->file_data.dir = 1;
			}
			else if( dentry->d_type == DT_LNK ) {
				/* Is there a better way? */
				char *tmp = malloc( strlen(dirname) + strlen(dentry->d_name) + 2 );
				DIR *lnk = NULL;
				
				if( tmp == NULL ) {
					fprintf( stderr, "Couldn't allocate buffer for link name.\n" );
				}
				else {
					sprintf( tmp, "%s/%s", dirname, dentry->d_name );
					lnk = opendir( tmp );
					if( lnk && errno != ENOTDIR )
						item->file_data.dir = 1;
					closedir( lnk );
					free( tmp );
				}
			}
			goomba_item_add_child_sorted( menu, item );
		}
	}

	closedir( dir );

	return menu;
}

int goomba_item_refresh( struct goomba_item *item ) {
	if( item ) {
		switch( item->type ) {
			case GOOMBA_ITEM_ENUM:
				while( item->enum_data.selected->value != *(item->enum_data.value)
				&& item->enum_data.selected != item->enum_data.options->prev ) {
					item->enum_data.selected = item->enum_data.selected->next;
				}				
				if( item->enum_data.selected->value != *(item->enum_data.value) ) {
					item->enum_data.selected = item->enum_data.options;
					return -1;
				}
				return 0;
				break;
			case GOOMBA_ITEM_MENU:
				if( item->menu_data.items ) {
					struct goomba_item *p = item->menu_data.items;

					do {
						goomba_item_refresh( p );
						p = p->next;
					} while( p != item->menu_data.items );
				}
				break;
			default:
				break;
		}
	}
	return -1;
}

void goomba_item_dump( struct goomba_item *item ) {
	static int indent = 0;

	if( item ) {
		int i = 0;

		for( i = 0 ; i < indent ; i++ ) {
			putchar( ' ' );
		}

		printf( "%s ", item->text ? item->text : "<anon>" );
		switch( item->type ) {
			case GOOMBA_ITEM_INT:
				printf( "(int) min=%d max=%d step=%d value=", 
					item->int_data.min,
					item->int_data.max,
					item->int_data.step );
				if( item->int_data.value )
					printf( "%d\n", *item->int_data.value );
				else
					printf( "NULL\n" );
				break;

			case GOOMBA_ITEM_ENUM:
				printf( "(enum)" );
				if( item->enum_data.options ) {
					struct goomba_item_enum_option *p = item->enum_data.options;
					do {
						printf( " [%s]=%d",
							p->name ? p->name : "<NULL>",
							p->value );
						p = p->next;
					} while( p != item->enum_data.options );
				}
				if( item->enum_data.value )
					printf( " value=%d\n", *item->enum_data.value );
				else
					printf( " value=NULL\n" );
				break;

			case GOOMBA_ITEM_STRING:
				printf( "(string) size=%d value=\"%s\"\n", 
					item->string_data.size,
					item->string_data.value ? item->string_data.value : "<NULL>" );
				break;

			case GOOMBA_ITEM_CONTROL: {
				char tmp[32];
				goomba_control_string( tmp, 32, item->control_data.control );
				printf( "(control) control=\"%s\"\n", tmp );
			}
				break;

			case GOOMBA_ITEM_FILE:
				printf( "(file) text=\"%s\" dir=%d\n", 
					item->text ? item->text : "<NULL>",
					item->file_data.dir );
				break;

			case GOOMBA_ITEM_FILESEL:
				printf( "(file) value=\"%s\"",
					item->filesel_data.value ? item->filesel_data.value : "<NULL>" );
				if( item->filesel_data.filters ) {
					struct goomba_file_filter *p = item->filesel_data.filters;
					while( p != NULL ) {
						printf( " [%s](\"%s\")",
							p->pattern ? p->pattern : "<NULL>",
							p->description ? p->description : "<NULL>");
						p = p->next;
					}
				}
				printf("\n");
				break;

			case GOOMBA_ITEM_MENU:
				printf( "(menu)\n" );
				if( item->menu_data.items ) {
					struct goomba_item *p = item->menu_data.items;

					indent += 2;
					do {
						goomba_item_dump( p );
						p = p->next;
					} while( p != item->menu_data.items );
					indent -= 2;
				}
				break;

			case GOOMBA_ITEM_ACTION:
				printf( "(action) action=%d\n",
					item->action );
				break;

			default:
				fprintf( stderr, "Unknown goomba item type %d.\n", item->type );
				break;
		}
	}
}

