#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * read_width_height reads the PBM width and height parameter from a given
 * line.
 * @param line The line to read.
 * @param line_length The length of the line.
 * @param width The returned width.
 * @param height The returned height.
 * @return Zero, if successful.
 */
int read_width_height(char *line, int line_length, int *width, int *height) {
    int ret = 0;
    int tok_idx = 0;
    char *line_copy = malloc(line_length + 1);
    if (!line_copy) {
        ret = -1;
        goto final;
    }
    strncpy(line_copy, line, line_length);
    char *line_tok = strtok(line_copy, " ");
    while (line_tok != NULL) {
        switch (tok_idx) {
            case 0:
                *width = atoi(line_tok);
                break;
            case 1:
                *height = atoi(line_tok);
                break;
            default:
                ret = -1;
                goto free_line_copy;
        }
        tok_idx++;
        line_tok = strtok(NULL, " ");
    }
    free_line_copy:
        free(line_copy);
    final:
        return ret;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: renderer [filename]\n");
        exit(1);
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("error opening file\n");
        goto final;
    }
    char line[256];
    if (!fgets(line, sizeof(line), f)) {
        printf("error reading header: no magic number found\n");
        goto close_file;
    }
    if (strncmp(line, "P1", 2) != 0) {
        printf("error reading header: expected 'P4' header\n");
        goto close_file;
    }
    if (!fgets(line, sizeof(line), f)) {
        printf("error reading header: no content after magic number\n");
        goto close_file;
    }
    int width, height;
    if (read_width_height(line, sizeof(line), &width, &height) != 0) {
        printf("error reading header: no width height\n");
        goto close_file;
    }
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            printf("\n");
        }
        if (c == '0') {
            printf(" ");
        }
        if (c == '1') {
            printf("X");
        }
    }
close_file:
    fclose(f);
final:
    exit(0);
}