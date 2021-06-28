#pragma once

#include <string>
#include <optional>
#include "entry.h"

class EntryParser {
    private:
        std::string path;
        std::string filename;
        std::string partitionPath;

        std::string param;
        std::string value;
        size_t lineNumber;

        std::optional<std::string> kernel, initrd, title, cmdline, dtbFile;
        bool legacy;

        void elog(std::string msg) const;
        void checkStringParam(std::optional<std::string> &save) const;
        void checkFileParam(std::optional<std::string> &save) const;

    public:
        EntryParser(std::string partitionPath, std::string path, std::string filename);
        std::optional<Entry> parse();
};
