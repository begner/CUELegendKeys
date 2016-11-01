#pragma once

#include <string>
#include <windows.h>
#include <Shlobj.h>//for knownFolder #include "libs\SimpleIni.h"
#include "libs\SimpleIni.h"

#include "convert.h"
#include "NuLogger.h"

using namespace std;


class Settings
{
private:
	string settingsFile = "settings.ini";
	CSimpleIniA ini;
	bool autoCommit = true;
	string getFQFN();
	Settings(void);
	~Settings(void);

	static Settings * theInstance;
	bool checkOrCreateEmptyFile();
	
public:
	static Settings * getInstance();
	static void release();
	void saveValue(string keyName, string data);
	bool checkSettings();

	void setValue(string section, string keyName, int data);
	void setValue(string section, string keyName, string data);

	int getValue(string section, string keyName, int default = 0);
	string getValue(string section, string keyName, string default = "");

	void setAutoCommit(bool state);
	string getFQNAppDataFile(string fileName);
	string getAppDataPath();
	void commit();
	void removeValue(string section, string keyName);
	
};


