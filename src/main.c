#include "game.h"
#include <stdio.h>

int main(){							
	x_window_param_t window_param;
	
	game_res_t res;
	game_stat_t game;
	
	int err;
	
	err = window_init(&window_param, "Gallows game", 100, 100, 400, 250);
	if(err != 0){
		fprintf(stderr, "window_init failed.\n");
		return 1;
	}
err = game_res_init(&window_param, &res, "./res/");
	if(err != 0){
		fprintf(stderr, "game_res_init failed.\n");
		return 1;
	}
	
	err = game_init(&game, &res);
	if(err != 0){
		fprintf(stderr, "game_init failed.\n");
	}
err = pre_game_settings(&window_param);	
	if(err != 0)
		return 1;
		
	game_loop(&window_param, &res, &game);
	
	game_free(&game);
	game_res_free(&window_param, &res);
	window_free(&window_param);
}
