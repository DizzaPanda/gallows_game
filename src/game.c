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

int load_pixmap(x_window_param_t *window, pixmap_attr_t *pixmap,
		char *filedir, char *filename){
			
	char *fullpath = merge_str(filedir, filename);
	
	int rc = XReadBitmapFile(window->display, window->window,
             fullpath,
             &pixmap->bitmap_width, &pixmap->bitmap_height,
             &pixmap->bitmap,
             &pixmap->x, &pixmap->y);
	
	if(rc != BitmapSuccess){
		fprintf(stderr,"Read bitmap failed: %s\n", fullpath);
		return 1;
	}
	
	free(fullpath);
	return 0;
}

int game_res_init(x_window_param_t *window, game_res_t *res, char *path){
	memset(&res->words, 0, sizeof(string_vec_t));

#ifdef ENG_WORDS
	int err = str_vec_load_from_file(&res->words, path, "words_eng.txt");
#else
	int err = str_vec_load_from_file(&res->words, path, "words.txt");
#endif

	if(err != 0)
		goto error_handler_1;
	
	int count = 0;
	
	for( ; count < 7; count++){
		
		char image_name[11];
		sprintf(image_name, "pos_%i.xbm", count);

		err = load_pixmap(window, &res->step_to_death[count], path, image_name);\
			if(err != 0)
				goto error_handler_2;
	}
	
	return 0;
	
error_handler_2:
	do{
		count--;
		XFreePixmap(window->display, res->step_to_death[count].bitmap);
	}while(count > 0);

	str_vec_free(&res->words);
	
error_handler_1:
	fprintf(stderr,"Game resources loading failed.\n");
	return 1;
}

void game_res_free(x_window_param_t *window, game_res_t *res){
	str_vec_free(&res->words);
	
	for(int i = 0; i < 6 ; i++)
		XFreePixmap(window->display, res->step_to_death[i].bitmap);
}

int game_init(game_stat_t *game, game_res_t *game_res){
	game->words_base = &game_res->words;
	game->word_progress = NULL;
	if(game_reset(game)){
		fprintf(stderr,"Game reset failed.\n");
		return 1;
	}
	return 0;
}

void game_letter_push(game_stat_t *game, char *letter){
	int hitting = 0;
	
	for(int i = 0; i < strlen(game->current_word); ++i){
		if(		(letter[0] == game->current_word[i]) 		&&
				(letter[1] == game->current_word[i + 1])	){
			game->word_progress[i] = game->current_word[i];
			game->word_progress[i + 1] = game->current_word[i + 1];
			i++;
			hitting++;
		}
	}
	
	if(hitting == 0)
		game->step_to_death++;
	
	if(game->step_to_death == 6)
		game->status = GAME_OVER;	
}

static void game_word_progress_free(game_stat_t *game){
	if(game->word_progress != NULL)
		free(game->word_progress);
}

int game_reset(game_stat_t *game){
	game->current_word = str_vec_random(game->words_base);
	
	game_word_progress_free(game);
	int word_len = strlen(game->current_word) + 1;
	
	game->word_progress = malloc(word_len * sizeof(char));
	if(!game->word_progress){
		perror("Word_progress allocation failed");
		return 1;
	}
	
	memset(game->word_progress, '_', word_len * sizeof(char));
	game->word_progress[word_len - 1] = '\0';
	
	game->step_to_death = 0; 
	game->status = GAME_PROGRESS;
	
	return 0;
}
