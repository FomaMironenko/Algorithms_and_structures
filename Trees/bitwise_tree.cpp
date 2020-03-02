#include "pch.h"
#include <iostream>

using namespace std;

int Pow(int a, int n)
{
	if (n == 0)
		return 1;
	if (n % 2 == 0)
	{
		int tmp = Pow(a, n/2);
		return tmp * tmp;
	}
	return a * Pow(a, n - 1);
}

template<class T>
struct vertex
{
	virtual void add(int key, T data) = 0;
	virtual T get(int key) const = 0;
	virtual void dfs(int cur_lev, int p) const = 0;
	virtual ~vertex() 
	{	}
};


template<class T>
struct leaf : vertex<T>
{
	leaf(T data) : data(data)
	{	}
	leaf()
	{	}

	void add(int key, T data)
	{
		this->data = data;
	}

	T get(int key) const
	{
		return data;
	}

	void dfs(int cur_lev, int p) const
	{
		cout << cur_lev << " " << data << endl;
	}

	T data;
};

template<class T>
struct node : vertex<T>
{
	node(): cont(0), zero(0), one(0)
	{	}

	void add(int key, T data)
	{
		if (key == 0)
		{
			if (cont)
				cont->add(key, data);
			else
				cont = new leaf<T>(data);
		}
		else if ((key & 1) == 0)
		{
			if (!zero)
				zero = new node<T>();
			zero->add(key >> 1, data);
		}
		else
		{
			if (!one)
				one = new node();
			one->add(key >> 1, data);
		}
	}

	T get(int key) const
	{
		if (key == 0)
		{
			if (!cont)
				return numeric_limits<T>::min();
			else
				return cont->get(key);
		}
		else if ((key & 1) == 0)
		{
			if (!zero)
				return numeric_limits<T>::min();
			return zero->get(key >> 1);
		}
		else
		{
			if (!one)
				return numeric_limits<T>::min();
			return one->get(key >> 1);
		}
	}

	void dfs(int cur_lev, int p) const
	{
		if (cont)
		{
			cont->dfs(cur_lev, p);
		}
		if (zero)
		{			
			zero->dfs(cur_lev, p + 1);
		}
		if (one)
		{
			one->dfs(cur_lev + Pow(2, p), p + 1);
		}
	}

	void del(int key, bool* empty_branch)
	{
		if (key == 0)
		{
			if (cont)
			{
				delete cont;
				cont = 0;
				if (zero || one)
					*empty_branch = false;
			}
			*empty_branch = false;
		}
		else if ((key & 1) == 0)
		{
			if (zero)
				((node<T>*)zero)->del(key >> 1, empty_branch);
			else
			{
				*empty_branch = false;
				return;
			}
			if (*empty_branch)
			{
				delete zero;
				zero = 0;
				if (one || cont)
				{
					*empty_branch = false;
				}
			}
		}
		else
		{
			if (one)
				((node<T>*)one)->del(key >> 1, empty_branch);
			else
			{
				*empty_branch = false;
				return;
			}
			if (*empty_branch)
			{
				delete one;
				one = 0;
				if (zero || cont)
				{
					*empty_branch = false;
				}
			}
		}
	}

	vertex<T>* cont;
	vertex<T>* zero;
	vertex<T>* one;
};


template<class T>
struct BitTree
{
	BitTree():root(new node<T>())
	{	}

	void add(int key, T data)
	{
		if(key == 0)
			((node<T>*)root)->cont = new leaf<T>(data);
		else if ((key & 1) == 0)
		{
			if (!((node<T>*)root)->zero)
				((node<T>*)root)->zero = new node<T>();
			(((node<T>*)root)->zero)->add(key >> 1, data);
		}
		else
		{
			if (!((node<T>*)root)->one)
				((node<T>*)root)->one = new node<T>();
			(((node<T>*)root)->one)->add(key >> 1, data);
		}
	}

	T get(int key)
	{
		if (key == 0)
		{
			if (!((node<T>*)root)->cont)
				return numeric_limits<T>::min();
			else
				return ((node<T>*)root)->cont->get(key);
		}
		if ((key & 1) == 0)
		{
			if (!((node<T>*)root)->zero)
				return numeric_limits<T>::min();
			return (((node<T>*)root)->zero)->get(key >> 1);
		}
		else
		{
			if (!((node<T>*)root)->one)
				return numeric_limits<T>::min();
			return (((node<T>*)root)->one)->get(key >> 1);
		}
	}

	void del(int key)
	{
		bool empty_branch = true;
		((node<T>*)root)->del(key, &empty_branch);
	}

	void print()
	{
		root->dfs(0, 0);
	}

	vertex<T>* root;
};


int main()
{
	BitTree<float> a;
	a.add(40, 67.5);
	a.add(24, 12.1);
	a.add(29, 8);
	a.add(128, 7.5);
	a.add(0, 4);
	a.add(13, 8);
	a.add(55, 5);

	a.del(128);
	a.del(228);
	a.del(0);

	a.add(78, 1488);

	a.del(24);


	a.print();

    return EXIT_SUCCESS; 
}