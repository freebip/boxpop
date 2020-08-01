// boxpop by x27

#include "boxpop.h"
#include "font.h"
#include "level.h"
#include "common.h"

#define MAX_THUMBNAIL_WIDTH 30

struct appdata_t** game_appdata_p;
struct appdata_t* game_appdata;

struct regmenu_ menu_screen = { 55, 1, 0, dispatch_screen, keypress_screen, screen_job, 0, show_screen, 0, 0 };

struct saved_status_t saved_status;
int ticks;
int show_count_level;

int main(int p, char** a)
{
    show_screen((void*)p);
}

void show_screen(void* p)
{
    game_appdata_p = (struct appdata_t**) get_ptr_temp_buf_2();

    if ((p == *game_appdata_p) && get_var_menu_overlay()) {
        game_appdata = *game_appdata_p;
        *game_appdata_p = (struct appdata_t *)NULL;
        reg_menu(&menu_screen, 0);
        *game_appdata_p = game_appdata;
    }
    else {
        reg_menu(&menu_screen, 0);
        *game_appdata_p = (struct appdata_t*)pvPortMalloc(sizeof(struct appdata_t));
        game_appdata = *game_appdata_p;
        _memclr(game_appdata, sizeof(struct appdata_t));
        game_appdata->proc = (Elf_proc_*)p;
    }

    if (p && game_appdata->proc->elf_finish)
        game_appdata->ret_f = game_appdata->proc->elf_finish;
    else
        game_appdata->ret_f = show_watchface;

    ElfReadSettings(game_appdata->proc->index_listed, &saved_status, 0, sizeof(struct saved_status_t));
    if (saved_status.n_last_game_figure >= get_level_count())
    {
        _memset(&saved_status, sizeof(struct saved_status_t), 0xff);
        saved_status.n_last_game_figure = 0;
        saved_status.level_stars[0] = 0;
    }

    game_appdata->n_current_level = saved_status.n_last_game_figure;

    ticks = 0;
    game_appdata->screen = TITLE_SCREEN;
    
    draw_screen();
    set_update_period(1, 100);
}

void screen_job()
{
    ticks++;
    draw_screen();
    repaint_screen_lines(0, 176);
    set_update_period(1, 100);
}


int dispatch_screen(void* p)
{
    game_appdata_p = (struct appdata_t **) get_ptr_temp_buf_2();
    game_appdata = *game_appdata_p;

    struct gesture_* gest = (struct gesture_*)p;

    switch (game_appdata->screen)
    {
        case TITLE_SCREEN:
            // пропускаем титульный экран по любому действию
            game_appdata->screen = SELECT_FIGURE;
            break;
        case SELECT_FIGURE:
            if (gest->gesture == GESTURE_SWIPE_DOWN)
            {
                if (game_appdata->n_current_level > 0)
                    game_appdata->n_current_level--;
            }
            else if (gest->gesture == GESTURE_SWIPE_UP)
            {
                // учитываем, что во время выбора фигуры у нас на экране их 2 штуки
                if (game_appdata->n_current_level < get_level_count() - 2)
                    game_appdata->n_current_level++;
            }
            else if (gest->gesture == GESTURE_CLICK)
            {
                int n_sel_figure = game_appdata->n_current_level;
                if (gest->touch_pos_y > VIDEO_Y / 2)
                    n_sel_figure++;

                if (saved_status.level_stars[n_sel_figure] != 0xff)
                {
                    game_appdata->n_current_level = n_sel_figure;
                }
                else
                    // кликнули по еще не доступной фигуре
                    // можно повибрировать даже
                    break;


                init_current_level();
                game_appdata->screen = PLAY_GAME;
            }
            break;
        case GAME_RESULTS:
            game_appdata->screen = SELECT_FIGURE;
            break;
        case PLAY_GAME:
        {
            switch (gest->gesture) 
            {
                case GESTURE_CLICK: 
                {
                    level_click_action();
                    ticks = 0;
                    show_count_level = game_appdata->n_current_level;

                    if (!level_exist_next_pos())
                    {

                        struct level_t* level_info = get_current_level_info();

                        if (game_appdata->game_state.moves >= get_level_cell_count())
                            saved_status.level_stars[game_appdata->n_current_level] = 3;
                        else if (game_appdata->game_state.moves >= level_info->score2star)
                            saved_status.level_stars[game_appdata->n_current_level] = 2;
                        else if (game_appdata->game_state.moves >= level_info->score1star)
                            saved_status.level_stars[game_appdata->n_current_level] = 1;
                        else
                            saved_status.level_stars[game_appdata->n_current_level] = 0;

                        if (game_appdata->n_current_level + 1 < get_level_count())
                        {
                            // есть следующий уровень
                            if (saved_status.level_stars[game_appdata->n_current_level + 1] == 0xff)
                            {
                                // он залочен
                                if (saved_status.level_stars[game_appdata->n_current_level])
                                {
                                    // и у текущего уровня набрано больше 0 звезд, то разлачиваем
                                    saved_status.level_stars[game_appdata->n_current_level + 1] = 0;
                                    if (game_appdata->n_current_level + 2 < get_level_count())
                                        game_appdata->n_current_level++;

                                }
                            }
                        }
                        else
                            game_appdata->n_current_level--;

                        saved_status.n_last_game_figure = game_appdata->n_current_level;

                        ElfWriteSettings(game_appdata->proc->index_listed, &saved_status, 0, sizeof(struct saved_status_t));
                        game_appdata->screen = GAME_RESULTS;
                    }
                    break;
                };
                case GESTURE_SWIPE_RIGHT: 
                {
                    level_make_action(gest->gesture);
                    break;
                };
                case GESTURE_SWIPE_LEFT: 
                {
                    level_make_action(gest->gesture);
                    break;
                };
                case GESTURE_SWIPE_UP:
                {
                    level_make_action(gest->gesture);
                    break;
                };
                case GESTURE_SWIPE_DOWN: 
                {
                    level_make_action(gest->gesture);
                    break;
                };
                default: 
                {
                    break;
                };
            }
        }
    }
    return 0;
}

void keypress_screen()
{
    game_appdata_p = (struct appdata_t **) get_ptr_temp_buf_2();
    game_appdata = *game_appdata_p;
    show_menu_animate(game_appdata->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void draw_title_screen()
{
    char* t_freebip = "FREEBIP";
    char* t_popbox = "BOXPOP";

    set_bg_color(COLOR_YELLOW);
    fill_screen_bg();
    set_fg_color(COLOR_BLACK);

    int n = randint(13);

    draw_filled_rect(3, 63, 171, 86);
    draw_filled_rect(15, 87, 159, 110);
    for (int i = 0; i < 7; i++)
    {
        set_bg_color(n == i ? COLOR_RED : COLOR_WHITE);
        draw_filled_rect_bg(4 + 24 * i, 64, 26 + 24 * i, 85);
        draw_symbol(t_freebip[i], 8 + 24 * i, 68, 3);
    }
    set_fg_color(COLOR_BLUE);
    for (int i = 0; i < 6; i++)
    {
        set_bg_color(n-7 == i ? COLOR_RED : COLOR_WHITE);
        draw_filled_rect_bg(16 + 24 * i, 87, 38 + 24 * i, 109);
        draw_symbol(t_popbox[i], 20 + 24 * i, 92, 3);
    }
}

void draw_star(int x, int y)
{
    draw_vertical_line(x, y, y + 7);
    draw_filled_rect(x - 1, y + 2, x + 1, y + 3);
    draw_horizontal_line(y + 4, x - 5, x + 5);
    draw_horizontal_line(y + 5, x - 4, x + 4);
    draw_horizontal_line(y + 6, x - 2, x + 2);
    draw_filled_rect(x - 3, y + 7, x, y + 8);
    draw_filled_rect(x + 1, y + 7, x + 3, y + 8);
    draw_horizontal_line(y + 9, x - 4, x - 3);
    draw_horizontal_line(y + 9, x + 3, x + 4);
}

void draw_lock(int x, int y)
{
    draw_horizontal_line(y, x - 2, x + 3);
    draw_filled_rect(x - 3, y + 1, x + 4, y + 10);
    draw_vertical_line(x - 4, y + 4, y + 9);
    draw_vertical_line(x + 5, y + 4, y + 9);
    draw_filled_rect_bg(x - 1, y + 2, x + 2, y + 3);
    draw_filled_rect_bg(x, y + 7, x + 1, y + 7);
}

void draw_thumbnail(int level, int y)
{
    set_fg_color(COLOR_BLUE);

    struct size_* size = get_level_size(level);
    int scale = MAX_THUMBNAIL_WIDTH / size->width;
    int offset_y = VIDEO_Y / 4 - scale * size->height / 2;

    draw_thumbnail_level(level, 15, y+offset_y, scale);

    char* level_name = get_level_name(level);
    int scale_text = 2;
    draw_text(level_name, 5+VIDEO_X / 3, y + VIDEO_Y / 4 - get_multitext_height(level_name) * scale_text / 2, scale_text);

    if (saved_status.level_stars[level] == 0xff)
        draw_lock(155, y+70);
    else
    {
        set_fg_color(COLOR_BLUE);
        for (int i = 0; i < 3; i++)
        {
            if (saved_status.level_stars[level] <= i)
                set_fg_color(COLOR_WHITE);
            draw_star(155 - 16 * i, y+70);
        }
    }
}

void draw_level_selecting()
{
    set_bg_color(COLOR_YELLOW);
    fill_screen_bg();
    set_fg_color(COLOR_BLUE);
    draw_horizontal_line(VIDEO_Y / 2, 0, VIDEO_X-5);
    draw_vertical_line(VIDEO_X - 5, 0, VIDEO_Y);
    set_fg_color(COLOR_WHITE);
    draw_filled_rect(VIDEO_X - 4, 0, VIDEO_X, VIDEO_Y);
    set_fg_color(COLOR_BLUE);

    draw_thumbnail(game_appdata->n_current_level, 0);
    draw_thumbnail(game_appdata->n_current_level+1, VIDEO_Y/2);

    // scrollbar
    set_fg_color(COLOR_BLUE);
    int size = VIDEO_Y / (get_level_count() - 1) + 1 ;
    int position = game_appdata->n_current_level  * VIDEO_Y / (get_level_count() - 1);
    draw_filled_rect(VIDEO_X - 4, position, VIDEO_X, position+size);
}

void draw_move_count(int level)
{
    set_fg_color(COLOR_RED);
    struct point_ p = get_cell_position(level, game_appdata->game_state.x, game_appdata->game_state.y);
    int scale = 3 * (5 - ticks);
    char buf[10];
    _sprintf(buf, "%d", game_appdata->game_state.moves);
    int offset = game_appdata->game_state.moves > 9 ? 5 * scale : 5 * scale / 2;
    draw_text(buf, p.x - offset, p.y - offset, scale);
}

void draw_game_results()
{
    set_bg_color(COLOR_YELLOW);
    fill_screen_bg();

    int x = 190 - 6 * ticks;

    set_fg_color(COLOR_RED);
    draw_text(get_level_name(show_count_level), x, 10, 3);
    
    char buf[50];
    _sprintf(buf, "MOVES:%d", game_appdata->game_state.moves);
    set_fg_color(COLOR_BLUE);
    x = 4 * ticks - 90;
    draw_text(buf, x, 80, 3);
    
    int y = 4 * ticks;
    _sprintf(buf, "STARS:%d", saved_status.level_stars[show_count_level]);
    set_fg_color(COLOR_GREEN);
    draw_text(buf, 10, y, 3);
}


void draw_screen()
{
    set_graph_callback_to_ram_1();
    load_font();

    switch (game_appdata->screen)
    {
        case TITLE_SCREEN:
            draw_title_screen();
            if (ticks == 20)
                game_appdata->screen = SELECT_FIGURE;

            break;
        case SELECT_FIGURE:
            draw_level_selecting();
            break;
        case PLAY_GAME:
            draw_level();

            if (ticks < 5 && game_appdata->game_state.moves > 1)
            {
                draw_move_count(show_count_level);
            }

            break;
        case GAME_RESULTS:
            if (ticks < 5)
            {
                set_bg_color(COLOR_YELLOW);
                fill_screen_bg();
                draw_move_count(show_count_level);
            }
            else
                draw_game_results();
            
            if (ticks == 30)
                game_appdata->screen = SELECT_FIGURE;
            break;
    }

    set_display_state_value(8, 1);
    set_display_state_value(4, 1);
    set_display_state_value(2, 0);
}
