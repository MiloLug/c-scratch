#ifndef CSCRATCH_FILES_H
#define CSCRATCH_FILES_H

#include <cstdio>
#include <filesystem>


class File {
public:
    FILE * fp;
    bool opened;

    File(const std::filesystem::path & path, const char * mode) {
        fp = fopen(path.c_str(), mode);
        opened = fp != nullptr;
        if (opened) fwide(fp, 1);
    }

    /// @brief Reads a line from the file
    /// @param out pointer to the string destination. It will contain the line excluding \\n or \\r\\n sequences
    /// @return line's length or -1 if it's the end of the file
    int64_t getwline(wchar_t ** out);

    void close() {
        if (fp) {
            fclose(fp);
        }
        opened = false;
    }

    ~File() { close(); }
};


#endif
