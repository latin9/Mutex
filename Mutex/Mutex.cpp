
// Mutex�� ����ȭ ������ ������ ���� �ƴ϶� Ŀ�� ��忡���� ������ ����ȭ ������ ������ ��?

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <process.h>
#include <WS2tcpip.h>


unsigned WINAPI ThreadIncrement(void* arg);
void ErrorHandling(const char* message);

char thread1[] = "A Thread";
char thread2[] = "B Thread";

int number = 0;

// Mutex ���� �� ���޵Ǵ� �ڵ��� �����ϱ� ���� ����
// ������鿡 ���ؼ� �����Ǿ�� �ϹǷ�(������ ����ȭ�� ���ؼ�) ���� ������ ����Ǿ� �ִ�.
// ���ø޸� �̿��� �޸𸮵��� �����̱� ����
HANDLE hMutex;

int main(int argc, char** argv)
{
	HANDLE hThread1, hThread2;
	DWORD dwThreadID1, dwThreadID2;

	// Mutex�� ����
	// ������ Mutex�� �����ڰ� ���� signaled ������ Ŀ�� ������Ʈ�̴�.
	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hMutex == NULL)
	{
		puts("���ؽ� ������Ʈ ���� ����");
		exit(1);
	}

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread1, 0, (unsigned*)&dwThreadID1);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread2, 0, (unsigned*)&dwThreadID2);

	if (hThread1 == 0 || hThread2 == 0)
	{
		puts("������ ���� ����");
		exit(1);
	}

	if (WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
		ErrorHandling("������ wait ����");

	if (WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED)
		ErrorHandling("������ wait ����");

	printf_s("���� number : %d \n", number);

	// Mutex ������Ʈ �Ҹ�
	CloseHandle(hMutex);

	return 0;
}

unsigned WINAPI ThreadIncrement(void* arg)
{
	int i;

	for (i = 0; i < 5; i++)
	{
		// Mutex�� ��´�
		WaitForSingleObject(hMutex, INFINITE);	// Mutex�� ��´�.
		// �Ʒ����� ReleaseMutex �������� �Ӱ� �����̴�
		// �����ǰ� �ִ� �ڿ��� number�� ���� �����ϰ� �ֱ� ����
		// Sleep�� �Ӱ� ������ ���Խ�Ų ������ ���� ������ �� ���� �����尡 ������ ���鼭 �����ϰ� �ϱ� ���ؼ� ��� ��. 
		Sleep(100);
		number++;
		// �Ʒ� �ٵ� �Ӱ迵���� ���Խ�Ų ������ �����͸� ��� �ϴ� ���������� �� ���� ���� �� �� �ֱ� �����̴�.
		printf_s("���� : %s, number : %d \n", (char*)arg, number);
		// Mutex�� ��ȯ
		ReleaseMutex(hMutex);
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}