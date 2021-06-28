#include "common.h"
#include "ui.h"
#include "partition.h"
#include "model.h"
#include <sys/stat.h>
#include <sys/mount.h>
#include <iostream>
#include <unistd.h>

void scanDisks(int delay)
{
    sleep(delay);
    entries = {};

    Partition("/dev/mmcblk0p8").scan({"media/0/boot"});

    for (int disk = 1; fileExists("/dev/mmcblk" + std::to_string(disk)); disk++)
    {
         Partition("/dev/mmcblk" + std::to_string(disk)).scan();
         for (int part = 1; fileExists("/dev/mmcblk" + std::to_string(disk) + "p" + std::to_string(part)); part++)
             Partition("/dev/mmcblk" + std::to_string(disk) + "p" + std::to_string(part)).scan();
    }

    for (int disk = 0; fileExists("/dev/sd" + (char)('a' + disk)); disk++)
    {
        Partition("/dev/sd" + std::to_string('a' + (char)disk)).scan();
        for (int part = 1; fileExists("/dev/sd" + std::to_string('a' + disk) + std::to_string(part)); part++)
            Partition("/dev/sd" + std::to_string('a' + disk) + std::to_string(part)).scan();
    }
}

int main()
{
    ui::initTerminal();

    mkdir("/tmpmount", 0777);
    if (mkdir("/dev", 0777) != 0)
        /*cout << "Failed to mkdir /dev\n"*/;
    if (mkdir("/sys", 0777) != 0)
        std::cout << "Failed to mkdir /sys\n";
    if (mkdir("/proc", 0777) != 0)
        std::cout << "Failed to mkdir /proc\n";
    if (mount(NULL, "/proc", "proc", 0, "") != 0)
        std::cout << "Failed to mount proc\n";
    if (mount(NULL, "/sys", "sysfs", 0, "") != 0)
        std::cout << "Failed to mount sysfs\n";
    if (mount(NULL, "/dev", "devtmpfs", 0, "") != 0)
        std::cout << "Failed to mount deftmpfs\n";

    detectModel();

    scanDisks(5);


    ui::mainLoop();
}
