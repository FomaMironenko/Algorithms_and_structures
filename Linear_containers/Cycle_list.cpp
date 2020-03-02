// Cycle_list.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>


using namespace std;

template <typename T>
struct node
{
	node(T data = 0)
	{
		next = 0;
		value = new T(data);
	}

	~node()
	{
		delete value;
	}

	T*		 value;
	node<T>* next;
};


//LIST//////////////////////////

template <typename T>
struct Cycle
{
	Cycle()
	{
		first = 0;
	}
	Cycle(T data)
	{
		first = new node<T>(data);
		first->next = first;
	}

	void push(T data)
	{
		node<T>* New = new node<T>(data);
		if (first == 0)
		{
			first = New;
			first->next = first;
			return;
		}
		New->next = first->next;
		first->next = New;
		first = New;
	}

	void del()
	{
		node<T>* cur = first;
		while (cur->next != first)
		{
			cur = cur->next;
		}
		cur->next = first->next;
		node<T>* tmp = first;
		first = cur;
		delete tmp;
	}

	bool IsEmpty()
	{
		if (first == 0)
			return true;
		return false;
	}

	node<T>* first;
};

int main()
{
	Cycle<int> Flavius(1);
	for (int i = 2; i <= 40; i++)
		Flavius.push(i);
	Flavius.first = Flavius.first->next;
	node<int>* tmp = Flavius.first;
	for (int i = 0; i < 40; i++)
	{
		if (i % 10 == 0)
			cout << endl;
		printf("%-2d ", *Flavius.first->value);
		Flavius.del();
		if(i != 39)
			for (int j = 0; j < 5; j++)
				Flavius.first = Flavius.first->next;
	}
    return EXIT_SUCCESS; 
}