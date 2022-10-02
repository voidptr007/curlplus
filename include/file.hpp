#ifndef FILE_HPP
#define FILE_HPP

#include <stdio.h>

struct File
{
    typedef enum FileMode
    {
        R,
        W,
        RW,
        NONE
    } FileMode;

    const char *filename;
    FILE *stream;
    File();
    File(const char *fname, const FileMode mode = NONE);
    size_t getFileSize();
    ~File();
};

#endif