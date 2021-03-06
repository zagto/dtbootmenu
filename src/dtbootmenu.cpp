#include "common.h"

void scanDisks(int delay)
{
    sleep(delay);
    entries = {};

    Partition("/dev/mmcblk0p8", true).scan({"media/0/boot"});

    for (int disk = 1; fileExists("/dev/mmcblk" + to_string(disk)); disk++)
    {
         Partition((string)"/dev/mmcblk" + to_string(disk)).scan();
         for (int part = 1; fileExists("/dev/mmcblk" + to_string(disk) + "p" + to_string(part)); part++)
             Partition((string)"/dev/mmcblk" + to_string(disk) + "p" + to_string(part)).scan();
    }

    for (int disk = 0; fileExists((string)"/dev/sd" + (char)('a' + disk)); disk++)
    {
        Partition((string)"/dev/sd" + (char)('a' + (char)disk)).scan();
        for (int part = 1; fileExists("/dev/sd" + (char)('a' + disk) + to_string(part)); part++)
            Partition((string)"/dev/sd" + (char)('a' + disk) + to_string(part)).scan();
    }
}

int main()
{
    ui::initTerminal();

    mkdir("/tmpmount", 0777);
    if (mkdir("/dev", 0777) != 0)
        /*cout << "Failed to mkdir /dev\n"*/;
    if (mkdir("/sys", 0777) != 0)
        cout << "Failed to mkdir /sys\n";
    if (mkdir("/proc", 0777) != 0)
        cout << "Failed to mkdir /proc\n";
    if (mount(NULL, "/proc", "proc", 0, "") != 0)
        cout << "Failed to mount proc\n";
    if (mount(NULL, "/sys", "sysfs", 0, "") != 0)
        cout << "Failed to mount sysfs\n";
    if (mount(NULL, "/dev", "devtmpfs", 0, "") != 0)
        cout << "Failed to mount deftmpfs\n";

    detectModel();

    scanDisks(1);


    ui::mainLoop();
}
