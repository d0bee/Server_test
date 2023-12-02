#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread> // thread
#include <atomic> // atom
#include <mutex> // lock
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);

		// Signal을 파란불로 설정
		::SetEvent(handle);
		// ex, 100ms가 아닌 하루마다 실행되는 작업이 있다는 가정하에
		this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	// Consumer를 담당하는 쓰레드는 대기 시간, while 연산 시간, lock을 통한 데이터 점유 낭비가 굉장히 심함.
	while (true)
	{
		// Signal을 빨간불로 설정
		// 무의미한 실행을 예방
		::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// handle == 커널 오브젝트
	// Usage Count
	// Signal (파란불) / Non-Signal (빨간불) << bool
	// Auto (자동) / Manual (수동) << bool
	// 따라서 EVENT, 감시자를 생성
	// EVENT로 인해 CPU 점유율이 줄어든다.
	handle = ::CreateEvent(NULL/*보안 속성*/, FALSE/*Manual Reset*/, FALSE/*binitialstate*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}
