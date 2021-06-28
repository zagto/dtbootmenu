#include "common.h"
#include "partition.h"
#include "entryfile.h"
#include <dirent.h>
#include <sys/mount.h>
#include <iostream>

bool checkExtension(std::string name, std::string ext)
{
    if (name.length() >= ext.length())
    {
        std::string extension = name.substr(name.length() - ext.length());
        extension = lower(extension);
        if (extension == ext)
            return true;
    }
    return false;
}

void Partition::scanDir(std::string dirname)
{
    DIR *dir;
    dir = opendir(dirname.c_str());
    if (dir)
    {
        dirent *entry = NULL;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
#ifdef DEBUG_SCAN
            cout << "Found file " << entry->d_name << ".\n";
#endif
            if (checkExtension(name, ".dtbootmenu"))
            {
                std::optional<Entry> entry = EntryParser(path, dirname, name).parse();
                if (entry)
                    entries.push_back(*entry);
            }

            if (checkExtension(name, ".dtb"))
            {
                std::cout << "Unsing external " << name << " from " << path << ".\n";
                saveFile("/" + name, loadFile(dirname + "/" + name));
            }
        }
    }
}

Partition::Partition(std::string path) :
    path{path} {}

void Partition::scan(std::vector<std::string> directories)
{
#ifdef DEBUG_SCAN
    cout << "Scanning " + path + "\n";
#endif
    if (mount(path.c_str(), "/tmpmount", "ext4", MS_RDONLY, "") == 0)
    {
#ifdef DEBUG_SCAN
        cout << "Found supported file system on " + path + ".\n";
#endif

        for (std::string &d: directories)
        {
#ifdef DEBUG_SCAN
            cout << "Scanning " + d + " on " + path + "\n";
#endif
            scanDir("/tmpmount/" + d);
        }

        umount("/tmpmount");

    }
}
