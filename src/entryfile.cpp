#include "entryfile.h"
#include "common.h"
#include <fstream>
#include <iostream>

void EntryParser::elog(std::string msg) const {
    std::cerr << filename << ": line " << lineNumber << ": " << msg << std::endl;
}

void EntryParser::checkStringParam(std::optional<std::string> &save) const
{
    if (save) {
        elog("Overwriting propery \"" + param + "\"");
    }
    save = value;
}

void EntryParser::checkFileParam(std::optional<std::string> &save) const {
    if (fileExists(path + "/" + value)) {
        checkStringParam(save);
    } else {
        elog("File not found");
    }
}

EntryParser::EntryParser(std::string partitionPath, std::string path, std::string filename) :
    path{path},
    filename{filename},
    partitionPath{partitionPath} {}

std::optional<Entry> EntryParser::parse() {
    legacy = false;

    std::ifstream file(path + "/" + filename);
    if (!file) {
        std::cerr << "Could not open entry file " << filename << std::endl;
        return {};
    }

    std::string line;
    lineNumber = 1;

    while (getline(file, line)) {
        line = trim(line);
        if (line == "legacy") {
            legacy = true;
        } else if (!(line == "" || line[0] == '#')) {
            size_t pos = line.find(':');
            if (pos == std::string::npos) {
                elog("No colon (':') found");
            } else {
                param = trim(lower(line.substr(0, pos)));
                value = trim(line.substr(pos + 1));

                if (param == "kernel") {
                    checkFileParam(kernel);
                } else if (param == "initrd") {
                    checkFileParam(initrd);
                } else if (param == "dtb") {
                    checkFileParam(dtbFile);
                } else if (param == "title") {
                    checkStringParam(title);
                } else if (param == "cmdline") {
                    checkStringParam(cmdline);
                }
            }
        }
        lineNumber++;
    }

    if (legacy && cmdline) {
        std::cerr << filename + ": Both cmdline and legacy arguments set" << std::endl;
        return {};
    }

    if (legacy && kernel && initrd && title) {
        return Entry(partitionPath,
                     *title,
                     path + "/" + *kernel,
                     path + "/" + *initrd);
    } else if (kernel && initrd && title && cmdline) {
        return Entry(partitionPath,
                     *title,
                     path + "/" + *kernel,
                     path + "/" + *initrd,
                     *cmdline,
                     dtbFile);
    } else {
        std::cerr << filename + ": Incomplete entry file" << std::endl;
        return {};
    }
}

