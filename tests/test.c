#include "./../src/game.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void logic_test(char *current_word, char letter, char *expected_result);	//проверяет логику функции game_letter_push_eng

int main(){
	logic_test("widow", 'w', "w___w");
	logic_test("monkey", 'n', "__n___");
	logic_test("angel", 'l', "____l");
	logic_test("library", 'r', "___r_r_");
	logic_test("snake", 'o', "_____");

	return 0;
}

void logic_test(char *current_word, char letter, char *expected_result){
	game_stat_t game;							//сщстояние игры

	game.current_word = current_word;					//устанавливаем текущее слово
	int size = strlen(current_word) + 1;					//размер слова
	game.word_progress = malloc(size * sizeof(char));			//выделяем память для game.word_progress
	memset(game.word_progress, '_', size);
	game.word_progress[size - 1] = '\0';
	game.step_to_death = 0;
	game.status = GAME_PROGRESS;

	game_letter_push_eng(&game, letter);					//запускаем тестирование
	assert(!strcmp(game.word_progress, expected_result));			//проверяет соответствие с ожидаемым результатом
	printf("%s Test is done!\n", current_word);

	free(game.word_progress);
}
