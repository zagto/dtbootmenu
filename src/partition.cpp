#include "common.h"

bool checkExtension(string name, string ext)
{
    if (name.length() >= ext.length())
    {
        string extension = name.substr(name.length() - ext.length());
        extension = lower(extension);
        if (extension == ext)
            return true;
    }
    return false;
}

void Partition::scanDir(string dirname)
{
    DIR *dir;
    dir = opendir(dirname.c_str());
    if (dir)
    {
        dirent *entry = NULL;
        while ((entry = readdir(dir)) != NULL)
        {
            string name = entry->d_name;
#ifdef DEBUG_SCAN
            cout << "Found file " << entry->d_name << ".\n";
#endif
            if (checkExtension(name, ".dtbootmenu"))
            {
                Entry entry = EntryFile(path, dirname, name).parse();
                if (entry.isValid())
                    entries.push_back(entry);
            }

            if (checkExtension(name, ".dtb"))
            {
                cout << "Unsing external " << name << " from " << path << ".\n";
                saveFile("/" + name, loadFile(dirname + "/" + name));
            }
        }
    }
}

Partition::Partition(string _path)
{
    path = _path;
}

void Partition::scan(vector<string> directories)
{
#ifdef DEBUG_SCAN
    cout << "Scanning " + path + "\n";
#endif
    if (mount(path.c_str(), "/tmpmount", "ext4", MS_RDONLY, "") == 0)
    {
#ifdef DEBUG_SCAN
        cout << "Found supported file system on " + path + ".\n";
#endif

        for (string &d: directories)
        {
#ifdef DEBUG_SCAN
            cout << "Scanning " + d + " on " + path + "\n";
#endif
            scanDir("/tmpmount/" + d);
        }

        umount("/tmpmount");

    }
}
