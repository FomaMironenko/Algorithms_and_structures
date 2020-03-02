// List.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
struct List
{
	List()
	{
		first = 0;
	}
	List(istream &st)
	{
		first = 0;
		T a;
		cin >> a;
		first = new node<T>(a);
		node<T>* cur = first;
		while (cin >> a)
		{
			node<T>* New = new node<T>(a);
			cur = (cur->next = New);
		}

		cin.clear();
	}
	List(T data)
	{
		first = new node<T>(data);
	}
	List(node<T>* frst) : first(frst) {};

//COPY LIST//////////////////////////////////////////
	List(List const& from): first(new node<T>)
	{
		*first->value = *from.first->value;
		node<T>* cur_f = from.first;
		node<T>* cur_t = first;
		while (cur_f = cur_f->next)
		{
			node<T>* New = new node<T>(*cur_f->value);
			cur_t = (cur_t->next = New);
		}
	}

	List & operator=(List & from)
	{
		if (this != &from)
		{
			this->~List();
			first = 0;
			if (!from.isEmpty())
			{
				first = new node<T>(*from.first->value);
				node<T>* cur_f = from.first;
				node<T>* cur_t = first;
				while (cur_f = cur_f->next)
				{
					node<T>* New = new node<T>(*cur_f->value);
					cur_t = (cur_t->next = New);
				}
				return *this;
			}
			return *this;
		}
	}
/////////////////////////////////////////////////////

	List operator +=(List const& left)
	{
		node<T>* cur = first;
		while (cur->next)
			cur = cur->next;
		List<T> New = left;
		cur->next = New.first;
		return *this;
	}

////////////////NEW ELEMENT//////////////////
	void push_back(T data)
	{
		node<T>* New = new node<T>(data);
		if (first == 0)
		{
			first = New;
			return;
		}
		node<T>* tmp = first;
		while (tmp->next != 0)
			tmp = tmp->next;
		tmp->next = New;
	}

	void push_front(T data)
	{
		node<T>* New = new node<T>(data);
		New->next = first;
		first = New;
	}

	void insert(node<T>* cur, T value)
	{
		node<T>* New = new node<T>(value);
		New->next = cur->next;
		cur->next = New;
	}

	void insert(T data, int j)
	{
		node<T>* New = new node<T>(data);
		node<T>* cur = first;
		if (j <= 0)
		{
			New->next = first;
			first = New;
		}
		int i = 0;
		while (cur->next)
		{
			if (i++ == j - 1)
				break;
			cur = cur->next;
		}
		New->next = cur->next;
		cur->next = New;
	}
/////////////////////////////////////////////

//LENGTH/////////////////////////
	bool isEmpty()
	{
		return !(bool)first;
	}

	int len()
	{
		int i = 1;
		if (!first)
			return 0;
		node<T>* tmp = first;
		while (tmp = tmp->next)
			i++;
		return i;
	}
/////////////////////////////////

	T& operator[](int j)
	{
		if (!first)
		{
			first = new node<T>(0);
			return *first->value;
		}
		node<T>* cur = first;
		if (j == 0)
			return *first->value;
		int i = 0;
		while (cur->next != 0)
			if (cur = cur->next, ++i == j)
				break;
		return *cur->value; //если индекс вне длины списка - возвращает последний элемент
	}

	void del(int j)
	{
		node<T>* cur = first;
		if (first == 0)
			return;
		int i = 0;
		if (j == 0)
		{
			node<T>* tmp = first;
			first = first->next;
			if(!tmp)
				delete tmp;
		}
		while (cur->next)
		{
			if (i++ == j - 1)
				break;
			cur = cur->next;
		}
		if (cur->next != 0)
		{
			node<T>* tmp = cur->next;
			cur->next = cur->next->next;
			delete tmp;
		}
	}

	void print()
	{
		node<T>* cur = first;
		while (cur)
		{
			cout << *cur->value << " ";
			cur = cur->next;
		}
		cout << "\n";
	}

	int count(bool(*f)(T))
	{
		node<T>* cur = first;
		int counter = 0;
		while (cur)
		{
			counter += int(f(*cur->value));
			cur = cur->next;
		}
		return counter;
	}

	void ord_concat(List const & other)
	{
		if (this->isEmpty())
		{
			*this += other;
			return;
		}

		node<T>* tmpF = this->first;
		node<T>* tmpO = other.first;
		if (tmpO && *tmpF->value > *tmpO->value)
		{
			push_front(*tmpO->value);
			tmpF = this->first;
			tmpO = tmpO->next;
		}

		while (tmpO != 0)
		{
			if (tmpF->next && *tmpO->value >= *tmpF->value && *tmpO->value < *tmpF->next->value)
			{
				insert(tmpF, *tmpO->value);
				tmpO = tmpO->next;
				tmpF = tmpF->next;
			}
			else if (tmpF->next)
				tmpF = tmpF->next;
			else
			{
				push_back(*tmpO->value);
				tmpO = tmpO->next;
			}
		}
	}

	////////SORT//////////////////////////////////////////
	List Merge(List other)
	{
		if (this->isEmpty())
		{
			first = other.first;;
			return *this;
		}

		if (*first->value > *other.first->value)
		{
			node<T>* tmp = first;
			first = other.first;
			other.first = tmp;
		}
		node<T>* tmpF = this->first;
		node<T>* tmpO = other.first;
		while (tmpO != 0)
		{
			if (tmpF->next && *tmpO->value >= *tmpF->value && *tmpO->value < *tmpF->next->value)
			{
				node<T>* tmp = tmpO;
				tmpO = tmpO->next;
				tmp->next = tmpF->next;
				tmpF->next = tmp;
				tmpF = tmpF->next;
			}
			else if (tmpF->next)
				tmpF = tmpF->next;
			else
			{
				tmpF->next = tmpO;
				break;
			}
		}
		return *this;
	}

	List divide()
	{
		if (first->next == 0)
			return List();
		node<T>* mid = this->first;
		node<T>* tmp = this->first->next;
		while (tmp && tmp->next)
		{
			mid = mid->next;
			tmp = tmp->next->next;
		}
		tmp = mid->next;
		mid->next = 0;
		return List(tmp);
	}

	List __sort__()
	{
		List right = this->divide();
		if (this->isEmpty())
		{
			this->first = right.first;
			return *this;;
		}
		if (right.isEmpty())
			return *this;
		return (this->__sort__()).Merge(right.__sort__());
	}

	void sort()
	{
		*this = *(&__sort__());
	}
	//////////////////////////////////////////////////////

	~List()
	{
		while (first)
		{
			del(0);
		}
	}


private:
	node<T>* first;
};

//LIST///////////////////////////////


template <typename T>
bool f(T a)
{
	if (a >= 0)
		return true;
	return false;
}

int main()
{
	List<int> a(cin);
	a.sort();
	a.print();
	return EXIT_SUCCESS; 
}
