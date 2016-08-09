#pragma once

using namespace std;

#include <stddef.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include "convert.h"
#include <iostream>

class NuLogger
{
public:
	static NuLogger * getInstance();
	static void release();

	void log(string msg, ...);
private:
	NuLogger(void);
	~NuLogger(void);
	static NuLogger * theInstance;
	ofstream logFileStream;
};

