#include "game.h"
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define ENG_WORDS

#define ESC_KEYCODE 0x09

static void set_title(char *title_ptr, Display *display, Window window){
	XTextProperty windowname;							
	XStringListToTextProperty (&title_ptr, 1, &windowname);

	XSetWMProperties ( display, window, &windowname,
  		NULL, NULL, NULL, NULL, NULL,
  		NULL );
}

int window_init(x_window_param_t *param, char *title,				
		int pos_x, int pos_y, int Width, int Height){
			
	param->display = XOpenDisplay(NULL);
	if(param->display == NULL){
		perror("Open display failed");
        return 1;
	}
	
	param->screen_number = DefaultScreen(param->display);
	
	param->window =  XCreateSimpleWindow(
			param->display, RootWindow(param->display, param->screen_number), sudo
            		BlackPixel(param->display, param->screen_number),
			WhitePixel(param->display, param->screen_number));
	
	set_title(title, param->display, param->window);
	
	XSelectInput(param->display, param->window, KeyPressMask | KeyReleaseMask );
	XMapWindow(param->display, param->window);
	
	param->gc = XCreateGC ( param->display, param->window, 0 , NULL );
	
	return 0;
}


