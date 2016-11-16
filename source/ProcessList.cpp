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


HWND ProcessList::getWindowsByProcessName(string processName, string windowName, string className) {
	windowList = {};
	// NuLogger::getInstance()->log("-----------------------------------------------------");
	// NuLogger::getInstance()->log("getWindowsByProcessName");
	// NuLogger::getInstance()->log("-----------------------------------------------------");
	
	vector<DWORD> pidList = getPIDsofProcess(processName);
	for (vector<DWORD>::iterator it = pidList.begin(); it != pidList.end(); ++it) {
		addMainWindowByProcessIdToWindowList(*it);
	}

	vector<HWND> mainWindows = windowList;
	windowList = {};

	for (vector<HWND>::iterator itHWND = mainWindows.begin(); itHWND != mainWindows.end(); ++itHWND) {
		addChildWindowsByMainWindowToWindowList(*itHWND);
	}

	vector<HWND> childWindows = windowList;
	windowList = {};

	vector<HWND> allWindows = mainWindows;
	allWindows.insert(allWindows.end(), childWindows.begin(), childWindows.end());

	
	vector<HWND> filteredWindows = {};
	for (vector<HWND>::iterator cHWND = allWindows.begin(); cHWND != allWindows.end(); ++cHWND) {
		
		if (IsWindow(*cHWND) && IsWindowVisible(*cHWND)) {
			// NuLogger::getInstance()->log(" - Handle %i", *cHWND);
			
			CHAR windowNameBuf[255];
			GetWindowTextA(*cHWND, windowNameBuf, 255);
			// NuLogger::getInstance()->log("   |- Window: \"%s\"", windowNameBuf);

			CHAR classNameBuf[255];
			GetClassNameA(*cHWND, classNameBuf, 255);
			// NuLogger::getInstance()->log("   |- ClassName: \"%s\"", classNameBuf);
						
			if ((windowNameBuf == windowName || windowName == "") && 
				(classNameBuf == className || className == "")) {
				filteredWindows.push_back(*cHWND);
				// return *cHWND;
			}

		}
		else {
		//	NuLogger::getInstance()->log("\\ no window");
		}
		
	}
	
	// NuLogger::getInstance()->log("-----------------------------------------------------");
	
	if (filteredWindows.size() > 0) {
		return filteredWindows[0];
	}
	else {
		return NULL;
	}
	
}



void ProcessList::addChildWindowsByMainWindowToWindowList(HWND mainWindow) {
	EnumChildWindows(mainWindow, ProcessList::HelperEnumChildWindows, 0);
}

BOOL CALLBACK ProcessList::HelperEnumChildWindows(HWND hwnd, LPARAM lParam)
{
	ProcessList::getInstance()->addToWindowList(hwnd);
	return TRUE;
}

void ProcessList::addMainWindowByProcessIdToWindowList(DWORD processId, string windowName) {
	EnumWindows(ProcessList::HelperEnumMainWindow, processId);
}

BOOL CALLBACK ProcessList::HelperEnumMainWindow(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		ProcessList::getInstance()->addToWindowList(hwnd);
	}
	return TRUE;
}

void ProcessList::addToWindowList(HWND hwnd) {
	windowList.push_back(hwnd);
}

void ProcessList::setProcessWindowHandle(HWND hwnd) {
	processHWND = hwnd;
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
	for (map<DWORD, wstring>::iterator it = cachedProcessList.begin(); it != cachedProcessList.end(); it++) {
		if (it->first == pid) {
			return ws2s(it->second).c_str();
		}
	}
	return "";
}


void ProcessList::makeSnapshot() {
	if (snapshot) {
		CloseHandle(snapshot);
	}
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	if (Process32First(snapshot, &process))
	{
		do {
			cachedProcessList[process.th32ProcessID] = process.szExeFile;
		} while (Process32Next(snapshot, &process));
	}
}


DWORD ProcessList::getPIDofProcess(string processName) {
	vector<DWORD> pidList = getPIDsofProcess(processName);
	if (pidList.size() > 0) {
		return pidList[0];
	}
	return 0;
}

vector<DWORD> ProcessList::getPIDsofProcess(string processName) {

	vector<DWORD> pidList = {};

	std::wstring wsProcessName = s2ws(processName);

	for (map<DWORD, wstring>::iterator it = cachedProcessList.begin(); it != cachedProcessList.end(); it++) {
		if (it->second == wsProcessName) {
			pidList.push_back(it->first);
		}
	}
	
	return pidList;
}


string ProcessList::getList(string prefix, DWORD curProcessId) {
	std::wstring ret = L"";
	std::wstring wsPrefix = s2ws(prefix);

	for (map<DWORD, wstring>::iterator it = cachedProcessList.begin(); it != cachedProcessList.end(); it++) {
		if (it->first == curProcessId) {
			ret += wsPrefix + it->second + L" << OWN PROCESS\n";
		}
		else {
			ret += wsPrefix + it->second + L"\n";
		}
	}

	return ws2s(ret);
}


bool ProcessList::ProcessExists(string processName, DWORD curProcessId) {
	
	std::wstring wsProcessName = s2ws(processName);

	for (map<DWORD, wstring>::iterator it = cachedProcessList.begin(); it != cachedProcessList.end(); it++) {
		if (it->second == wsProcessName && (curProcessId == 0 || it->first != curProcessId)) {
			return true;
		}
	}

	return false;
}
