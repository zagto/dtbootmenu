#include "common.h"

Entry::Entry(string _partition, string _title, string _zImage, string _initrd, string _cmdline)
{
    partition = _partition;
    title = _title;
    zImage = _zImage;
    initrd = _initrd;
    cmdline = _cmdline;

    title = title + " (on " + partition + ")";
    valid = true;
}

Entry::Entry(string _partition, string _title, string _zImage, string _initrd)
{
    partition = _partition;
    title = _title;
    zImage = _zImage;
    initrd = _initrd;
    cmdline = "";

    title = title + " (on " + partition + ")";
    valid = true;
    legacy = true;
}

Entry::Entry()
{
}

bool Entry::isValid()
{
    return valid;
}

string Entry::getTitle()
{
    return title;
}

void Entry::run()
{
    mount(partition.c_str(), "/tmpmount", "ext4", MS_RDONLY, "");

    if (legacy)
    {
        cmdline = loadFile("/proc/cmdline");
        cmdline = cmdline.substr(cmdline.find("tegraid="));
    }

    if (cmdline == "")
    {
        cout << "No cmdline, aborting\n";
        while (1);
    }

    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        if (legacy)
            execl("/kexec", "/kexec", "--load", zImage.c_str(), "--initrd", initrd.c_str(), "--command-line", cmdline.c_str(), "--mem-min=0x85000000", "--atags", NULL);
        else
            execl("/kexec", "/kexec", "--load", zImage.c_str(), "--initrd", initrd.c_str(), "--command-line", cmdline.c_str(), "--mem-min=0x85000000", "--dtb", "/tegra30-tf300t.dtb", NULL);
    }
    int status;
    waitpid(pid, &status, 0);
    if (status != 0)
    {
        cout << "Loading Kexec kernel failed, status " << status << "\n";
        while(1);
    }
    execl("/kexec", "/kexec", "-e", NULL);

}
