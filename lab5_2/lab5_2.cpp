#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace std;

HANDLE semaphore;
DWORD WINAPI thread_function(LPVOID param);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);



int main()
{
    int max_handles;
    int max_threads;
    srand(time(NULL));

    cout << "Input max number of handles = ";
    cin >> max_handles;

    cout << "Input max number of threads = ";
    cin >> max_threads;

    HANDLE* threads = new HANDLE[max_threads];

    semaphore = CreateSemaphoreA(NULL, max_handles, max_handles, "Semaphore");

    if (semaphore == NULL)
        return 1;

    for (int i = 0; i < max_threads; i++)
    {
        int* params = new int[2];
        //number thread
        params[0] = i;
        //time
        params[1] = 1 + rand() % (10);
        threads[i] = CreateThread(NULL, 0, thread_function, (LPVOID)params, NULL, NULL);
    }
    WaitForMultipleObjects(max_threads, threads, TRUE, INFINITE);
    SetConsoleTextAttribute(hConsole, 15);
    return 0;
}


DWORD WINAPI thread_function(LPVOID param)
{
    int* params = (int*)param;
    clock_t start = clock();
    int thread_number = params[0];
    LPSTR str = new CHAR[128];

    DWORD result = WaitForSingleObject(semaphore, 500);
    while (result != WAIT_OBJECT_0)
    {
        
        result = WaitForSingleObject(semaphore, 1000);
        SetConsoleTextAttribute(hConsole, 15);
        printf("[%i Thread]  waiting for semaphore\n", thread_number);
    }
    SetConsoleTextAttribute(hConsole, 11);
    printf("[%i Thread] decrement semaphore. Going to sleep\n", thread_number);


    Sleep(params[1] * 1000);

    HANDLE file = CreateFileA("file.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        ReleaseSemaphore(semaphore, 1, NULL);
        return 0;
    }
    SetFilePointer(file, 0, NULL, FILE_END);

    clock_t finish = clock();
    float time_elapsed = (finish - start) / CLK_TCK;
    
    sprintf(str, "[%i Thread] made this in %f seconds\n\0", thread_number, time_elapsed);
    WriteFile(file, str, strlen(str), NULL, NULL);
    CloseHandle(file);
    SetConsoleTextAttribute(hConsole, 10);
    printf("[%i Thread] released semaphore.\n", thread_number);  
    ReleaseSemaphore(semaphore, 1, NULL);

    return 0;
}
