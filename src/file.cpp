#include <sys/stat.h>
#include "file.hpp"

File::File()
{
    filename = NULL;
    stream = NULL;
}

File::File(const char *fname, const FileMode mode)
{
    filename = fname;
    stream = NULL;
    if (mode == R)
    {
        stream = fopen(filename, "rb");
    }
    else if (mode == W)
    {
        stream = fopen(filename, "wb");
    }
    else if (mode == RW)
    {
        stream = fopen(filename, "ab+");
    }
}

size_t File::getFileSize()
{
    size_t fsize = 0;
    struct stat file_info;
    stream = fopen(filename, "rb");
    if (fstat(fileno(stream), &file_info) == 0)
    {
        fsize = file_info.st_size;
    }
    fclose(stream);
    return fsize;
}

File::~File()
{
    if (stream)
    {
        fclose(stream); /* close the local file */
    }
}