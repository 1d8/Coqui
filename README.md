# Coqui
## DISCLAIMER: This project was made for research purposes. Anything you do with this code is on you and is not my responsibility.

This malware is designed to activate when a user visits a banking website, the malware will check the window title against a set of hardcoded values to see if they are on certain sites. This set of hardcoded values can be expanded by simply adding them into the _banktitles_ variable:

![](/imgs/img1.png)

If a banking title is matched the keylogger is started but if it is not matched the malware will simply keep running until the user visits a website that matches one in the hardcoded list.

Once the keylogger is started, the malware will save a file called db.txt in the %TMP% directory.

The dropper associated with this malware is simple: it checks all the running windows to see if they have the words "Process" or "Windows Task Manager" in them as these usually indicate the file is being analyzed (EX: Process Hacker, Process Monitor, etc), if any windows have this in their title, it doesn't continue running. 
#### NOTE: This hardcoded list of processes that is checked before continuing execution can be expanded by changing the _xprocesses_ variable:

![](/imgs/img2.png)

Otherwise, it checks if the victim is already infected by searching the %TMP% directory for a list of files. If the victim is already infected, the dropper will send off the collected keystrokes to a remote server which is specified in the 2nd parameter of the _Pigeon_ function via GET request. If the victim isn't infected, it copies itself to the %TMP% directory with the _01.exe_ name & creates a scheduled Task to run itself every 12 days at 12:00 noon. Finally, it downloads the keylogger & names it _ursakta.exe_.

## Before Using

Change the IP address of the server (2nd parameter to the _Pigeon_) function as well as the URL for the main keylogger file (1st parameter to the _Pigeon_ function). The pigeon function is called in _main_:

![](/imgs/img6.png)

## Compiling

Cross-compile from Linux to Windows using mingw

64-bit (for the dropper):
`x86_64-w64-mingw32-gcc input.c -o output.exe -lurlmon -lwininet`

32-bit (for the dropper):
`i686-w64-mingw32-gcc input.c -o output.exe -lurlmon -lwininet`

64-bit (for keylogger):
`x86_64-w64-mingw32-gcc input.c -o output.exe`

32-bit (for keylogger):
`i686-w64-mingw32-gcc input.c -o output.exe`

## TODO:
1. Add a feature that constantly checks for processes that involve system imaging (such as FTK) & if it finds it, kill all running processes related to the malware & remove itself.
2. Add a feature to disable keylogger once banking windows are closed out of (added killlogger which does this).
