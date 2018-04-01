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

