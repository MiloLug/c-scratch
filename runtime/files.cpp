#include "files.h"


int64_t File::getwline(wchar_t ** out) {
    constexpr int64_t bufferSize = 64;
    constexpr int64_t bufferByteSize = sizeof(wchar_t) * bufferSize;
    // Not counting 0 ending
    constexpr int64_t bufferStrSize = bufferSize - 1;
    constexpr int64_t bufferStrByteSize = sizeof(wchar_t) * bufferStrSize;
    constexpr int64_t buffersNumber = 8;

    wchar_t * buffers[buffersNumber];
    wchar_t * buffer;
    int64_t lineSize = 0;
    int64_t readSize;
    int64_t offset = 0;
    int64_t copiesNum = 0;

    while (true) {
        for (uint64_t i = 0; i < buffersNumber; i++) {
            buffer = buffers[i] = (wchar_t *)malloc(bufferByteSize);

            if (fgetws(buffer, bufferSize, fp) == nullptr) {
                free(buffer);
                break;
            }

            readSize = wcslen(buffer);
            lineSize += readSize;
            if (buffer[readSize] == L'\n' || (readSize + 1) < bufferSize) break;
        }

        if (lineSize != offset) {
            *out = (wchar_t *)realloc(*out, sizeof(wchar_t) * (lineSize + 1));
            readSize = lineSize - offset;
            copiesNum = readSize / bufferStrSize;

            for (uint64_t i = 0; i < copiesNum; i++) {
                memcpy(*out + offset, buffers[i], bufferStrByteSize);
                offset += bufferStrSize;

                free(buffers[i]);
            }

            if (lineSize != offset) {
                memcpy(
                    *out + offset,
                    buffers[copiesNum],
                    (readSize - copiesNum * bufferStrSize) * sizeof(wchar_t)
                );
                offset += bufferSize;

                free(buffers[copiesNum]);

                // The string ends before the buffer, so it's been read fully
                (*out)[lineSize] = L'\0';
                break;
            }

            (*out)[lineSize] = L'\0';

            // it's obvious the line is ended if it didn't fill all the buffers
            if (buffers[buffersNumber - 1] == nullptr || (*out)[lineSize - 1] == L'\n') break;
        } else {
            break;
        }
    }

    if (lineSize == 0) return -1;

    if ((*out)[lineSize - 1] == L'\n') {
        lineSize--;
        if (lineSize != 0 && (*out)[lineSize - 1] == L'\r') {
            lineSize--;
        }
        (*out)[lineSize] = L'\0';
    }

    return lineSize;
}
