#include "entry.h"
#include "common.h"
#include "model.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

Entry::Entry(std::string partition,
       std::string title,
       std::string zImage,
       std::string initrd,
       std::string cmdline,
       std::optional<std::string> dtbFile) :
    title{title + " (" + (dtbFile ? "custom DTB, " : "") + "on " + partition + ")"},
    zImage{zImage},
    initrd{initrd},
    cmdline{cmdline},
    dtbFile{dtbFile},
    partition{partition} {}

Entry::Entry(std::string partition, std::string title, std::string zImage, std::string initrd) :
    legacy{true},
    title{title + " (legacy, on " + partition + ")"},
    zImage{zImage},
    initrd{initrd} {}


void Entry::run() {
    mount(partition.c_str(), "/tmpmount", "ext4", MS_RDONLY, "");

    std::string actualCommandLine;
    if (legacy) {
        actualCommandLine = loadFile("/proc/cmdline");
        actualCommandLine = actualCommandLine.substr(actualCommandLine.find("tegraid="));
    } else {
        actualCommandLine = cmdline;
    }

    pid_t pid;
    pid = fork();

    std::string fullDtbPath;
    if (dtbFile) {
        fullDtbPath = *dtbFile;
    } else {
        fullDtbPath = std::string("/") + modelDtbPath;
    }
    
    if (pid == 0) {
        if (legacy)
            execl("/kexec",
                  "/kexec",
                  "--load",
                  zImage.c_str(),
                  "--initrd",
                  initrd.c_str(),
                  "--command-line",
                  actualCommandLine.c_str(),
                  "--mem-min=0x85000000",
                  "--atags",
                  NULL);
        else
            execl("/kexec",
                  "/kexec",
                  "--load",
                  zImage.c_str(),
                  "--initrd",
                  initrd.c_str(),
                  "--command-line",
                  actualCommandLine.c_str(),
                  "--mem-min=0x85000000",
                  "--dtb",
                  fullDtbPath.c_str(),
                  NULL);
    }
    int status;
    waitpid(pid, &status, 0);
    if (status != 0) {
        std::cout << "Loading Kexec kernel failed, status " << status << std::endl;
        while(1);
    }
    execl("/kexec", "/kexec", "-e", NULL);

}
