#ifndef __FONT_H__
#define __FONT_H__

void draw_text(char* text, int x, int y, int scale);
void draw_symbol(int sym, int x, int y, int scale);
int get_multitext_height(char* text);

#endif