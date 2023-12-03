#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread> // thread
#include <atomic> // atom
#include <mutex> // lock
#include <Windows.h>


struct Node
{
	int data;
	Node* next;
};

atomic<Node*> _head = nullptr;

void push(int data)
{
	Node* new_Node = new Node();
	new_Node->data = data;
	new_Node->next = _head;

	// cas + while을 통해 lock free를 달성.
	// 명시적으로 lock을 하지 않고 lock을 구현한 모습
	while (!_head.compare_exchange_strong(new_Node->next, new_Node));

	// _head = new_Node;
}

int pop()
{
	Node* temp = _head;

	// if temp == nulptr인 경우를 제외하기 위해 temp && 추가
	while (temp && !_head.compare_exchange_strong(temp, temp->next));

	// temp == nullptr이라면 false
	if (temp == nullptr)
		return false;

	int value = temp->data;

	// delete가 충돌이 일어날 수 있다.
	// why? 는 몰루
	// C#, Java는 gc가 있어서 delete 굳이 안해줘도 되지만, C++는 없기 때문에 해줘야 한다.
	// delete temp;

	return value;
}

void push_inf()
{
	while (true) 
	{
		int32 value = rand() % 100;
		push(value);
	}
}

void push_inf_1()
{
	while (true)
	{
		push(100);
	}
}

void push_inf_2()
{
	while (true)
	{
		push(200);
	}
}

void pop_inf()
{
	while (true)
	{
		cout << pop() << endl;
	}
}


int main()
{
	thread t1(push_inf_1);
	thread t2(pop_inf);

	t1.join();
	t2.join();

}
