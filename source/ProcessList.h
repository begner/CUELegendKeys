#pragma once

#include "convert.h"
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

#include "NuLogger.h"

using namespace std;

class ProcessList
{
private:
	ProcessList();
	~ProcessList();
	static ProcessList * theInstance;
	string findWindowName;
	DWORD activeProcessId = -1;
	string getNameByProcessId(DWORD pid);
	HANDLE snapshot;
	HWND processHWND;
	static BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam);
public:
	static ProcessList * getInstance();
	static void release();

	void setProcessWindowHandle(HWND hwnd);
	DWORD getPIDofProcess(string processName);
	
	string getList(string prefix, DWORD curProcessId = 0);

	bool ProcessExists(string processName, DWORD curProcessId = 0);
	void makeSnapshot();
	bool checkProcessChanged();
	DWORD determineActiveProcessId();
	string getActiveProcessName();
	DWORD getActiveProcessId();
	HWND getProcessWindowHandle(DWORD processId = NULL, string windowName = "");
};



