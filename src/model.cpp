#include "model.h"
#include "common.h"
#include <string>
#include <iostream>

std::string modelDtbPath;

void detectModel() {
    modelDtbPath = loadFile("/proc/tegra3_project");
    std::cout << "dtbootmenu on " << modelDtbPath << std::endl;
    modelDtbPath += + ".dtb";
}
