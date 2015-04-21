#include <string.h>

#include <string>

#include "BufferedFileReader.h"
#include "BufferedFileWriter.h"
#include "mcut.h"

#define BUFFER_SIZE     1048576

/*
* Reverse memchr()
* Find the last occurrence of 'c' in the buffer 's' of size 'n'.
*/
void *memrchr(const void *s, int c, size_t n)
{
    const unsigned char *cp;

    if (n != 0) {
        cp = (unsigned char *)s + n;

        do {
            if (*(--cp) == (unsigned char)c) {
                return (void *)cp;
            }
        } while (--n != 0);
    }

    return (void *)0;
}

void parseContent(const char *content, size_t contentSize, const char *pattern, size_t &part1Size, size_t &part2Size)
{
    const char *patternPos = strstr(content, pattern);

    if (!patternPos) {
        const void *lfPos = memrchr(content, '\n', contentSize);

        if (!lfPos) {
            part1Size = contentSize;
        } else {
            part1Size = (const char *)lfPos - content + 1;
        }

        part2Size = 0;
    } else {
        const void *lfPos = memrchr(content, '\n', patternPos - content);

        if (!lfPos) {
            part1Size = 0;
        } else {
            part1Size = (const char *)lfPos - content + 1;
        }

        part2Size = contentSize - part1Size;
    }
}

void process(const char *file, const char *pattern)
{
    std::string dest1Name = std::string(file) + "_1";
    std::string dest2Name = std::string(file) + "_2";

    BufferedFileReader reader(file);
    BufferedFileWriter writer1(dest1Name.c_str());
    BufferedFileWriter writer2(dest2Name.c_str());

    char *content = (char *)malloc(BUFFER_SIZE + 1);
    size_t contentSize = 0;

    size_t part1Size = 0;
    size_t part2Size = 0;

    // search and write part1/part2 data
    while (true) {
        contentSize += reader.read(content + contentSize, BUFFER_SIZE - contentSize);

        if (contentSize == 0) {
            break;
        }

        content[contentSize] = 0;

        parseContent(content, contentSize, pattern, part1Size, part2Size);

        // printf("part1Size = %ld, part2Size = %ld\n", part1Size, part2Size);
        putchar((part2Size > 0) ? '|' : '.');

        writer1.write(content, part1Size);
        writer2.write(content + part1Size, part2Size);

        if (part2Size > 0) {
            break;
        }

        contentSize -= part1Size;
        memmove(content, content + part1Size, contentSize);
    }

    free(content);

    // write part2 data
    const void *data = NULL;
    size_t dataSize = 0;

    while ((dataSize = reader.read(data)) > 0) {
        // printf("dataSize = %ld\n", 0, dataSize);
        putchar('.');

        writer2.write(data, dataSize);
    }

    putchar('\n');
}

void printHelp()
{
    printf("mcut: Cut a file based on match\n");
    printf("Usage\n");
    printf("$ mcut <pattern> <input_file>\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printHelp();
        return 1;
    }

    const char *pattern = argv[1];
    const char *file = argv[2];

    printf("pattern = %s\n", pattern);
    printf("file    = %s\n", file);
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");

    process(file, pattern);

    return 0;
}
