#include "common.h"

string dtbPath;

void detectModel() {
    dtbPath = loadFile("/proc/tegra3_project");
    cout << "dtbootmenu on " << dtbPath << endl;
}
