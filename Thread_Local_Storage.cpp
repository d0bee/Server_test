쓰레드의 스택/tls(thread local storage) 메모리의 차이점
스택 메모리 :
함수를 위한 저장소
tls :
메모리 접근 횟수를 줄이기 위한 저장소
나만의 전역 메모리

#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread> // thread
#include <atomic> // atom
#include <mutex> // lock
#include <windows.h>
#include <future>

//__declspec(thread) int32 value;
thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << "Hi! I AM tHREAD" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0;i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}

	for (thread& t : threads)
		t.join();
}
