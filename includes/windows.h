#ifndef WINDOWS
#define WINDOWS

#include "editor_types.h"

int create_window(editor_ctx *edt_ctx);
void refresh_curr_window(editor_ctx *edt_ctx);
int change_window(editor_ctx *edt_ctx, int new_window);
void open_window_by_id(editor_ctx *edt_ctx, int id);

#endif
