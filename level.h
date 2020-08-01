#ifndef __LEVEL_H__
#define __LEVEL_H__

// цвет поля
#define FIELD_COLOR COLOR_YELLOW
// цвет фигуры
#define CELL_COLOR COLOR_BLUE
// цвет элемента фигуры куда возможен ход
#define NEXT_CELL_COLOR COLOR_RED
// цвет 2 элемента фигуры куда возможен ход
#define NEXT_CELL_SECOND_COLOR COLOR_PURPLE

// цвет текущего элемента фигуры
#define CURRENT_CELL_COLOR COLOR_WHITE

struct size_
{
    unsigned char width;
    unsigned char height;
};

struct point_
{
    int x;
    int y;
};

struct level_t
{
    char* name;
    struct size_ size;
    unsigned char score2star;
    unsigned char score1star;
};

void init_current_level();
void draw_level();
int get_level_count();
struct size_* get_level_size(int level);
char* get_level_name(int level);
void draw_thumbnail_level(int level, int x, int y, int scale);
void level_make_action(int action);
void level_click_action();
int level_exist_next_pos();
int get_level_cell_count();
struct level_t *get_current_level_info();
struct point_ get_cell_position(int level, int x_cell, int y_cell);

#endif