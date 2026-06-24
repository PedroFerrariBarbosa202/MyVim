#ifndef SCREEN
#define SCREEN

#include "editor_types.h"

void move_cursor(int row, int col);
void change_text_color(char *col_cd);
void reset_screen(editor_ctx *edt_ctx);

void render_screen(editor_ctx *edt_ctx);
void roll_screen(editor_ctx *edt_ctx, int y_offset);
void check_colored_words(editor_ctx *edt_ctx, erow *curr_row);
#endif
