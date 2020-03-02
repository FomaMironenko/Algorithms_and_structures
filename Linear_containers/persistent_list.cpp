#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct node
{
	node(int value): value(value)
	{
		next = 0;
	}

	int value;
	node* next;
};

struct Pers_list
{
	Pers_list()
	{	}
	Pers_list(int a)
	{
		node* New = new node(a);
		firsts.push_back(New);
	}

	void push(int a)
	{
		if (firsts.empty())
		{
			node* New = new node(a);
			New->next = 0;
			firsts.push_back(New);
			return;
		}

		node* cur = *(firsts.end() - 1);
		if (a > cur->value)
		{
			node* New = new node(a);
			New->next = cur;
			firsts.push_back(New);
		}
		else
		{
			node* iter = cur;
			node* tmp = new node(iter->value);
			cur = tmp;
			firsts.push_back(tmp);
			iter = iter->next;
			if (iter != 0)
			{
				while (a < iter->value)
				{
					if (!iter->next)
					{
						iter = 0;
						break;
					}

					tmp = new node(iter->value);
					cur->next = tmp;
					cur = tmp;
					iter = iter->next;
				}
			}
			tmp = new node(a);
			tmp->next = iter;
			cur->next = tmp;
		}
	}

	int get(int n, int k)
	{
		node* cur = firsts[n - 1];
		for (int i = 0; i < k - 1; i++)
		{
			cur = cur->next;
		};
		return cur->value;
	}

	vector<node*> firsts;
};


int main()
{
	Pers_list List;
	string inp;
	int a, b;
	cout << "To insert an element enter                  ins A\n";
	cout << "To get k-th element out of first n enter    chk n k\n";
	while (cin >> inp)
	{
		if (inp == "ins")
		{
			cin >> a;
			List.push(a);
		}
		if (inp == "chk")
		{
			cin >> a >> b;
			cout << List.get(a, b) << endl;
		}
	}

    return EXIT_SUCCESS; 
}