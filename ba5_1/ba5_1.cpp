#include <Windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

int main()
{
	HANDLE mutex = CreateMutexA(NULL, FALSE, "Mutex");

	if (WaitForSingleObject(mutex, 0) == WAIT_OBJECT_0)
	{
		cout << "Is Running" << endl;
		_getch();
		ReleaseMutex(mutex);
	}
	else
	{
		cout << "Can't start program..." << endl;
		_getch();
	}

	CloseHandle(mutex);
	return 0;
}