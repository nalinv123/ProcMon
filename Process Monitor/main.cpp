#include"header.h"

int main()
{
	BOOL bRet;
	ProcessInfo *ppobj = NULL;
	char *ptr = NULL;
	char command[4][80], str[80];
	int count, min, date, month, hr;

	while (1)
	{
		fflush(stdin);
		strcpy_s(str, "");

		cout << "\nProcMon : >";
		fgets(str, 80, stdin);

		count = sscanf(str, "%s%s%s%s", command[0], command[1], command[2], command[3]);

		if (count == 1)
		{
			if (_stricmp(command[0], "ps") == 0)
			{
				ppobj = new ProcessInfo();
				bRet = ppobj->ProcessDisplay("-a");
				if (bRet == FALSE)
				{
					cout << "\nERROR : Unable to display process information\n" << endl;
				}
				delete ppobj;
			}
			else if (_stricmp(command[0], "log") == 0)
			{
				ppobj = new ProcessInfo();
				bRet = ppobj->ProcessLog();
				delete ppobj;
			}
			else if (_stricmp(command[0],"sysinfo") == 0)
			{
				bRet = HardwareInfo();
				if (bRet == FALSE)
				{
					cout << "\nERROR : Unable to get hardware information" << endl;
				}
			}
			else if (_stricmp(command[0],"readlog") == 0)
			{
				ppobj = new ProcessInfo();
				
				cout << "\nEnter log file details : " << endl;
				cout << "\nHour : ";
				cin >> hr;
				cout << "\nMinute : ";
				cin >> min;
				cout << "\nDate : ";
				cin >> date;
				cout << "\nMonth : ";
				cin >> month;
				
				ppobj->ReadLog(hr, min, date, month);
				delete ppobj;
			}
			else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
			}
			else if (_stricmp(command[0], "help") == 0)
			{
				DisplayHelp();
			}
			else if (_stricmp(command[0], "exit") == 0)
			{
				cout << "\nTerminating ProcMon \n" << endl;
				break;
			}
			else
			{
				cout << "\nERROR : Command not found !!!" << endl;
				cout << "\nUse 'help' command to get detailed information about the tool" << endl;
			}
		}
		else if (count == 2)
		{
			if (_stricmp(command[0], "ps") == 0)
			{
				ppobj = new ProcessInfo();
				bRet = ppobj->ProcessDisplay(command[1]);
				if (bRet == FALSE)
				{
					cout << "\nERROR : Unable to display process information\n" << endl;
				}
				delete ppobj;
			}
			else if (_stricmp(command[0], "search") == 0)
			{
				ppobj = new ProcessInfo();
				bRet = ppobj->ProcessSearch(command[1]);
				if (bRet == FALSE)
				{
					cout << "\nERROR : There is no such process\n" << endl;
				}
				delete ppobj;
			}
			else if (_stricmp(command[0], "kill") == 0)
			{
				ppobj = new ProcessInfo();
				bRet = ppobj->KillProcess(command[1]);
				if (bRet == TRUE)
				{
					cout << "\nTerminated successfully" << endl;
				}
				delete ppobj;
			}
			else
			{
				cout << "\nERROR : Command not found !!!" << endl;
				cout << "\nUse 'help' command to get detailed information about the tool" << endl;
			}
		}
		else
		{
			cout << "\nERROR : Command not found !!!" << endl;
			cout << "\nUse 'help' command to get detailed information about the tool" << endl;
		}
	}

	return 0;
}