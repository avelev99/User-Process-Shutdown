#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

// Enumerate all the top-level and child windows on the desktop
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    // Add the handle of the current window to the vector of window handles
    vector<HWND>* pvhWnds = reinterpret_cast<vector<HWND> *>(lParam);
    pvhWnds->push_back(hwnd);

    // Enumerate the child windows of the current window
    EnumChildWindows(hwnd, EnumWindowsProc, lParam);

    return TRUE;
}

int main()
{
    // Enumerate all the top-level and child windows on the desktop
    vector<HWND> vhWnds;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&vhWnds));

    // Get the process ID of the process that owns each window
    vector<DWORD> vdwPIDs;
    for (size_t i = 0; i < vhWnds.size(); i++)
    {
        DWORD dwPID;
        GetWindowThreadProcessId(vhWnds[i], &dwPID);
        vdwPIDs.push_back(dwPID);
    }

    // Open a handle to each process and terminate it
    for (size_t i = 0; i < vdwPIDs.size(); i++)
    {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, vdwPIDs[i]);
        if (hProcess != NULL)
        {
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }
    }

    return 0;
}