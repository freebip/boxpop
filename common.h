#ifndef __COMMON_H__
#define __COMMON_H__

#include "libbip.h"

// ������������ ������ �������� ����
#define FIELD_MAX_SIZE 10*10

typedef unsigned short word;
typedef unsigned char byte;

struct game_state_t
{
    // ���������� ������ ������
    byte data[FIELD_MAX_SIZE];
    // ������� ��������� ������ �� ������
    int x, y;
    // ������� ��������� ���������� ����
    int x_next, y_next;
    // ����� ����� �������
    int moves;
};

struct appdata_t
{
    Elf_proc_* proc;
    void* ret_f;
    // ������� �����
    byte screen;
    // ����� ������� ������
    byte n_current_level;
    // ��������� ����
    struct game_state_t game_state;
};

extern struct appdata_t** game_appdata_p;
extern struct appdata_t* game_appdata;

extern int ticks;

extern unsigned short randint(short max);

#endif