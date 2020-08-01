#ifndef __COMMON_H__
#define __COMMON_H__

#include "libbip.h"

// максимальный размер игрового поля
#define FIELD_MAX_SIZE 10*10

typedef unsigned short word;
typedef unsigned char byte;

struct game_state_t
{
    // изменяемые данные фигуры
    byte data[FIELD_MAX_SIZE];
    // текущее положение игрока на фигуре
    int x, y;
    // текущее положение выьранного хода
    int x_next, y_next;
    // всего ходов сделано
    int moves;
};

struct appdata_t
{
    Elf_proc_* proc;
    void* ret_f;
    // текущий экран
    byte screen;
    // номер текущей фигуры
    byte n_current_level;
    // состояние игры
    struct game_state_t game_state;
};

extern struct appdata_t** game_appdata_p;
extern struct appdata_t* game_appdata;

extern int ticks;

extern unsigned short randint(short max);

#endif