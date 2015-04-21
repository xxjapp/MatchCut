#pragma once

typedef unsigned char BYTE;

class BufferedFileReader
{
public:
    BufferedFileReader(const char *path, size_t bufSize = 1048576);
    ~BufferedFileReader();

public:
    size_t read(void *buffer, size_t bufferSize);
    size_t read(const void *&buffer);

protected:
    size_t readOnce(void *buffer, size_t bufferSize);
    void internalRead();

private:
    FILE *fp;
    bool end;

    BYTE *buf;
    size_t bufSize;

    const BYTE *content;
    size_t contentSize;
};
