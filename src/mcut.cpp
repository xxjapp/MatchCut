#include <stdio.h>

#include <fstream>
#include <string>

#include "mcut.h"

#define BUF_SIZE    524288
#define USE_REGEX   0

#if USE_REGEX
#   include <regex>
#   define MATCH(line, ex)     std::regex_search(line, ex)
#else
#   define MATCH(line, ex)     (line.find(ex) != std::string::npos)
#endif

long long searchPatternInFile(const char *file, const char *pattern)
{
    std::ifstream infile(file, std::ifstream::binary);

    if (!infile) {
        return -1;
    }

#if USE_REGEX
    std::regex ex(pattern);
#else
    std::string ex = pattern;
#endif

    std::string line;
    bool found = false;

    while (std::getline(infile, line)) {
        if (MATCH(line, ex)) {
            found = true;
            break;
        }

    }

    if (!found) {
        return -2;
    }

    return lineOffset;
}

void splitFileAtOffset(const char *file, long long offset)
{
    std::string dest1Name = std::string(file) + "_1";
    std::string dest2Name = std::string(file) + "_2";

    FILE *source = fopen(file, "rb");
    FILE *dest1 = fopen(dest1Name.c_str(), "wb");
    FILE *dest2 = fopen(dest2Name.c_str(), "wb");

    char buf[BUF_SIZE];
    long long size;
    long long writeSize = 0;
    bool part1 = true;

    while (size = fread(buf, 1, BUF_SIZE, source)) {
        if (part1) {
            writeSize += size;

            if (writeSize < offset) {
                fwrite(buf, 1, size_t(size), dest1);
            } else {
                fwrite(buf, 1, size_t(size - (writeSize - offset)), dest1);
                fwrite(buf + size_t(size - (writeSize - offset)), 1, size_t(writeSize - offset), dest2);
                part1 = false;
            }
        } else {
            fwrite(buf, 1, size_t(size), dest2);
        }
    }

    fclose(dest2);
    fclose(dest1);
    fclose(source);
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
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n", file);

    long long offset = searchPatternInFile(file, pattern);

    printf("offset  = %lld\n", offset);

    if (offset == -2) {
        fprintf(stderr, "can not found pattern '%s' in file '%s'\n", pattern, file);
    } else if (offset == -1) {
        fprintf(stderr, "can not read '%s'\n", file);
    } else if (offset == 0) {
        fprintf(stdout, "first line of file '%s' matched\n", file);
    } else {
        splitFileAtOffset(file, offset);
    }

    return (int)offset;
}
