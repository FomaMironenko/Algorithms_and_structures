// linear_containers.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>

using namespace std;

template <typename T>
struct node
{
	node() : value(new T(0)), next(0)
	{}
	node(T data = 0) : next(0)
	{
		value = new T(data);
	}

	~node()
	{
		delete value;
	}

	T*		 value;
	node<T>* next;
};


template <typename T>
struct Stack
{
	Stack()
	{
		first = 0;
	}
	Stack(T data)
	{
		first = new node<T>(data);
	}

	void push(T data)
	{
		node<T>* tmp = new node<T>(data);
		tmp->next = first;
		first = tmp;
	}

	T pop()
	{
		if (first)
		{
			T val = *(first->value);
			node<T>* tmp = first->next;
			delete first;
			first = tmp;
			return val;
		}
		else return NULL;
	}

	bool isEmpty()
	{
		return !(bool)first;
	}

	~Stack()
	{
		while (first)
			this->pop();
	}

	node<T>* first;
};


int main()
{
	Stack<char> a('a');
	a.push('b');
	a.push('c');
	cout << a.pop() << " ";
	cout << a.pop() << " ";
	cout << a.pop() << " ";
	cout << a.pop() << " ";



	return EXIT_SUCCESS;
}