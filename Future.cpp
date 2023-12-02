#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread> // thread
#include <atomic> // atom
#include <mutex> // lock
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기(synchronous) 실행
	// int64 sum = Calculate();
	// cout << sum << endl;

	// std::future, 비동기 실행, * 멀티쓰레드와는 또 다르다??
	{
		// 1) DEFERRED : 지연해서 실행하세요.
		// 2) ASYNC : 별도의 쓰레드를 만들어서 실행하세요.
		// 3) DEFERRED | ASYNC : 둘 중 알아서 골라주세요.
		
		// 언젠가 미래에 결과물을 뱉어줄거야! >> Async, 비동기
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO


		// TODO ~~ 이후에 결과물이 이제서야 필요하다?
		int64 sum = future.get();
	}

	// std::promise
	{
		// 미래(std::future)에 결과물을 반환해줄거라 약속, 계약서
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론)
	// mutex, condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는 방법?
	// 1) Async
	// 원하는 함수를 비동기적으로 실행
	// 2) Promise
	// 결과물을 Promise를 통해 future로 받아줌
	// 3) Packaged_task
	// 원하는 함수의 실행 결과를 Packaged_task를 통해 future로 받아줌.

	// 특히나, 한 번 발생하는 이벤트에 유용하다.
	// 닭 잡는데 소 잡는 칼 사용할 필요 없다.
}
