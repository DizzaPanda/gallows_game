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
