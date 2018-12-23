#ifndef PARTITION_H
#define PARTITION_H

class Partition
{
    private:
        string path;
        bool override;

        void scanDir(string path);

    public:
        Partition(string _path = "", bool override = false);
        void scan(vector<string> directories = {"", "boot"});
};

#endif // PARTITION_H
