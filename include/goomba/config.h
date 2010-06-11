#ifndef _GOOMBA_CONFIG_H_
#define _GOOMBA_CONFIG_H_ 1

struct goomba_config_rgb {
	int red;
	int green;
	int blue;
};

struct goomba_config_background {
	struct goomba_config_rgb fill;
	char *image;
	int alpha; /* percentage */
};

struct goomba_config_font {
	char *font;
	int size;
	struct goomba_config_rgb color;
};

struct goomba_config_selector {
	struct goomba_config_rgb fill;
	int alpha; /* percentage */
};

struct goomba_config {
	struct goomba_config_background background;	
	struct goomba_config_font font;
	struct goomba_config_selector selector;
};

int goomba_config_init( struct goomba_config *config );

#endif

