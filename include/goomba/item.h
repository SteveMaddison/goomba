#ifndef _GOOMBA_ITEM_H_
#define _GOOMBA_ITEM_H_ 1

#include <goomba/control.h>

typedef enum {
	GOOMBA_ITEM_INT,
	GOOMBA_ITEM_ENUM,
	GOOMBA_ITEM_STRING,
	GOOMBA_ITEM_CONTROL,
	GOOMBA_ITEM_FILESEL,
	GOOMBA_ITEM_FILE,
	GOOMBA_ITEM_MENU,
	GOOMBA_ITEM_ACTION,
	GOOMBA_ITEMS
} goomba_item_t;

typedef enum {
	GOOMBA_ACTION_BACK,
	GOOMBA_ACTION_EXIT,
    GOOMBA_ACTION_CUSTOM,
    GOOMBA_ACTIONS
} goomba_action_t;

struct goomba_item_int {
	int *value;
	int min;
	int max;
	int step;
};

struct goomba_item_enum_option {
	struct goomba_item_enum_option *next;
	struct goomba_item_enum_option *prev;
	int value;
	char *name;
};

struct goomba_item_enum {
	int *value;
	struct goomba_item_enum_option *options;
	struct goomba_item_enum_option *selected;
};

struct goomba_item_string {
	char *value;
	int size;
};

struct goomba_item_control {
	struct goomba_control *control;
};

struct goomba_file_filter {
	struct goomba_file_filter *next;
	char *pattern;
	char *description;
};

struct goomba_item_file {
	int dir;
};

struct goomba_item_file_selector {
	char *value;
	int size;
	char *directory;
	struct goomba_file_filter *filters;
};

struct goomba_item_menu {
	struct goomba_item *items;
	struct goomba_item *selected;
};


struct goomba_item {
	struct goomba_item *next;
	struct goomba_item *prev;
	struct goomba_item *parent;
	goomba_item_t type;
	char *text;
	goomba_action_t action;
	int (*callback)(struct goomba_item *item);
	union {
		struct goomba_item_int int_data;
		struct goomba_item_enum enum_data;
		struct goomba_item_string string_data;
		struct goomba_item_control control_data;
		struct goomba_item_file_selector filesel_data;
		struct goomba_item_file file_data;
		struct goomba_item_menu menu_data;
	};
};

#ifdef __cplusplus
extern "C" {
#endif

struct goomba_item *goomba_item_create( goomba_item_t type );
void goomba_item_free( struct goomba_item *item );
int goomba_item_append_child( struct goomba_item *parent, struct goomba_item *child );
int goomba_item_add_child_sorted( struct goomba_item *parent, struct goomba_item *child );
int goomba_item_child_count( struct goomba_item *item );
int goomba_add_enum_option( struct goomba_item *enum_item, char *name, int value );

void goomba_item_advance( struct goomba_item *item );
void goomba_item_retreat( struct goomba_item *item );
struct goomba_item *goomba_item_file_selector( char *buffer, int size, char *start, struct goomba_item *parent );

void goomba_item_dump( struct goomba_item *item );

#ifdef __cplusplus
}
#endif

#endif

