#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

#define ARRAY_MAX 100000000
CRITICAL_SECTION section;
int* array;

DWORD WINAPI ThreadFunctionSum(LPVOID use_critical_section);
DWORD WINAPI ThreadFunctionMaxAndMin(LPVOID use_critical_section);
DWORD WINAPI ThreadFunctionAvg(LPVOID use_critical_section);


int main()
{
	HANDLE* threads;
	clock_t start;
	float elapsed_time;
	srand(time(NULL));
	InitializeCriticalSection(&section);
	int percent10 = ceil((ARRAY_MAX / (float)10));
	printf("Create array");
	start = clock();
	int st = 0;
	array = new int[ARRAY_MAX];
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		if (i == st) {
			printf(".");
			st += percent10;
		}
		array[i] = 0 + rand() % (1001);
	}
	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("\nTime : %f.\nCompleted!\n\n", elapsed_time);
	printf("Start of calculations:\n\n");

	start = clock();
	threads = new HANDLE[3];
	threads[0] = CreateThread(NULL, 0, ThreadFunctionSum, (LPVOID)FALSE, NULL, NULL);
	threads[1] = CreateThread(NULL, 0, ThreadFunctionAvg, (LPVOID)FALSE, NULL, NULL);
	threads[2] = CreateThread(NULL, 0, ThreadFunctionMaxAndMin, (LPVOID)FALSE, NULL, NULL);

	WaitForMultipleObjects(3, threads, TRUE, INFINITE);

	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("Without critical section it took %f seconds\n\n", elapsed_time);

	start = clock();
	threads = new HANDLE[3];
	threads[0] = CreateThread(NULL, 0, ThreadFunctionSum, (LPVOID)TRUE, NULL, NULL);
	threads[1] = CreateThread(NULL, 0, ThreadFunctionAvg, (LPVOID)TRUE, NULL, NULL);
	threads[2] = CreateThread(NULL, 0, ThreadFunctionMaxAndMin, (LPVOID)TRUE, NULL, NULL);

	WaitForMultipleObjects(3, threads, TRUE, INFINITE);

	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("With critical section it took %f seconds\n\n", elapsed_time);

	DeleteCriticalSection(&section);

	return 0;
}


DWORD WINAPI ThreadFunctionSum(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section)) {}
	}
	float sum = 0;
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		sum += array[i];
	}
	printf("Sum : %f\n", sum);
	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}
	return 0;
}

DWORD WINAPI ThreadFunctionMaxAndMin(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section)) {}
	}

	int min = 0;
	int max = 0;
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		if (array[min] > array[i])
			min = i;
		else if (array[max] < array[i]) {
			max = i;
		}
	}
	printf("Min: array[%i] = %i\n", min, array[min]);
	printf("Max: array[%i] = %i\n", max, array[max]);

	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}
	return 0;
}

DWORD WINAPI ThreadFunctionAvg(LPVOID use_critical_section)
{
	if ((bool)use_critical_section)
	{
		while (!TryEnterCriticalSection(&section)) {}
	}

	float avg = 0;
	for (int i = 0; i < ARRAY_MAX; i++)
	{
		avg += array[i];
	}
	printf("Avg: %f\n", avg / ARRAY_MAX);

	if ((bool)use_critical_section)
	{
		LeaveCriticalSection(&section);
	}

	return 0;
}

