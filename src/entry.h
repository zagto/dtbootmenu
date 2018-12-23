#ifndef ENTRY_H
#define ENTRY_H


class Entry
{
    private:
        string title;
        bool legacy = false;
        bool valid = false;

        void fitText();

    public:
        string initrd, zImage, cmdline;
        string partition;

        // regular
        Entry(string _partition, string _title, string _zImage, string _initrd, string _cmdline);
        // legacy
        Entry(string _partition, string _title, string _zImage, string _initrd);
        // invalid
        Entry();

        bool isValid();
        string getTitle();
        void run();
};

#endif // ENTRY_H
