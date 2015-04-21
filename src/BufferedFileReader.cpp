#include <stdlib.h>
#include <stdio.h>

#include <algorithm>

#include "BufferedFileReader.h"

BufferedFileReader::BufferedFileReader(const char *path, size_t bufSize /*= 1048576*/)
{
    this->fp = fopen(path, "rb");
    this->end = false;

    this->bufSize = bufSize;
    this->buf = (BYTE *)malloc(bufSize);

    this->contentSize = 0;
    this->content = buf;
}

BufferedFileReader::~BufferedFileReader()
{
    free(buf);
    fclose(fp);
}

size_t BufferedFileReader::read(void *buffer, size_t bufferSize)
{
    size_t readSize = 0;

    while (readSize < bufferSize) {
        size_t rSize = readOnce((BYTE *)buffer + readSize, bufferSize - readSize);

        if (rSize == 0) {
            break;
        }

        readSize += rSize;
    }

    return readSize;
}

size_t BufferedFileReader::readOnce(void *buffer, size_t bufferSize)
{
    if (end) {
        return 0;
    }

    if (contentSize == 0) {
        internalRead();
    }

    if (contentSize == 0) {
        end = true;
        return 0;
    }

    size_t copySize = std::min(contentSize, bufferSize);

    memcpy(buffer, content, copySize);

    content += copySize;
    contentSize -= copySize;

    return copySize;
}

void BufferedFileReader::internalRead()
{
    contentSize = fread(buf, 1, bufSize, fp);
    content = buf;
}
