#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "BufferedFileWriter.h"

BufferedFileWriter::BufferedFileWriter(const char *path, size_t bufSize /*= 1048576*/)
{
    this->fp = fopen(path, "wb");

    this->bufSize = bufSize;
    this->buf = (BYTE *)malloc(bufSize);

    this->spaceSize = bufSize;
    this->space = buf;
}

BufferedFileWriter::~BufferedFileWriter()
{
    internalWrite();

    free(buf);
    fclose(fp);
}

size_t BufferedFileWriter::write(const void *buffer, size_t bufferSize)
{
    size_t writeSize = 0;

    while (writeSize < bufferSize) {
        size_t wSize = writeOnce((const BYTE *)buffer + writeSize, bufferSize - writeSize);
        writeSize += wSize;
    }

    return writeSize;
}

size_t BufferedFileWriter::writeOnce(const void *buffer, size_t bufferSize)
{
    if (spaceSize == 0) {
        internalWrite();
    }

    size_t copySize = std::min(spaceSize, bufferSize);

    memcpy(space, buffer, copySize);

    space += copySize;
    spaceSize -= copySize;

    return copySize;
}

void BufferedFileWriter::internalWrite()
{
    fwrite(buf, 1, bufSize - spaceSize, fp);

    spaceSize = bufSize;
    space = buf;
}
