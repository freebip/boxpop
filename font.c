#include "libbip.h"
#include "font.h"

#define FONT_SYMBOL_SIZE 7
#define FONT_SYMBOL_COUNT (sizeof(font)/FONT_SYMBOL_SIZE)

unsigned char font[] =
{
    0x41, 0x00, // A
    0b11111,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    
    0x42, 0x00, // B
    0b11110,
    0b10001,
    0b11111,
    0b10001,
    0b11110,
    
    0x43, 0x00, // C
    0b11111, 
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    
    0x44, 0x00, // D
    0b11110,
    0b10001,
    0b10001,
    0b10001,
    0b11110,
    
    0x45, 0x00, // E
    0b11111,
    0b10000,
    0b11110,
    0b10000,
    0b11111,
     
    0x46, 0x00, // F
    0b11111,
    0b10000,
    0b11110,
    0b10000,
    0b10000,
    
    0x47, 0x00, // G
    0b11111,
    0b10000,
    0b10111,
    0b10001,
    0b11111,
    
    0x48, 0x00, // H
    0b10001,
    0b10001,
    0b11111,
    0b10001,
    0b10001,
    
    0x49, 0x00, // I
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b11111,
    
    0x4a, 0x00, // J
    0b11111,
    0b00010,
    0b00010,
    0b10010,
    0b11110,
    
    0x4b, 0x00, // K
    0b10001,
    0b10010,
    0b11100,
    0b10010,
    0b10001,
    
    0x4c, 0x00, // L
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    
    0x4d, 0x00, // M
    0b10001,
    0b11011,
    0b10101,
    0b10001,
    0b10001,
    
    0x4e, 0x00, // N
    0b10001,
    0b11001,
    0b10101,
    0b10011,
    0b10001,
    
    0x4f, 0x00, // O
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    
    0x50, 0x00, // P
    0b11111,
    0b10001,
    0b11111,
    0b10000,
    0b10000,
    
    0x51, 0x00, // Q
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    
    0x52, 0x00, // R
    0b11111,
    0b10001,
    0b11111,
    0b10010,
    0b10001,
    
    0x53, 0x00, // S
    0b11111,
    0b10000,
    0b11111,
    0b00001,
    0b11111,
    
    0x54, 0x00, // T
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    
    0x55, 0x00, // U
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    
    0x56, 0x00, // V
    0b10001,
    0b10001,
    0b01010,
    0b01010,
    0b00100,
    
    0x57, 0x00, // W
    0b10001,
    0b10001,
    0b10001,
    0b10101,
    0b11011,
    
    0x58, 0x00, // X
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    
    0x59, 0x00, // Y
    0b10001,
    0b01010,
    0b00100,
    0b00100,
    0b00100,
    
    0x5a, 0x00, // Z
    0b11111,
    0b00010,
    0b00100,
    0b01000,
    0b11111,

    0x20, 0x00, // space
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,

    0x30, 0x00, // 0
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,

    0x31, 0x00, // 1
    0b01100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,

    0x32, 0x00, // 2
    0b01110,
    0b10001,
    0b00110,
    0b01000,
    0b11111,

    0x33, 0x00, // 3
    0b11111,
    0b00001,
    0b00111,
    0b00001,
    0b11111,

    0x34, 0x00, // 4
    0b10001,
    0b10001,
    0b11111,
    0b00001,
    0b00001,

    0x35, 0x00, // 5
    0b01111,
    0b10000,
    0b11111,
    0b00001,
    0b11111,

    0x36, 0x00, // 6
    0b11111,
    0b10000,
    0b11111,
    0b10001,
    0b11111,

    0x37, 0x00, // 7
    0b11111,
    0b00001,
    0b00010,
    0b00100,
    0b01000,

    0x38, 0x00, // 8
    0b11111,
    0b10001,
    0b01110,
    0b10001,
    0b11111,

    0x39, 0x00, // 9
    0b11111,
    0b10001,
    0b11111,
    0b00001,
    0b11111
};

void draw_text(char* text, int x, int y, int scale)
{
    int len = _strlen(text);
    int x_offset = 0;
    int y_offset = 0;
    for (int i = 0; i < len; i++)
    {
        if (text[i] != 0x20)
        {
            draw_symbol(text[i], x + x_offset, y + y_offset, scale);
            x_offset += scale * 6;
        }
        else
        {
            x_offset = 0;
            y_offset += scale * 6;
        }
    }
}


int get_symbol_data_offset(int sym)
{
    for (int i = 0; i < FONT_SYMBOL_COUNT; i++)
    {
        int s = font[i * FONT_SYMBOL_SIZE] | (font[i * FONT_SYMBOL_SIZE + 1] << 8);
        if (s == sym)
            return 2 + i * FONT_SYMBOL_SIZE;
    }
    return -1;
}

void draw_symbol(int sym, int x, int y, int scale)
{
    int data_offset = get_symbol_data_offset(sym);
    if (data_offset == -1)
        return;

    for (int i = 0; i < 5; i++)
    {
        unsigned char line = font[data_offset + i];
        for (int j = 0; j < 5; j++)
        {
            if ((line & 0x10) != 0)
                draw_filled_rect(x + j * scale, y + i * scale, x + (j + 1) * scale, y + (i + 1) * scale);
            line = line << 1;
        }
    }
}

int get_multitext_height(char* text)
{
    int len = _strlen(text);
    int height = 5;
    for (int i = 0; i < len; i++)
    {
        if (text[i] == 0x20)
        {
            height += 6;
        }
    }
    return height;
}