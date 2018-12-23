#ifndef ENTRYFILE_H
#define ENTRYFILE_H


class EntryFile
{
    private:
        string path, filename;
        string partitionPath;
        string param, value;
        size_t lineNumber;

        string kernel, initrd, title, cmdline;
        bool legacy;

        void elog(string msg);
        void checkStringParam(string *save);
        void checkFileParam(string *save);

    public:
        EntryFile(string _partitionPath, string _path, string _filename);
        Entry parse();
};


#endif // ENTRYFILE_H
