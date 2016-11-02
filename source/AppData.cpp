#include "AppData.h"



AppData::AppData()
{
}


AppData::~AppData()
{
}

string AppData::getFQNAppDataFile(string fileName) {
	return getAppDataPath() + "\\" + fileName;
}

string AppData::getAppDataPath() {

	string settingsPath = "";

	wchar_t* localAppData = new wchar_t[128];
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppData);

	settingsPath = ws2s(localAppData);
#ifdef _DEBUG
	settingsPath = settingsPath + "\\CUELegendKeysDEV";
#else
	settingsPath = settingsPath + "\\CUELegendKeys";
#endif
	return settingsPath;
}

void AppData::createFolder() {
	SHCreateDirectoryEx(NULL, s2ws(getAppDataPath()).c_str(), NULL);
}
