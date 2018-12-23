#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <dirent.h>

using namespace std;


#include "partition.h"
#include "entry.h"
#include "entryfile.h"
#include "ui.h"


extern string errorLog;

extern vector<Entry> entries;


bool fileExists(string filename);

string trim(string str);
string lower(string str);

string loadFile(string filename);
void saveFile(string filename, string data);

#endif // COMMON_H
