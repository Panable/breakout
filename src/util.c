#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* file_name)
{
    /* Get length of file */
    FILE* file = fopen(file_name, "rb");
    fseek(file, 0, SEEK_END);
    long sz = ftell(file);

    char* buf = malloc(sz + 1);

    fread(buf, 1, sz, file);
    buf[sz] = '\0';

    return buf;
}
