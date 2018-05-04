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
int str_vec_push(string_vec_t *vec, char *value){
	if(value[0] == '\0')
		return 0;
	
	if(vec->size >= vec->capacity){	
		
		unsigned new_capacity = vec->capacity * 2;
		char ** new_content = realloc(vec->content, new_capacity * sizeof(char **));
		if(!new_content){
			perror("Vector push reallocation failed");
			return 1;
		}
		
		vec->capacity = new_capacity;
		vec->content = new_content;
	} 
	
	int value_len = strlen(value) + 1;
	vec->content[vec->size] = malloc(value_len * sizeof(char));
	if(!vec->content[vec->size]){
		perror("Vector push allocation failed");
		return 1;
	}
	
	strncpy(vec->content[vec->size], value, value_len);
	++vec->size;

	return 0;
}

void str_vec_free(string_vec_t *vec){
	if(vec->content == NULL)
		return;
	
	for(int n = 0; n < vec->size; ++n){
		 free(vec->content[n]);
	}
	free(vec->content);
}

static char *merge_str(char *first, char *second){
	int size = strlen(first);
	size += strlen(second);	
	++size;
	
	char *result = malloc(size * sizeof(char));
	strncpy(result, first, strlen(first));
	strncpy(result + strlen(first), second, strlen(second));
	result[size] = '\0';
	return result;
}

int	str_vec_load_from_file(string_vec_t *vec, char *filedir, char *filename){
	str_vec_free(vec);
	str_vec_init(vec, 10);
	
	char *fullpath = merge_str(filedir, filename);
	
	FILE* words_file = fopen(fullpath, "r");
	if(!words_file) {
		perror("Open words file failed");
        return 1;
    }
	
	free(fullpath);	
	
	char current_word[100];
	int err = 0;
	
	while(!feof(words_file)){
		fscanf(words_file, "%s", current_word);
		
		err = str_vec_push(vec, current_word);
		if(err != 0){
			fprintf(stderr,"Vector push failed.\n");
			return 1;
		}
		
		current_word[0] = '\0';
	}
	
	return 0;
}


