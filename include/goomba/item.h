#ifndef _GOOMBA_ITEM_H_
#define _GOOMBA_ITEM_H_ 1

typedef enum {
	GOOMBA_INT,
	GOOMBA_ENUM,
	GOOMBA_STRING,
	GOOMBA_FILESEL,
	GOOMBA_FILE,
	GOOMBA_MENU,
	GOOMBA_ACTION
} goomba_item_type;

typedef enum {
	GOOMBA_BACK
} goomba_action_type;

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

struct goomba_item_action {
	goomba_action_type action;
};

struct goomba_item {
	struct goomba_item *next;
	struct goomba_item *prev;
	struct goomba_item *parent;
	goomba_item_type type;
	char *text;
	int (*callback)(void);
	union {
		struct goomba_item_int int_data;
		struct goomba_item_enum enum_data;
		struct goomba_item_string string_data;
		struct goomba_item_file_selector filesel_data;
		struct goomba_item_file file_data;
		struct goomba_item_menu menu_data;
		struct goomba_item_action action_data;
	};
};

struct goomba_item *goomba_item_create( goomba_item_type type );
void goomba_item_free( struct goomba_item *item );
int goomba_item_append_child( struct goomba_item *parent, struct goomba_item *child );
int goomba_item_add_child_sorted( struct goomba_item *parent, struct goomba_item *child );
int goomba_item_child_count( struct goomba_item *item );

void goomba_item_advance( struct goomba_item *item );
void goomba_item_retreat( struct goomba_item *item );
struct goomba_item *goomba_item_select( struct goomba_item *item );
struct goomba_item *goomba_item_file_selector( char *buffer, int size, char *start, struct goomba_item *parent );

void goomba_item_dump( struct goomba_item *item );

#endif

