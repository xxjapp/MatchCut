#pragma once

typedef unsigned char BYTE;

class BufferedFileWriter
{
public:
    BufferedFileWriter(const char *path, size_t bufSize = 1048576);
    ~BufferedFileWriter();

public:
    size_t write(const BYTE *buffer, size_t bufferSize);

protected:
    size_t writeOnce(const BYTE *buffer, size_t bufferSize);
    void internalWrite();

private:
    FILE *fp;

    BYTE *buf;
    size_t bufSize;

    BYTE *space;
    size_t spaceSize;
};
