#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <goomba/item.h>

struct goomba_item *goomba_item_create( goomba_item_type type ) {
	struct goomba_item *item = malloc( sizeof(struct goomba_item) );

	if( item == NULL ) {
		fprintf( stderr, "Couldn't allocate memory for goomba_item.\n" );
	}
	else {
		memset( item, 0, sizeof(struct goomba_item) );
		item->type = type;

		switch( type ) {
			case GOOMBA_INT:
				item->int_data.min = 0;
				item->int_data.max = 32768;
				item->int_data.step = 1;
				break;
			case GOOMBA_ENUM:
			case GOOMBA_STRING:
			case GOOMBA_FILE:
			case GOOMBA_MENU:
			case GOOMBA_ACTION:
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
			case GOOMBA_INT:
			case GOOMBA_STRING:
			case GOOMBA_ACTION:
				free( item );
				item = NULL;
				break;

			case GOOMBA_ENUM:
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

			case GOOMBA_FILE:
				if( item->file_data.filters ) {
					struct goomba_file_filter *p = item->file_data.filters;
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

			case GOOMBA_MENU:
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
				fprintf( stderr, "Unknown goomba item type %d.\n", item->type );
				break;
		}
	}
}

int goomba_append_child( struct goomba_item *parent, struct goomba_item *child ) {
	switch( parent->type ) {
		case GOOMBA_MENU:
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

int goomba_add_enum_option( struct goomba_item *enum_item, char *name, int value ) {
	if( enum_item->type != GOOMBA_ENUM ) {
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
			case GOOMBA_INT:
				*item->int_data.value += item->int_data.step;
				if( *item->int_data.value > item->int_data.max ) {
					*item->int_data.value = item->int_data.max;
				}
				break;
			case GOOMBA_ENUM:
				item->enum_data.selected = item->enum_data.selected->next;
				*item->enum_data.value = item->enum_data.selected->value;
				break;
			
			case GOOMBA_MENU:
				item->menu_data.selected = item->menu_data.selected->next;
				break;

			default:
				/* No effect */
				break;
		}
		if( item->callback && *item->callback ) {
			item->callback();
		}
	}
}

void goomba_item_retreat( struct goomba_item *item ) {
	if( item ) {
		switch( item->type ) {
			case GOOMBA_INT:
				*item->int_data.value -= item->int_data.step;
				if( *item->int_data.value < item->int_data.min ) {
					*item->int_data.value = item->int_data.min;
				}
				break;
			case GOOMBA_ENUM:
				item->enum_data.selected = item->enum_data.selected->prev;
				*item->enum_data.value = item->enum_data.selected->value;
				break;
			
			case GOOMBA_MENU:
				item->menu_data.selected = item->menu_data.selected->prev;
				break;

			default:
				/* No effect */
				break;
		}
		if( item->callback && *item->callback ) {
			item->callback();
		}
	}
}

struct goomba_item *goomba_item_select( struct goomba_item *item ) {
	struct goomba_item *new_item = item;

	if( item ) {
		switch( item->type ) {
			case GOOMBA_STRING:
				break;
			case GOOMBA_FILE:
				break;
			case GOOMBA_MENU:
				/* Unless the result of selecting the menu item results
				 * in quitting, we stay in the menu. */
				if( goomba_item_select( item->menu_data.selected ) == NULL ) {
					return NULL;
				}
				break;
			case GOOMBA_ACTION:
				if( item->callback && *item->callback ) {
					item->callback();
				}
				if( item->action_data.action == GOOMBA_BACK ) {
					new_item = item->parent;
					if( new_item->type == GOOMBA_MENU ) {
						new_item = new_item->parent;
					}
				}
				break;
			default:
				break;
		}	
	}
	return new_item;
}

int goomba_item_child_count( struct goomba_item *item ) {
	int count = 0;

	if( item ) {
		switch( item->type ) {
			case GOOMBA_INT:
			case GOOMBA_STRING:
			case GOOMBA_ACTION:
				break;
				
			case GOOMBA_ENUM:
				if( item->enum_data.options ) {
					struct goomba_item_enum_option *p = item->enum_data.options;
					do {
						count++;
						p = p->next;
					} while( p != item->enum_data.options );
				}
				break;

			case GOOMBA_FILE:
				if( item->file_data.filters ) {
					struct goomba_file_filter *p = item->file_data.filters;
					while( p != NULL ) {
						count++;
						p = p->next;
					}
				}
				break;

			case GOOMBA_MENU:
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

void goomba_item_dump( struct goomba_item *item ) {
	static int indent = 0;

	if( item ) {
		int i = 0;

		for( i = 0 ; i < indent ; i++ ) {
			putchar( ' ' );
		}

		printf( "%s ", item->text ? item->text : "<anon>" );
		switch( item->type ) {
			case GOOMBA_INT:
				printf( "(int) min=%d max=%d step=%d value=", 
					item->int_data.min,
					item->int_data.max,
					item->int_data.step );
				if( item->int_data.value )
					printf( "%d\n", *item->int_data.value );
				else
					printf( "NULL\n" );
				break;

			case GOOMBA_ENUM:
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

			case GOOMBA_STRING:
				printf( "(string) size=%d value=\"%s\"\n", 
					item->string_data.size,
					item->string_data.value ? item->string_data.value : "<NULL>" );
				break;

			case GOOMBA_FILE:
				printf( "(file) value=\"%s\"",
					item->file_data.value ? item->file_data.value : "<NULL>" );
				if( item->file_data.filters ) {
					struct goomba_file_filter *p = item->file_data.filters;
					while( p != NULL ) {
						printf( " [%s](\"%s\")",
							p->pattern ? p->pattern : "<NULL>",
							p->description ? p->description : "<NULL>");
						p = p->next;
					}
				}
				break;

			case GOOMBA_MENU:
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

			case GOOMBA_ACTION:
				printf( "(action) action=%d\n",
					item->action_data.action );
				break;

			default:
				fprintf( stderr, "Unknown goomba item type %d.\n", item->type );
				break;
		}
	}
}

