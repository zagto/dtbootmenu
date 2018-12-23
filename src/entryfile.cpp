#include "common.h"

void EntryFile::elog(string msg)
{
    errorLog += filename + ": line " + to_string(lineNumber) + ": " + msg + "\n";
}

void EntryFile::checkStringParam(string *save)
{
    if (value != "")
    {
        if (*save != "")
            elog("Overwriting propery \"" + param + "\"");
        *save = value;
    }
    else
    {
        elog("setting propery \"" + param + "\" to empty string is not allowed");
    }
}

void EntryFile::checkFileParam(string *save)
{
    if (fileExists(path + "/" + value))
        checkStringParam(save);
    else
        elog("File not found");
}

EntryFile::EntryFile(string _partitionPath, string _path, string _filename)
{
    partitionPath = _partitionPath;
    path = _path;
    filename = _filename;
}

Entry EntryFile::parse()
{
    kernel = initrd = title = cmdline = "";
    legacy = false;

    ifstream file(path + "/" + filename);
    if (file)
    {

        string line;
        lineNumber = 1;
        while (getline(file, line))
        {
            line = trim(line);
            if (line == "legacy")
            {
                legacy = true;
            }
            else if (!(line == "" || line[0] == '#'))
            {
                size_t pos = line.find(':');
                if (pos == string::npos)
                {
                    elog("No colon (':') found");
                }
                else
                {
                    param = trim(lower(line.substr(0, pos)));
                    value = trim(line.substr(pos + 1));
                    if (param == "kernel" || param == "initrd")
                    {
                        checkFileParam(param == "kernel" ? &kernel : &initrd);
                    }
                    else if (param == "title")
                    {
                        checkStringParam(&title);
                    }
                    else if (param == "cmdline")
                    {
                        if (value == "")
                            value = " ";
                        checkStringParam(&cmdline);
                    }
                }
            }
            lineNumber++;
        }
    }

    if (legacy && kernel != "" && initrd != "" && title != "")
        return Entry(partitionPath, title, path + "/" + kernel, path + "/" + initrd);
    else if (kernel != "" && initrd != "" && title != "" && cmdline != "")
        return Entry(partitionPath, title, path + "/" + kernel, path + "/" + initrd, cmdline);
    else
    {
        errorLog += filename + ": Incomplete entry file\n";
        return Entry();
    }
}

