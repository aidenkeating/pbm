#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define P1_LINE_BUF_SIZE 256

/**
 * Reads the PBM width and height parameter from a given line.
 * @param line The line to read.
 * @param line_length The length of the line.
 * @param width The returned width.
 * @param height The returned height.
 * @return Zero, if successful.
 */
int read_width_height(
    const char *line,
    int line_length,
    int *width,
    int *height
) {
    int ret = 0;
    int tok_idx = 0;
    char *line_copy = malloc(line_length + 1);
    if (!line_copy) {
        ret = -1;
        goto final;
    }
    strncpy(line_copy, line, line_length);
    const char *line_tok = strtok(line_copy, " ");
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

/**
 * Returns the format version of a file, using the given line as the version
 * line.
 * @param line The line to read.
 * @param line_length The length of the line buffer.
 * @return The format version, if successful. Else, zero.
 */
int read_format(const char *line, const int line_length) {
    const size_t content_length = strnlen(line, line_length);
    if (content_length != 3) {
        return 0;
    }
    if (line[0] != 'P' || line[2] != '\n') {
        return 0;
    }
    return line[1] - '0';
}

int main(const int argc, char *argv[]) {
    int ret = 0;
    if (argc != 2) {
        printf("usage: renderer [filename]\n");
        ret = 1;
        goto final;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("error opening file\n");
        ret = 1;
        goto final;
    }
    char line[P1_LINE_BUF_SIZE];
    if (!fgets(line, sizeof(line), f)) {
        printf("error reading header: no magic number found\n");
        ret = 1;
        goto close_file;
    }
    const int format_version = read_format(line,P1_LINE_BUF_SIZE);
    if (format_version < 1) {
        printf("error reading header: invalid format\n");
        ret = 1;
        goto close_file;
    }
    if (format_version != 1) {
        printf("error reading header: only P1 is currently supported\n");
        ret = 1;
        goto close_file;
    }
    if (!fgets(line, P1_LINE_BUF_SIZE, f)) {
        printf("error reading header: no content after magic number\n");
        ret = 1;
        goto close_file;
    }
    int width, height;
    if (read_width_height(line, P1_LINE_BUF_SIZE, &width, &height) != 0) {
        printf("error reading header: no width height\n");
        ret = 1;
        goto close_file;
    }
    int c;
    while ((c = fgetc(f)) != EOF) {
        switch (c) {
            case '\n':
                printf("\n");
                break;
            case '0':
                printf(" ");
                break;
            case '1':
                printf("X");
                break;
            case ' ':
                break;
            default:
                ret = 1;
                goto close_file;
        }
    }
close_file:
    fclose(f);
final:
    exit(ret);
}