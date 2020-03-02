#include "pch.h"
#include <iostream>
#include <stack>
using namespace std;

template <typename T>
struct queue
{
	queue()
	{	}
	queue(T value)
	{	
		tail.push(value);
	}

	void push(T value)
	{
		tail.push(value);
	}

	T pop()
	{
		if (head.empty())
		{
			if (tail.empty())
				return NULL;
			while (!tail.empty())
			{
				head.push(tail.top());
				tail.pop();
			}
		}
		T tmp = head.top();
		head.pop();
		return tmp;
	}

	bool empty()
	{
		return tail.empty() && head.mepty();
	}

	stack<T> tail;
	stack<T> head;
};


int main()
{
    return EXIT_SUCCESS; 
}