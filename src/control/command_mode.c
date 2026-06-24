#include <unistd.h>
#include <stdio.h>

#include "screen.h"
#include "buffer.h"
#include "editor_types.h"
#include "terminal.h"
#include "commands.h"
#include "windows.h"

void handle_command_mode(editor_ctx *edt_ctx, int key){
  window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);

  // DELETE key logic
  if(key == 127) {
    if(delete_char_commd_erow(edt_ctx) == 0)
      edt_ctx->commd_row_cx--;
  }
  // ENTER key logic
  else if(key == '\r' || key == '\n'){
    // execute command
    exec_command(edt_ctx, edt_ctx->commd_row.buff);
  
    // clean command row
    edt_ctx->commd_row.buff = NULL;
    edt_ctx->commd_row_cx = 1;
    edt_ctx->curr_opm = NORMAL;
    return;
  }
  else{
    insert_char_commd_erow(edt_ctx, key);
    edt_ctx->commd_row_cx++;
  }
}
