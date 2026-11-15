#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

int readInt(const char *prompt, int min, int max) {
    char line[MAX_INPUT];
    char *end;
    long value;

    while (1) {
        readLine(prompt, line, sizeof(line));
        value = strtol(line, &end, 10);
        if (end != line && *end == '\0' && value >= min && value <= max) {
            return (int)value;
        }
        printf("Input tidak valid. Masukkan angka %d-%d.\n", min, max);
    }
}
