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
