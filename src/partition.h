#ifndef PARTITION_H
#define PARTITION_H

class Partition
{
    private:
        string path;

        void scanDir(string path);

    public:
        Partition(string _path = "");
        void scan(vector<string> directories = {"", "boot"});
};

#endif // PARTITION_H
