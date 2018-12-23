#include "common.h"

string errorLog = "";

vector<Entry> entries;


bool fileExists(string filename)
{
    struct stat st;
    return stat(filename.c_str(), &st) == 0;
}

string trim(string str)
{
    str.erase(str.begin(), find_if_not(str.begin(), str.end(), ptr_fun<int, int>(isspace)));
    str.erase(find_if_not(str.rbegin(), str.rend(), ptr_fun<int, int>(isspace)).base(), str.end());
    return str;
}

string lower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string loadFile(string filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << "Unable to load file " << filename << "\n";
        return "";
    }

    string result = "";
    while (file.good())
    {
        char c;
        file.read(&c, 1);
        result += c;
    }
    file.close();

    return result;
}

void saveFile(string filename, string data)
{
    ofstream file(filename);
    if (!file)
    {
        cout << "Unable to write file " << filename << "\n";
        return;
    }

    file.write(&data[0], data.length());
    file.close();
}
