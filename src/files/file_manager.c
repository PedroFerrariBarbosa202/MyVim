#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "editor_types.h"
#include "buffer.h"

char *get_curr_build_path()
{
    char *path = malloc(PATH_MAX);

    ssize_t len = readlink("/proc/self/exe", path, PATH_MAX - 1);
    if (len == -1) {
        free(path);
        return NULL;
    }

    path[len] = '\0';

    char *dir = strdup(dirname(path));

    free(path);
    return dir;
}

void set_current_directory(editor_ctx *edt_ctx, char *new_dir){
    window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);

    curr_window->curr_dir = realloc(
        curr_window->curr_dir,
        strlen(new_dir) + 1
    );

    strcpy(curr_window->curr_dir, new_dir);
}

void set_current_file(editor_ctx *edt_ctx, char *new_file){
    window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);

    curr_window->curr_file = realloc(
        curr_window->curr_file,
        strlen(new_file) + 1
    );

    strcpy(curr_window->curr_file, new_file);
}

int load_buffers_from_static_dir(editor_ctx *edt_ctx, char *file_name){
    window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);
    FILE *fp;
    int c;
    int curr_row = 0;

    fp = fopen(file_name, "r");

    if(fp == NULL) return 1;

    // set current file
    set_current_file(edt_ctx, file_name);

    clear_rows(edt_ctx);
    create_new_erow(edt_ctx);

    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            curr_row++;
            create_new_erow(edt_ctx);
            continue;
        }

        char tmp[2] = {c, '\0'};

        append_str_erow(
            NULL,
            &curr_window->rows[curr_row],
            tmp,
            curr_window->rows[curr_row].size
        );
    }

    fclose(fp);

    curr_window->cy = 1;
    curr_window->cy = 1;
    curr_window->y_offset = 0;
    return 0;
}


int load_buffers_from_file(editor_ctx *edt_ctx, char *file_name){
    window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);

    FILE *fp;
    int c;
    int curr_row = 0;

    char *curr_dir;
    curr_dir = (curr_window->curr_dir != NULL ? curr_window->curr_dir : get_curr_build_path());

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", curr_dir, file_name);
    fp = fopen(path, "r");

    if(fp == NULL) return 1;

    // set current file and directory
    set_current_file(edt_ctx, file_name);
    set_current_directory(edt_ctx, curr_dir);

    clear_rows(edt_ctx);
    create_new_erow(edt_ctx);

    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            curr_row++;
            create_new_erow(edt_ctx);
            continue;
        }

        char tmp[2] = {c, '\0'};

        append_str_erow(
            NULL,
            &curr_window->rows[curr_row],
            tmp,
            curr_window->rows[curr_row].size
        );
    }

    fclose(fp);

    curr_window->cy = 1;
    curr_window->cy = 1;
    curr_window->y_offset = 0;
    return 0;
}

int save_file_from_buffers(editor_ctx *edt_ctx, char *new_name){
    window_t *curr_window = &(edt_ctx->windows[edt_ctx->curr_window]);

    FILE *fp;
    int curr_row = 0;

    char curr_dir[512];
    sprintf(curr_dir, "%s/%s", 
            curr_window->curr_dir == NULL ? "" : curr_window->curr_dir,
            new_name == NULL ? curr_window->curr_file : new_name);

    fp = fopen(curr_dir, "w");

    if(fp == NULL) return 1;
   
    // change current file if a new name is requested
    if(new_name != NULL) set_current_file(edt_ctx, new_name);

    while(curr_row < curr_window->numrows){
        char *buffer = curr_window->rows[curr_row].buff;

        if(buffer != NULL)
            fputs(buffer, fp);
            fputs("\n", fp);
        curr_row++;
    }

    fclose(fp);
    
    curr_window->file_modified = 0;
    return 0;
}

