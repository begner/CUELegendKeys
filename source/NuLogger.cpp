

#include "NuLogger.h"


NuLogger* NuLogger::theInstance = NULL;

NuLogger* NuLogger::getInstance()
{
	if (theInstance == NULL)
		theInstance = new NuLogger();
	return theInstance;
}

void NuLogger::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

NuLogger::NuLogger(void) {
	AppData::createFolder();
	logFileStream.open(AppData::getFQNAppDataFile("log.txt"), ios::app);
}

NuLogger::~NuLogger(void) {
	logFileStream.close();
}

bool NuLogger::getWriteLog() {
	return writeLog;
}

void NuLogger::writeLogFile(bool state) {
	writeLog = state;
}

void NuLogger::log(string msg, ...) {

#ifndef _DEBUG
	if (!writeLog) {
		return;
	}
#endif
	
	msg += "\n";
	

	char buffer[2048];
	va_list args;
	va_start(args, msg);
	vsprintf_s(buffer, msg.c_str(), args);
	perror(buffer);
	va_end(args);
	string smsg = string(buffer);

	// create Timestamp
	SYSTEMTIME st;
	GetLocalTime(&st);
	char bufferTime[80];

	int myDay = (INT)st.wDay;
	int myMonth = (INT)st.wMonth;
	int myYear = (INT)st.wYear;
	int myHour = (INT)st.wHour;
	int myMin = (INT)st.wMinute;
	int mySec = (INT)st.wSecond;
	sprintf_s(bufferTime, 80, "%02d.%02d.%04d / %02d:%02d:%02d", myDay, myMonth, myYear, myHour, myMin, mySec);
	string timeString = string(bufferTime);
	
	// concat message & time
	smsg = timeString + string(" - ") + smsg;

	// convert to wstring to wchar
	wstring wmsg = utf8_decode(smsg);
	WCHAR * newText = (WCHAR*)wmsg.c_str();
	
	#ifdef _DEBUG
		OutputDebugStringW(newText);
	#endif // _DEBUG	
	
	#ifdef _DEBUG
		if (!writeLog) {
			return;
		}
	#endif
	// save to file
	logFileStream << smsg;
	logFileStream.flush();


}