#ifndef GAME_H
#define GAME_H

#include <X11/Xlib.h>

typedef struct{				
	Display 	*display;
	Window 		window;
	XEvent 		event;
	int 		screen_number;
	GC 			gc;
}
x_window_param_t;

int 	window_init	(x_window_param_t *param, char *title, 
					int pos_x, int pos_y, int Width, int Height);
	
void 	window_free	(x_window_param_t *param);

typedef struct{	
	char **content;	
	unsigned capacity;
	unsigned size;
}
string_vec_t;

int 	str_vec_init	(string_vec_t *vec, unsigned capacity);		
int 	str_vec_push	(string_vec_t *vec, char *value);		
void 	str_vec_free	(string_vec_t *vec);				
char *	str_vec_random	(string_vec_t *vec);				
int		str_vec_load_from_file (string_vec_t *vec, char *filedir, char *filename);

typedef struct{		
	Pixmap bitmap;							
	unsigned int bitmap_width, bitmap_height;			
	int x, y;							
}
