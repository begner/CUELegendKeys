#pragma once

#include <stddef.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include "convert.h"
#include <iostream>
#include "AppData.h"

using namespace std;

class NuLogger
{
private:

	NuLogger(void);
	~NuLogger(void);
	bool writeLog = true; 
	
	
	static NuLogger * theInstance;
	ofstream logFileStream;
public:

	static NuLogger * getInstance();
	static void release();

	void log(string msg, ...);
	void writeLogFile(bool state);
	bool getWriteLog();

};

