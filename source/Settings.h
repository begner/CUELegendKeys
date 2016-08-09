#pragma once

#include <string>
#include <windows.h>

#include <Shlobj.h>//for knownFolder 

#include "libs\SimpleIni.h"

#include "convert.h"
#include "NuLogger.h"

using namespace std;


class Settings
{
public:
	static Settings * getInstance();
	static void release();
	void saveValue(string keyName, string data);
	boolean checkSettings();

	void setValue(string section, string keyName, int data);
	void setValue(string section, string keyName, string data);

	int getValue(string section, string keyName, int default = 0);
	string getValue(string section, string keyName, string default = "");

	void setAutoCommit(boolean state);
	void commit();
private:
	string settingsPath = "";
	string settingsFile = "settings.ini";
	CSimpleIniA ini;
	boolean autoCommit = true;
	string getFQFN();
	Settings(void);
	~Settings(void);

	static Settings * theInstance;
	boolean checkOrCreateEmptyFile();
	string getSettingsPath();
};


