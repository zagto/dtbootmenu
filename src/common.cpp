#include "common.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>

std::string errorLog = "";

std::vector<Entry> entries;


bool fileExists(std::string filename)
{
    struct stat st;
    return stat(filename.c_str(), &st) == 0;
}

std::string trim(std::string str)
{
    str.erase(str.begin(), find_if_not(str.begin(), str.end(), std::ptr_fun<int, int>(isspace)));
    str.erase(find_if_not(str.rbegin(), str.rend(), std::ptr_fun<int, int>(isspace)).base(), str.end());
    return str;
}

std::string lower(std::string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string loadFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Unable to load file " << filename << std::endl;
        return "";
    }

    std::string result = "";
    while (file.good())
    {
        char c;
        file.read(&c, 1);
        if (file.good())
            result += c;
    }
    file.close();

    return result;
}

void saveFile(std::string filename, std::string data)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Unable to write file " << filename << std::endl;
        return;
    }

    file.write(&data[0], data.length());
    file.close();
}
