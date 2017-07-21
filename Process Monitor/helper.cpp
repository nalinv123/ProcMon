#include"Header.h"

/*
	member functions definations of class ThreadInfo
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ThreadInfo
// Author	:	Nalin
// Input	:	DWORD no	:	It accepts process ID
// Output	:	Nothing
// Description:	It is a constructor which creates snapshot of thread
///////////////////////////////////////////////////////////////////////////////////////////////////

ThreadInfo::ThreadInfo(DWORD no)
{
	PID = no;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, PID);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		cout << "\n Unable to create snapshot of current thread\n" << endl;
		return;
	}
	te32.dwSize = sizeof(THREADENTRY32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ThreadsDisplay
// Author	:	Nalin
// Input	:	Nothing
// Output	:	BOOL
//				returns TRUE on success
//				returns FALSE on failure
// Description:	It is a member function which display information about thread
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ThreadInfo :: ThreadsDisplay()
{
	if (!Thread32First(hThreadSnap, &te32))
	{
		cout << "\nERROR : In getting first thread\n" << endl;
		CloseHandle(hThreadSnap);
		return FALSE;
	}

	cout << "\nThread created by this process\n\n";
	do
	{
		if (te32.th32OwnerProcessID == PID)
		{
			cout << "\tThread ID : " << te32.th32ThreadID << endl;
		}
	} while (Thread32Next(hThreadSnap, &te32));
	
	CloseHandle(hThreadSnap);
	return FALSE;
}

/*
	member functions definations of class DLLInfo
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	DLLInfo
// Author	:	Nalin
// Input	:	DWORD no	:	It accepts process ID
// Output	:	Nothing
// Description:	It is a constructor which creates snapshot of dll files
///////////////////////////////////////////////////////////////////////////////////////////////////

DLLInfo::DLLInfo(DWORD no)
{
	PID = no;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		cout << "\nUnable to create snapshot of module\n" << endl;
		return;
	}
	me32.dwSize = sizeof(MODULEENTRY32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	DependentDLLDisplay
// Author	:	Nalin
// Input	:	Nothing
// Output	:	BOOL
//				returns TRUE on success
//				returns FALSE on failure
// Description:	It is a member function which display information about dll files
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DLLInfo :: DependentDLLDisplay()
{
	char arr[200];

	if (!Module32First(hModuleSnap, &me32))
	{
		cout << "\nERROR : In getting first module\n" << endl;
		CloseHandle(hModuleSnap);
		return FALSE;
	}

	cout << "\nDLL attached to this process\n";
	do
	{
		wcstombs_s(NULL, arr, me32.szModule, 200);
		cout << "\n\tDLL Name : " << arr;
	} while (Module32Next(hModuleSnap, &me32));
	
	CloseHandle(hModuleSnap);
	return TRUE;
}

/*
	member function definations of class ProcessInfo
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ProcessInfo
// Author	:	Nalin
// Input	:	Nothing
// Output	:	Nothing
// Description:	It is a constructor which creates snapshot of running process
///////////////////////////////////////////////////////////////////////////////////////////////////

ProcessInfo::ProcessInfo()
{
	ptobj = NULL;
	pdobj = NULL;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << "\n Unable to create snapshot of running process\n" << endl;
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ProcessDisplay
// Author	:	Nalin
// Input	:	char *option	:	It accepts options as which information of process should display
// Output	:	BOOL
//				returns TRUE on success
//				returns FALSE on failure
// Description:	It is a member function which display information of running process
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInfo::ProcessDisplay(char* option)
{
	char arr[200];
	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << "\nERROR : In finding first process\n" << endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		cout<<"\n------------------------------------------------------------------------------------\n";
		wcstombs_s(NULL, arr, pe32.szExeFile, 200);
		cout << "\nProcess Name\t:\t" << arr << "\nProcess ID\t:\t" << pe32.th32ProcessID << "\nParent Process ID:\t" << pe32.th32ParentProcessID << "\nCount Usage\t:\t" << pe32.cntUsage << "\nSize of process:\t" << pe32.dwSize << "\nNo of threads\t:\t" << pe32.cntThreads << endl;
		if ((_stricmp(option, "-a") == 0) || (_stricmp(option, "-d") == 0) || (_stricmp(option, "-t") == 0))
		{
			if ((_stricmp(option, "-t") == 0) || (_stricmp(option, "-a") == 0))
			{
				ptobj = new ThreadInfo(pe32.th32ProcessID);
				ptobj->ThreadsDisplay();
				delete ptobj;
			}
			if ((_stricmp(option, "-d") == 0) || (_stricmp(option, "-a") == 0))
			{
				pdobj = new DLLInfo(pe32.th32ProcessID);
				pdobj->DependentDLLDisplay();
				delete pdobj;
			}
		}

		cout<<"\n------------------------------------------------------------------------------------\n";
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ProcessLog
// Author	:	Nalin
// Input	:	Nothing
// Output	:	BOOL
//				returns TRUE after creating log
//				returns FALSE on failure
// Description:	It is a member function which creates log of running process
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInfo::ProcessLog()
{
	char* month[] = { "JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC" };
	char FileName[50], arr[200];
	SYSTEMTIME lt;
	LOGFILE lobj;
	FILE *fp;

	GetLocalTime(&lt);

	sprintf_s(FileName, "E://ProcMonLog %02d_%02d_%02d %s.txt", lt.wHour, lt.wMinute, lt.wDay, month[lt.wMonth - 1]);

	fp = fopen(FileName, "wb");
	if (fp == NULL)
	{
		cout << "\nUnable to create log file" << endl;
		return FALSE;
	}

	cout << "\nLog file successfully gets created as : " << FileName << endl;
	cout << "\nTime of log file creation -> " << lt.wHour << ":" << lt.wMinute << ":" << lt.wDay << "th" << month[lt.wMonth - 1] << endl;

	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << "\nERROR : In getting first process" << endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL, arr, 200, pe32.szExeFile, 200);
		strcpy(lobj.ProcessName, arr);
		lobj.pid = pe32.th32ProcessID;
		lobj.ppid = pe32.th32ParentProcessID;
		lobj.thread_cnt = pe32.cntThreads;
		fwrite(&lobj, sizeof(lobj), 1, fp);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	fclose(fp);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ReadLog
// Author	:	Nalin
// Input	:	DWORD hr	:	It accepts at which hour log created
//				DWORD min	:	It accepts at which minute log created
//				DWORD date	:	It accepts at which date log created
//				DWORD month	:	It accepts at which month log created
// Output	:	BOOL
//				returns TRUE after reading log
//				returns FALSE on failure
// Description:	It is a member function which reads log of process
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInfo::ReadLog(DWORD hr, DWORD min, DWORD date, DWORD month)
{
	char FileName[50];
	char* montharr[] = { "JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC" };
	FILE *fp;
	LOGFILE lobj;
	int ret;

	sprintf_s(FileName, "E://ProcMonLog %02d_%02d_%02d %s.txt", hr, min, date, montharr[month - 1]);

	fp = fopen(FileName, "rb");
	if (fp == NULL)
	{
		cout << "\nUnable to open log file named as " << FileName << endl;
		return FALSE;
	}

	while ((ret = fread(&lobj, 1, sizeof(lobj), fp)) != 0)
	{
		cout << "\n-----------------------------------------------------------" << endl;
		cout << "\nProcess Name : " << lobj.ProcessName << endl;
		cout << "\nProcess ID : " << lobj.pid << endl;
		cout << "\nProcess Parent ID : " << lobj.ppid << endl;
		cout << "\nNo of Threads : " << lobj.thread_cnt << endl;
		cout << "\n-----------------------------------------------------------" << endl;
	}

	fclose(fp);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	ProcessSearch
// Author	:	Nalin
// Input	:	char *name	:	It accepts address of process name
// Output	:	BOOL
//				returns TRUE after search in successful
//				returns FALSE on failure
// Description:	It is a member function which search running process
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInfo::ProcessSearch(char *name)
{
	char arr[200];
	BOOL Flag = FALSE;
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL, arr, pe32.szExeFile, 200);
		if (_stricmp(arr,name) == 0)
		{
			cout << endl << "PROCESS NAME : " << arr;
			cout << endl << "PID : " << pe32.th32ProcessID;
			cout << endl << "Parent PID : " << pe32.th32ParentProcessID;
			cout << endl << "No of Thread : " << pe32.cntThreads<<endl;
			Flag = TRUE;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return Flag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	KillProcess
// Author	:	Nalin
// Input	:	char *name	:	It accepts address of process name
// Output	:	BOOL
//				returns TRUE after killing process
//				returns FALSE on failure
// Description:	It is a member function which kills running process
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInfo::KillProcess(char *name)
{
	char arr[200];
	DWORD PID = -1;
	HANDLE hProcess;
	BOOL bRet;

	if (!Process32First(hProcessSnap, &pe32))
	{
		cout << "\n ERROR : In getting first process" << endl;
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	do
	{
		wcstombs_s(NULL, arr,200, pe32.szExeFile, 200);
		if (strcmp(arr, name) == 0)
		{
			PID = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	if (PID == -1)
	{
		cout << "\nERROR : There is no such process" << endl;
		return FALSE;
	}

	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
	if (hProcess == NULL)
	{
		cout << "\nERROR : There is no access to terminate" << endl;
		return FALSE;
	}

	bRet = TerminateProcess(hProcess, 0);
	if (bRet == FALSE)
	{
		cout << "\nUnable to terminate process" << endl;
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	HardwareInfo
// Author	:	Nalin
// Input	:	Nothing
// Output	:	BOOL
//				returns TRUE on sucess
//				returns FALSE on failure
// Description:	It is a function which display information about our system
///////////////////////////////////////////////////////////////////////////////////////////////////

BOOL HardwareInfo()
{
	SYSTEM_INFO siSysInfo;

	GetSystemInfo(&siSysInfo);

	cout << "\nOEM ID : " << siSysInfo.dwOemId;
	cout << "\nNumber of processor : " << siSysInfo.dwNumberOfProcessors;
	cout << "\nPage size : " << siSysInfo.dwPageSize;
	cout << "\nProcessor type : " << siSysInfo.dwProcessorType;
	cout << "\nMinimum application address : " << siSysInfo.lpMinimumApplicationAddress;
	cout << "\nMaximum application address : " << siSysInfo.lpMaximumApplicationAddress;
	cout << "\nActive processor mask : " << siSysInfo.dwActiveProcessorMask << endl;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Name		:	DisplayHelp
// Author	:	Nalin
// Input	:	Nothing
// Output	:	Nothing
// Description:	It is a function which display information about project
///////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
	cout << "\n Information about this tool" << endl;
	cout << "\n ps	:	shows all the information about running process" << endl;
	cout << "\n ps -t	:	shows information of running process with the thread information" << endl;
	cout << "\n ps -d	:	shows information of running process with attached DLL to the process" << endl;
	cout << "\n search	:	searches the running process" << endl;
	cout << "\n\t\tTo use this command follow below syntax as\n\t\tsearch process_name" << endl;
	cout << "\t\texample of process_name : notepad.exe" << endl;
	cout << "\n log	:	creates the log of all running process in E drive" << endl;
	cout << "\n\t\tTo use this command follow below syntax as\n\t\tlog" << endl;
	cout << "\n readlog:	read log created earlier by the tool" << endl;
	cout << "\n\t\tTo use this command follow below syntax as\n\t\treadlog" << endl;
	cout << "\n kill	:	kills the running process" << endl;
	cout << "\n\t\tTo use this command follow below syntax as\n\t\tkill process_name" << endl;
	cout << "\t\texample of process_name : notepad.exe" << endl;
	cout << "\n clear	:	To clear the screen" << endl;
}