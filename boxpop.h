#ifndef __BOXPOP_H__
#define __BOXPOP_H__

#include "common.h"

// сохранялка игры
struct saved_status_t
{
    // последняя фигура, которая игралась
    byte n_last_game_figure;
    // набранные звёзды по фигурам
    byte level_stars[50];
};

enum
{
    TITLE_SCREEN = 0,
    SELECT_FIGURE = 1,
    PLAY_GAME = 2,
    GAME_RESULTS = 3,
};


// прототипы

void show_screen(void* return_screen);
void keypress_screen();
int dispatch_screen(void* p);
void screen_job();
void draw_screen();

#endif