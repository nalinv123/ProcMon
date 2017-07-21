using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>
#include<Windows.h>
#include<TlHelp32.h>

/*
	Name		:	LogFile
	Description	:	It is a structure which stores running process information which is used to create log
*/

typedef struct LogFile
{
	char ProcessName[100];				//stores running process name
	unsigned int pid;					//stores process ID
	unsigned int ppid;					//stores parent process ID
	unsigned int thread_cnt;			//stores no of threads created by the process 
}LOGFILE;

/*
	Name		:	ThreadInfo
	Description	:	It is a class which stores information about threads
*/

class ThreadInfo
{
private:
	DWORD PID;							//stores the owner process ID
	HANDLE hThreadSnap;					//handle to create snapshot of threads of PID
	THREADENTRY32 te32;					//object of predefined structure THREADENTRY32
public:
	ThreadInfo(DWORD);					//constructor of a class
	BOOL ThreadsDisplay();				//member function to display information about thread
};

/*
	Name		:	DLLInfo
	Description	:	It is a class which stores information about DLL files
*/

class DLLInfo
{
private:
	DWORD PID;							//stores the owner process ID
	HANDLE hModuleSnap;					//handle to create snapshot of DLL of PID
	MODULEENTRY32 me32;					//object of predefined structure MODULEENTERY32
public:
	DLLInfo(DWORD);						//constructor of a class
	BOOL DependentDLLDisplay();			//member function to display information about DLL files
};

/*
	Name		:	ProcessInfo
	Description	:	It is a class which stores information about running process
*/

class ProcessInfo
{
private:
	DWORD PID;							//stores process ID
	DLLInfo *pdobj;						//pointer of type DLLInfo class
	ThreadInfo *ptobj;					//pointer of type ThreadInfo class
	HANDLE hProcessSnap;				//handle to create snapshot of running process
	PROCESSENTRY32 pe32;				//object of predefined structure PROCESSENTRY32
public:
	ProcessInfo();						//constructor of a class
	BOOL ProcessDisplay(char *);		//member function to display information about running process
	BOOL ProcessLog();					//member function to create log of running process
	BOOL ReadLog(DWORD, DWORD, 
				DWORD, DWORD);			//member function to read log created earlier
	BOOL ProcessSearch(char *);			//member function to search running process
	BOOL KillProcess(char *);			//member function to kill running process
};

BOOL HardwareInfo();
void DisplayHelp();