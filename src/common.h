#pragma once

/*#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <dirent.h>
*/
#include <vector>

#include "entry.h"

extern std::vector<Entry> entries;


bool fileExists(std::string filename);

std::string trim(std::string str);
std::string lower(std::string str);

std::string loadFile(std::string filename);
void saveFile(std::string filename, std::string data);
