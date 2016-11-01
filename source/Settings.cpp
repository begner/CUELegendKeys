#include "Settings.h"

Settings* Settings::theInstance = NULL;

Settings* Settings::getInstance()
{
	if (theInstance == NULL)
		theInstance = new Settings();
	return theInstance;
}

void Settings::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

Settings::Settings(void) {
	NuLogger::getInstance()->log("Settings Constructor called!");

	ini.SetUnicode();
	ini.LoadFile(getFQNAppDataFile(settingsFile).c_str());
}

Settings::~Settings(void) {
}

bool Settings::checkSettings() {
	return checkOrCreateEmptyFile();
}

bool Settings::checkOrCreateEmptyFile() {
	
	string sPath = getAppDataPath();
	if (sPath == "") {
		NuLogger::getInstance()->log("Cant get Local App Folder");
		return false;
	}
	SHCreateDirectoryEx(NULL, s2ws(sPath).c_str(), NULL);

	string sFile = getFQNAppDataFile(settingsFile);

	// if file not exists
	if (!ifstream(sFile))
	{
		
		// create
		ofstream file(sFile);
		if (!file)
		{
			// if not created
			NuLogger::getInstance()->log("File could not be created");
			return false;
		}
		file.close();
		
	}

	setValue("Main", "initCheck", true);
	int checkValue = getValue("Main", "initCheck", false);
	if (!checkValue) {
		NuLogger::getInstance()->log("Cant update ini value");
		return false;
	}
	removeValue("Main", "initCheck");
	commit();

	return true;
}



int Settings::getValue(string section, string keyName, int default) {
	
	stringstream defaultSs;
	defaultSs << default;
	string defaultS = defaultSs.str();

	string value = getValue(section, keyName, defaultS);
	int ret = stoi(value);
	return ret;
}


string Settings::getValue(string section, string keyName, string default) {
	const char * pVal = ini.GetValue(section.c_str(), keyName.c_str(), default.c_str());
	string value = string(pVal);
	return value;
}



void Settings::setValue(string section, string keyName, int data) {
	stringstream ss;
	ss << data;
	string str = ss.str();
	setValue(section, keyName, str);
}


void Settings::setValue(string section, string keyName, string data) {

	SI_Error rc;
	rc = ini.SetValue(section.c_str(), keyName.c_str(), data.c_str());
	
	if (autoCommit) {
		rc = ini.SaveFile(getFQNAppDataFile(settingsFile).c_str(), true);
		// NuLogger::getInstance()->log("SaveFile %s => %i", getFQFN().c_str(), rc);
	}
}

void Settings::commit() {
	if (!autoCommit) {
		ini.SaveFile(getFQNAppDataFile(settingsFile).c_str(), true);
		setAutoCommit(true);
	}
}

void Settings::removeValue(string section, string keyName) {
	ini.Delete(section.c_str(), keyName.c_str());
}



void Settings::setAutoCommit(bool state) {
	autoCommit = state;
}


string Settings::getFQNAppDataFile(string fileName) {
	return getAppDataPath() + "\\" + settingsFile;
}


string Settings::getAppDataPath() {

	string settingsPath = "";

	wchar_t* localAppData = new wchar_t[128];
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppData);

	settingsPath = ws2s(localAppData);
	settingsPath = settingsPath + "\\CUELegendKeys";

	return settingsPath;
}