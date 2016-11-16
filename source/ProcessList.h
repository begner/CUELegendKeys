#pragma once

#include "convert.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <TlHelp32.h>


#include "NuLogger.h"

using namespace std;

class ProcessList
{
private:
	ProcessList();
	~ProcessList();
	map<DWORD, wstring> cachedProcessList;

	static ProcessList * theInstance;
	string findWindowName;
	DWORD activeProcessId = -1;
	string getNameByProcessId(DWORD pid);
	HANDLE snapshot;
	HWND processHWND;
	vector<HWND> windowList = {};
	static BOOL CALLBACK HelperEnumMainWindow(HWND hwnd, LPARAM lParam);
	
	static BOOL CALLBACK HelperEnumChildWindows(HWND hwnd, LPARAM lParam);
public:
	static ProcessList * getInstance();
	static void release();

	

	void setProcessWindowHandle(HWND hwnd);
	DWORD getPIDofProcess(string processName);
	vector<DWORD> getPIDsofProcess(string processName);
	
	
	string getList(string prefix, DWORD curProcessId = 0);

	bool ProcessExists(string processName, DWORD curProcessId = 0);
	void makeSnapshot();
	
	bool checkProcessChanged();
	DWORD determineActiveProcessId();
	string getActiveProcessName();
	DWORD getActiveProcessId();
	HWND getWindowsByProcessName(string processName, string windowName = "", string className = "");
	
	void addToWindowList(HWND hwnd);
	void addMainWindowByProcessIdToWindowList(DWORD processId = NULL, string windowName = "");
	void addChildWindowsByMainWindowToWindowList(HWND mainWindow);
};



