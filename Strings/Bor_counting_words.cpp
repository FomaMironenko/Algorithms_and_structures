//#include "pch.h"
#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <vector>

using namespace std;


struct Node
{
	Node(char letter = ' '): letter(letter), n(0), son(nullptr), brother(nullptr)
	{	}

	char letter;		// буква соответствующая вершине
	int n;				// кол-во слов с концом в этой вершине 
	Node *son, *brother;// структура дерева
};

struct Bor
{
	Bor()
	{	
		root = new Node(' ');
	}

	void insert(string & word)
	{
		_insert(root, word, 0);
	}
	void _insert(Node* cur, string & word, int pos)
	{
		if (pos == word.size())
		{
			(cur->n)++;
		}
		else
		{
			char letter = word[pos];
			Node** tmp = &(cur->son);

			while (*tmp != nullptr)						// в частности обрабатывается случай root->son == nullptr
			{
				if (letter == (*tmp)->letter)
				{
					_insert(*tmp, word, pos + 1);
					return;
				}
				if (letter < (*tmp)->letter)
				{
					Node*  newnode = new Node(letter);
					newnode->brother = *tmp;			// *tmp текущая вешина
					*tmp = newnode;						// изменение значения указателя
					_insert(newnode, word, pos + 1);
					return;
				}
				tmp = &((*tmp)->brother);
			}
			// вставка в конец списка детей
			Node*  newnode = new Node(letter);
			*tmp = newnode;
			_insert(newnode, word, pos + 1);
		}
	}

	void output()
	{
		if (out.size() != 0)
		{
			out.clear();
		}
		dfs(root);
	}
	void dfs(Node* cur)
	{
		if (cur->n > 0)
		{
			for (auto it = out.begin(); it < out.end(); it++)
			{
				cout << *it;
			}
			cout << "  " << cur->n << endl;
		}

		Node* tmp = cur->son;
		while (tmp != nullptr)
		{
			out.push_back(tmp->letter);
			dfs(tmp);
			out.pop_back();
			tmp = tmp->brother;
		}
	}

	vector<char> out;
	Node* root;
};


void manual_input(Bor *dict)
{
	string word;
	while (cin >> word)
	{
		transform(word.begin(), word.end(), word.begin(), tolower);
		dict->insert(word);
	}
}

void file_input(Bor* dict, string file)
{
	ifstream input(file);
	if (!input.is_open())
	{
		cout << "file not found";
		return;
	}
	string word;
	while (input >> word)
	{
		transform(word.begin(), word.end(), word.begin(), tolower);
		dict->insert(word);
	}
	input.close();
}



int main()
{
	Bor dict;
	file_input(&dict, "input.txt");
	dict.output();

	return EXIT_SUCCESS;
}
