#include "pch.h"
#include <iostream>

using namespace std;


template<typename T>
struct Queue
{
	Queue(int n): data(new T[n]), N(n), len(0), first(0) {}

	void push(T element)
	{
		if (len++ == 0)
		{
			data[first] = element;
			return;
		}
		data[(first + len - 1) % N] = element;
	}

	T pop()
	{
		if(len > 0)
			len--;
		T tmp = data[first];
		first = (first + 1) % N;
		return tmp;
	}

	void print()
	{
		for (int i = first; i < first + len; i++)
		{
			std::cout << data[i%N] << " ";
		}
		std::cout << endl;
	}

	int size()
	{
		return len;
	}

private:
	T* data;
	int N;
	int first;
	int len;
};

int main()
{
	Queue<int> a(4);
	a.push(1);
	a.push(1);
	a.push(2); a.push(3);
	a.print();
	std::cout << a.pop() << " ";
	std::cout << a.pop() << " ";
	std::cout << a.pop() << " ";
    return EXIT_SUCCESS; 
}