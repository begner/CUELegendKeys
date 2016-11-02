#include "ProcessList.h"



ProcessList* ProcessList::theInstance = NULL;

ProcessList* ProcessList::getInstance()
{
	if (theInstance == NULL)
		theInstance = new ProcessList();
	return theInstance;
}

void ProcessList::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}


ProcessList::ProcessList()
{
	makeSnapshot();
}


ProcessList::~ProcessList()
{
}

DWORD ProcessList::getActiveProcessId() {
	return activeProcessId;
}


BOOL CALLBACK ProcessList::EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	/*
#ifdef _DEBUG	
	if (lpdwProcessId == lParam) {
	
		if (GetWindow(hwnd, GW_OWNER) == (HWND)0) {
			CHAR szBuf[255];
			GetWindowTextA(hwnd, szBuf, 255);
			NuLogger::getInstance()->log("WindowName: \"%s\"", szBuf);
		}
	}
#endif 
	*/
	if (lpdwProcessId == lParam)
	{
		if (GetWindow(hwnd, GW_OWNER) == (HWND)0) {
			
			ProcessList* procL = ProcessList::getInstance();
			if (procL->findWindowName != "") {
				CHAR szBuf[255];
				GetWindowTextA(hwnd, szBuf, 255);
				if (szBuf == procL->findWindowName) {
					ProcessList::getInstance()->setProcessWindowHandle(hwnd);
					return FALSE;
				}
			}
			else {
				ProcessList::getInstance()->setProcessWindowHandle(hwnd);
				return FALSE;
			}
			

			
			// 
		}
	}
	return TRUE;
}


void ProcessList::setProcessWindowHandle(HWND hwnd) {
	processHWND = hwnd;
}

HWND ProcessList::getProcessWindowHandle(DWORD processId, string windowName) {
	if (processId == NULL) {
		processId = activeProcessId;
	}
	
	processHWND = NULL;
	findWindowName = windowName;
	// NuLogger::getInstance()->log("WindowList -------- %i", processId);

	EnumWindows(ProcessList::EnumWindowsProcMy, processId);
	return processHWND;
}


string ProcessList::getActiveProcessName() {
	checkProcessChanged();
	return getNameByProcessId(activeProcessId);
}

bool ProcessList::checkProcessChanged() {
	DWORD newPid = determineActiveProcessId();
	// NuLogger::getInstance()->log("checkProcessChanged to %i", newPid);
	if (activeProcessId != newPid) {
		activeProcessId = newPid;
		// NuLogger::getInstance()->log("ActiveProcess Changed to %i", activeProcessId);
		return true;
	}
	return false;
}

DWORD ProcessList::determineActiveProcessId() {
	DWORD threadid = 0;
	HWND curWin = GetForegroundWindow();
	
	// DWORD procid;
	 GetWindowThreadProcessId(curWin, &threadid);
	// NuLogger::getInstance()->log("threadid %i", procid);
	return threadid;
}


string ProcessList::getNameByProcessId(DWORD pid) {
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			// NuLogger::getInstance()->log("Process: '%s' found!", ws2s(process.szExeFile).c_str());
			if (process.th32ProcessID == pid) {
				return ws2s(process.szExeFile).c_str();
			}
		} while (Process32Next(snapshot, &process));
	}

	return "";
}

void ProcessList::makeSnapshot() {
	if (snapshot) {
		CloseHandle(snapshot);
	}
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

}


DWORD ProcessList::getPIDofProcess(string processName) {

	std::wstring wsProcessName = s2ws(processName);

	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			if (process.szExeFile == wsProcessName) {
				return process.th32ProcessID;
			}

		} while (Process32Next(snapshot, &process));
	}

	return NULL;
}


string ProcessList::getList(string prefix, DWORD curProcessId) {
	std::wstring ret = L"";
	std::wstring wsPrefix = s2ws(prefix);

	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	if (Process32First(snapshot, &process))
	{
		do {
			if (process.th32ProcessID == curProcessId) {
				ret += wsPrefix + process.szExeFile + L" << OWN PROCESS\n";
			}
			else {
				ret += wsPrefix + process.szExeFile + L"\n";
			}
			
		} 
		while (Process32Next(snapshot, &process));
	}

	return ws2s(ret);
}

bool ProcessList::ProcessExists(string processName, DWORD curProcessId) {
	
	std::wstring wsProcessName = s2ws(processName);

	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			// NuLogger::getInstance()->log("Process: '%s' found!", ws2s(process.szExeFile).c_str());
			if (process.szExeFile == wsProcessName && (curProcessId == 0 || process.th32ProcessID != curProcessId)) {
				return true;
			}
		} while (Process32Next(snapshot, &process));
	}

	

	return false;
}
