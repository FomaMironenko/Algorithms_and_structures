#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#pragma warning (disable:4669)

using namespace std;

struct val
{
	val(int i, int depth): i(i), depth(depth) {}

	int i;
	int depth;
};

struct node
{
	node(int n): letters(new node*[n]), target(false), visited(false), suff(0)
	{
		for (int i = 0; i < n; i++)
			letters[i] = 0;
	}

	char get_let(string all_let, node* cur)
	{
		for (int i = 0; i < all_let.length(); i++)
		{
			if (this->letters[i] == cur)
				return all_let[i];
		}
	}

	bool visited;
	bool target;
	node** letters; 
	node* parent;
	node* suff;
};

string set_lets(vector<string> dict)
{
	string all_let = "";
	int n = dict.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < dict[i].length(); j++)
		{
			if (all_let.find(dict[i][j]) == string::npos)
				all_let += dict[i][j];
		}
	}
	return all_let;
}



struct automate
{
	automate(int n)
	{
		root = new node(n);
		root->parent = root;
		root->suff = root;
	}

	void buildBor(vector<string> dict, int W, string all_let)
	{
		node* iterrator = root;
		for (int i = 0; i < W; i++)
		{
			for (int j = 0; j < dict[i].length(); j++)
			{
				if (!iterrator->letters[all_let.find(dict[i][j])])
				{
					node* tmp = new node(all_let.length());
					tmp->parent = iterrator;
					iterrator->letters[all_let.find(dict[i][j])] = tmp;
				}
				iterrator = iterrator->letters[all_let.find(dict[i][j])];
				if (j == dict[i].length() - 1)
					iterrator->target = true;
			}
			iterrator = root;
		}
	}

	node* set_lett(node*, string, char);

	node* set_suff(node* cur, string all_let)
	{
		if (!cur->suff)
		{
			if (cur == root)
				return cur->suff = root;
			if (cur->parent == root)
				return cur->suff = root;
			return cur->suff = set_lett(set_suff(cur->parent, all_let), all_let, cur->parent->get_let(all_let, cur));
		}
		return cur->suff;
	}

	void buildAutomate(string all_let, node* cur)
	{
		static node* tmp = cur;
		cur->visited = true;
		for (int i = 0; i < all_let.length(); i++)
		{
			tmp = cur;
			if(!cur->letters[i])
				cur->letters[i] = set_lett(tmp, all_let, all_let[i]);
			if (!cur->letters[i]->visited)
				buildAutomate(all_let, cur->letters[i]);
		}
	}

	int get_depth(node* cur)
	{
		node* tmp = cur;
		int counter = 0;
		if (cur == root)
			return counter;
		counter++;
		while (tmp->parent != root)
		{
			counter++;
			tmp = tmp->parent;
		}
		return counter;
	}

	vector<val> find(string text, string all_let)
	{
		vector<val> arr;
		node* iterrator = root;
		for (int i = 0; i < text.length(); i++)
		{
			if (all_let.find(text[i]) == string::npos)
				iterrator = root;
			else
				iterrator = iterrator->letters[all_let.find(text[i])];
			node* tmp = iterrator;
			while (tmp != root)
			{
				if (tmp->target)
				{
					arr.push_back(val(i, get_depth(tmp)));
					break;
				}
				tmp = tmp->suff;
			}
		}
		return arr;
	}

	node* root;
};
node* automate::set_lett(node* cur, string all_let, char lett)
{
	if (!cur->letters[all_let.find(lett)])
	{
		if (cur == root)
			return cur->letters[all_let.find(lett)] = root;
		return cur->letters[all_let.find(lett)] = set_lett(set_suff(cur, all_let), all_let, lett);
	}
	return cur->letters[all_let.find(lett)];
}



int main()
{
	cout << "Enter the dictionary. After the last word enter 0\n";
	vector<string> dict;
	string tmp;
	while (true)
	{
		getline(cin, tmp);
		if (tmp == "0")
			break;
		dict.push_back(tmp);
	}
	string all_let = set_lets(dict);
	automate Auto(all_let.length());
	Auto.buildBor(dict, dict.size(), all_let);
	Auto.buildAutomate(all_let, Auto.root);

	cout << "Enter the text\n";
	string text;
	getline(cin, text);

	vector<val> filths = Auto.find(text, all_let);
	for (int i = 0; i < filths.size(); i++)
	{
		for (int j = 0; j < filths[i].depth-1; j++)
			text[filths[i].i - j] = '*';
	}
	cout << text;

	return EXIT_SUCCESS; 
}
