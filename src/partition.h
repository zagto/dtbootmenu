#pragma once

#include <string>
#include <vector>

class Partition
{
    private:
        const std::string path;

        void scanDir(std::string path);

    public:
        Partition(std::string _path = "");

        void scan(std::vector<std::string> directories = {"", "boot"});
};
