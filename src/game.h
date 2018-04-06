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
pixmap_attr_t;						

int load_pixmap(x_window_param_t *window, pixmap_attr_t *pixmap, 
		char *filedir, char *filename);

typedef struct{
	string_vec_t words;
	pixmap_attr_t step_to_death[7];	
}
game_res_t;

int 	game_res_init	(x_window_param_t *window, game_res_t *res, char *path);	
void 	game_res_free	(x_window_param_t *window, game_res_t *res);			
typedef enum{			
	GAME_PROGRESS,
	GAME_OVER
}
game_status_t;

typedef struct{
	string_vec_t	*words_base;			
	char			*current_word;
	char			*word_progress;	
	int				step_to_death;
	game_status_t	status;
}
game_stat_t;

int 	game_init				(game_stat_t *game, game_res_t *game_res);
void 	game_letter_push		(game_stat_t *game, char *letter);	
void 	game_letter_push_eng	(game_stat_t *game, char letter);	
int 	game_reset				(game_stat_t *game);	
void 	game_free				(game_stat_t *game);	
char 	*game_return_progress	(game_stat_t *game);
char 	*game_return_progress_eng	(game_stat_t *game);		
int		game_win_check			(game_stat_t *game);
int		game_lose_check			(game_stat_t *game);

int return_letter_by_keycode(unsigned int keycode, char* output);
int return_letter_by_keycode_eng(unsigned int keycode, char* output);

void game_draw(x_window_param_t *win, game_res_t *res, game_stat_t *game);
int pre_game_settings(x_window_param_t *win);
void game_loop(x_window_param_t *win, game_res_t *res, game_stat_t *game);

#endif
