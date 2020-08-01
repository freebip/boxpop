#include "libbip.h"
#include "level.h"
#include "common.h"

// расстояние между элементами фигуры
#define CELL_SPACING 1
// минимальное расстояние от фигуры до границ экрана
#define BORDER_DISTANCE 10

#define LEVEL_COUNT (sizeof(levels)/sizeof(struct level_t))

// 0:no_cell 1:cell 2:cell_with_start_pos
unsigned char levels_data[] =
{
    2,1,2,1,0,1,1,0,1,
    2,2,2,2,0,2,2,2,2,
    0,1,0,0,2,0,2,1,2,1,0,1,2,0,2,
    2,2,2,2,0,2,2,1,2,2,1,2,2,0,2,2,2,2,
    0,0,2,0,0,2,1,2,2,1,0,1,0,2,1,0,
    0,2,0,0,0,2,0,2,2,2,2,2,2,0,2,0,0,0,2,0,
    0,2,2,0,2,2,2,2,2,2,2,2,0,2,2,0,
    0,2,1,2,2,1,1,1,1,1,1,1,2,1,2,1,
    0,0,2,2,0,2,2,0,2,2,2,2,0,2,2,0,2,2,0,0,
    0,0,0,2,2,0,2,2,0,2,0,2,2,2,2,2,2,2,2,0,2,0,2,2,0,2,2,0,0,0,
    0,0,2,2,2,2,2,2,0,2,2,0,2,2,2,2,2,2,0,0,
    2,1,2,2,1,1,2,2,1,1,2,1,2,1,2,1,2,2,1,1,2,1,2,1,2,
    1,2,1,2,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,2,1,2,1,
    0,1,0,1,0,1,2,0,2,1,1,1,0,1,1,0,1,1,1,0,1,1,0,1,1,
    0,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,0, 0,0,2,2,0,0,
    0,0,2,2,2,0,0,2,2,2,2,2,0,2,2,2,2,0,2,2,2,2,2,0,0,2,2,2,0,0,
    0,0,2,0,2,0,2,2,2,0,2,0,0,0,2,2,2,2,2,2,2,2,0,0,0,2,0,2,2,2,0,2,0,2,0,0,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    0,0,2,2,0,0,0,2,2,2,2,0,0,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,0,0,2,2,2,2,0,0,0,2,2,0,0,
    0,0,0,0,2,2,0,0,0,0,2,0,2,2,0,0,2,2,2,2,0,0,2,2,2,2,0,0,2,2,0,2,0,0,0,0,2,2,0,0,0,0
};

struct level_t levels[] =
{
    { "MAGNET", {3, 3}, 5, 6 },
    { "DONUT", {3, 3}, 6, 7 },
    { "TUNING FORK", {3, 5}, 6, 7 },
    { "VHS", {3, 6}, 9, 12 },
    { "CRAB", {4, 4}, 6, 7 },
    { "CANES", {4, 5}, 6, 8 },
    { "CROSS", {4, 4}, 7, 9 },
    { "CUT SQUARE", {4, 4}, 10, 12 },
    { "HELIX", {4, 5}, 7, 10 },
    { "KNOT", {5, 6}, 10, 14 },
    { "INFINITY", {5, 4}, 9, 11},
    { "5X5", {5, 5}, 15, 22 },
    { "RHOMBOID", {5, 5}, 11, 17 },
    { "SHEARS", {5, 5}, 9, 13 },
    { "PEACOCK", {6, 6}, 18, 23 },
    { "ROCK", {6, 5}, 13, 17 },
    { "SHURIKEN", {6, 6}, 14, 17 },
    { "6X6", {6, 6}, 24, 32 },
    { "PORTAL", {6, 9}, 26, 32 },
    { "LOOP", {7, 6}, 11, 14 }
};

enum
{
    UP = 2,
    DOWN = 3,
    LEFT = 4,
    RIGHT = 5
};

int get_level_count()
{
    return LEVEL_COUNT;
}

struct size_* get_level_size(int level)
{
    return &(&levels[level])->size;
}

char* get_level_name(int level)
{
    return (&levels[level])->name;
}

int get_offset_level_data(int level)
{
    int offset = 0;
    for (int i = 0; i <= level; i++)
    {
        if (i == level)
            return offset;

        struct size_* size = &(&levels[i])->size;
        offset += size->width * size->height;
    }
    return -1;
}

void draw_thumbnail_level(int level, int x, int y, int scale)
{
    if (level >= LEVEL_COUNT)
        return;

    int offset_data = get_offset_level_data(level);
    if (offset_data < 0)
        return;

    if (offset_data >= sizeof(levels_data))
        return;

    struct size_ *size = &(&levels[level])->size;
    int index = 0;
    for (int iy = 0; iy < size->height; iy++)
        for (int ix = 0; ix < size->width; ix++)
        {
            if (levels_data[offset_data + index++])
                draw_filled_rect(x + ix * scale, y + iy * scale, x + (ix + 1) * scale-1, y + (iy + 1) * scale-1);
        }
}

int is_next_position(int x, int y)
{
    int xc = game_appdata->game_state.x;
    int yc = game_appdata->game_state.y;

    if (xc + 1 == x || xc - 1 == x)
        if (yc + 2 == y || yc - 2 == y)
            return 1;
    if (xc + 2 == x || xc - 2 == x)
        if (yc + 1 == y || yc - 1 == y)
            return 1;

    return 0;
}

unsigned long randseed;

unsigned short randint(short max)
{
    randseed = randseed * get_tick_count();
    randseed++;
    return ((randseed >> 16) * max) >> 16;
}

struct level_t* get_current_level_info()
{
    return &levels[game_appdata->n_current_level];
}

int get_level_cell_count()
{
    int data_offset = get_offset_level_data(game_appdata->n_current_level);
    struct size_ s_level = levels[game_appdata->n_current_level].size;
    int flat_size = s_level.width * s_level.height;
    int count=0;
    for (int i = 0; i < flat_size; i++)
        if (levels_data[data_offset + i])
            count++;
    return count;
}

void init_current_level()
{
    int data_offset = get_offset_level_data(game_appdata->n_current_level);
    struct size_ s_level = levels[game_appdata->n_current_level].size;
    int flat_size = s_level.width * s_level.height;
    _memcpy(game_appdata->game_state.data, &levels_data[data_offset], flat_size);
    
    // случайно выбираем первоначальную позицию из доступных
    int n = 0;
    for (int i = 0; i < flat_size; i++)
        if (game_appdata->game_state.data[i] == 2)
            n++;
    int pos = randint(n);
    n = 0;
    for (int i = 0; i < flat_size; i++)
    {
        if (game_appdata->game_state.data[i] == 2)
        {
            if (n++ == pos)
            {
                game_appdata->game_state.x = i % s_level.width;
                game_appdata->game_state.y = i / s_level.width;
                game_appdata->game_state.data[i] = 0;
                break;
            }
        }
    }

    game_appdata->game_state.moves = 1;
    game_appdata->game_state.x_next = -1;
    game_appdata->game_state.y_next = -1;
    ticks = 0;
}

struct point_ get_cell_position(int level, int x_cell, int y_cell)
{
    struct size_ s_level = levels[level].size;

    // размер элемента фигуры
    int size_x = (VIDEO_X - 2 * BORDER_DISTANCE - (s_level.width - 1) * CELL_SPACING) / s_level.width;
    int size_y = (VIDEO_Y - 2 * BORDER_DISTANCE - (s_level.height - 1) * CELL_SPACING) / s_level.height;
    int size = min(size_x, size_y);
    // начальное положение фигуры на экране
    int start_x = (VIDEO_X - size * s_level.width - (s_level.width - 1) * CELL_SPACING) / 2;
    int start_y = (VIDEO_Y - size * s_level.height - (s_level.height - 1) * CELL_SPACING) / 2;
    
    struct point_ p;
    p.x = start_x + x_cell * (size + CELL_SPACING) + size / 2;
    p.y = start_y + y_cell * (size + CELL_SPACING) + size / 2;
    return p;
}

void draw_level()
{
    set_bg_color(FIELD_COLOR);
    fill_screen_bg();
    set_bg_color(CELL_COLOR);

    struct size_ s_level = levels[game_appdata->n_current_level].size;

    // размер элемента фигуры
    int size_x = (VIDEO_X - 2 * BORDER_DISTANCE - (s_level.width - 1) * CELL_SPACING) / s_level.width;
    int size_y = (VIDEO_Y - 2 * BORDER_DISTANCE - (s_level.height - 1) * CELL_SPACING) / s_level.height;
    int size = min(size_x, size_y);
    // начальное положение фигуры на экране
    int start_x = (VIDEO_X - size * s_level.width - (s_level.width - 1) * CELL_SPACING) / 2;
    int start_y = (VIDEO_Y - size * s_level.height - (s_level.height - 1) * CELL_SPACING) / 2;
    
    int index = 0;
    for (int j = 0; j < s_level.height; j++)
    {
        for (int i = 0; i < s_level.width; i++)
        {
            set_fg_color(FIELD_COLOR);

            if (j == game_appdata->game_state.y && i == game_appdata->game_state.x)
                set_fg_color(CURRENT_CELL_COLOR);

            else if (game_appdata->game_state.data[index])
            {
                if (is_next_position(i, j))
                {
                    if (game_appdata->game_state.x_next == -1)
                    {
                        game_appdata->game_state.x_next = i;
                        game_appdata->game_state.y_next = j;
                    }
    
                    if (game_appdata->game_state.x_next == i && game_appdata->game_state.y_next == j)
                    {
                        set_fg_color(ticks % 2 == 1 ? NEXT_CELL_COLOR : NEXT_CELL_SECOND_COLOR);
                    }
                    else
                        set_fg_color(NEXT_CELL_COLOR);
                }
                else
                    set_fg_color(CELL_COLOR);
            }

            draw_filled_rect(
                start_x + i * (size + CELL_SPACING),
                start_y + j * (size + CELL_SPACING),
                start_x + i * (size + CELL_SPACING) + size-1,
                start_y + j * (size + CELL_SPACING) + size-1
            );

            index++;
        }
    }
}

int level_exist_next_pos()
{
    struct size_ s_level = levels[game_appdata->n_current_level].size;
    int index = 0;
    for (int j = 0; j < s_level.height; j++)
        for (int i = 0; i < s_level.width; i++)
            if (game_appdata->game_state.data[index++])
                if (is_next_position(i, j))
                    return 1;
    return 0;
}


void level_click_action()
{
    struct size_ s_level = levels[game_appdata->n_current_level].size;

    game_appdata->game_state.data[game_appdata->game_state.x_next + s_level.width * game_appdata->game_state.y_next] = 0;
    game_appdata->game_state.moves++;
    game_appdata->game_state.x = game_appdata->game_state.x_next;
    game_appdata->game_state.y = game_appdata->game_state.y_next;
    game_appdata->game_state.x_next = -1;
    game_appdata->game_state.y_next = -1;
}

void level_make_action(int action)
{
    struct size_ s_level = levels[game_appdata->n_current_level].size;
    int index = 0;
    int x_sel = -1, y_sel = -1, distance = 999;
    for (int j = 0; j < s_level.height; j++)
    {
        for (int i = 0; i < s_level.width; i++)
        {
            if (game_appdata->game_state.data[index++] == 0)
                continue;

            if (is_next_position(i, j) &&
                (i != game_appdata->game_state.x_next ||
                j != game_appdata->game_state.y_next))
            {
                int d = (game_appdata->game_state.x_next - i) * (game_appdata->game_state.x_next - i) +
                    (game_appdata->game_state.y_next - j) * (game_appdata->game_state.y_next - j);

                if (distance > d)
                {
                    if (((action == UP) && (j < game_appdata->game_state.y_next)) ||
                        ((action == DOWN) && (j > game_appdata->game_state.y_next)) ||
                        ((action == LEFT) && (i < game_appdata->game_state.x_next)) ||
                        ((action == RIGHT) && (i> game_appdata->game_state.x_next)))
                    {
                        y_sel = j;
                        x_sel = i;
                        distance = d;
                    }
                }
            }
        }
    }

    if (distance != 999)
    {
        game_appdata->game_state.x_next = x_sel;
        game_appdata->game_state.y_next = y_sel;
    }
}
