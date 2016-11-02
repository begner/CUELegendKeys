#pragma once

#include <string>
#include <Shlobj.h> //for knownFolder 
#include "convert.h"

using namespace std;

class AppData
{
public:
	AppData();
	~AppData();
	static string getFQNAppDataFile(string fileName);
	static string getAppDataPath();
	static void createFolder();
};

