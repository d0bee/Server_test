#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread> // thread
#include <atomic> // atom
#include <mutex> // lock

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false;
		bool desired = true;

		// desired 값을 얻을 때까지, Lock 권한을 얻을 때까지 반복한다.
		// why? while == false, 즉 true가 될 때까지 Spin
		while (_locked.compare_exchange_strong(expected, desired) == false) 
		{
			// 매개변수 expected가 레퍼런스 참조이기 때문에 지속적으로 false를 명시해줘야함.
			expected = false;
		}
	}

	void unlock()
	{
		_locked.store(false);
	}
private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	// volatile, 최적화 하지 않고 a = 1, a = 2, a = 3와 같은 과정을 모두 진행하라는 것을 명시.
	volatile int32 a = 0;
	a = 1;
	a = 2;
	a = 3;
	a = 4;
	cout << a << endl;

	thread t1(Add);
	thread t2(Sub);
	
	t1.join();
	t2.join();

	cout << sum << endl;
}
