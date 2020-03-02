#include "pch.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <math.h>

using namespace std;


struct node
{
	node(int n) : letters(new node*[n]), delts(new node*[n]), target(false), visited(false), suff(0)
	{
		for (int i = 0; i < n; i++)
		{
			letters[i] = 0;
			delts[i] = 0;
		}
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
	int target;
	node** letters;
	node** delts;
	node* parent;
	node* suff;
};


struct searchers
{
	searchers(node* vert, long long visited, string Str): vertex(vert), visited(visited), Str(Str) {}

	node* vertex;
	long long visited;
	string Str;
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
		static int n = 1;
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
					iterrator->delts[all_let.find(dict[i][j])] = tmp;
				}
				iterrator = iterrator->letters[all_let.find(dict[i][j])];
				if (j == dict[i].length() - 1)
					iterrator->target = n++;
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
			if (!cur->delts[i])
				cur->delts[i] = set_lett(tmp, all_let, all_let[i]);
			if (!cur->delts[i]->visited)
				buildAutomate(all_let, cur->delts[i]);
		}
	}


//START OF THE MAIN PART

	void bfs(queue<searchers> Q, int N, string all_let)
	{
		static bool found = false;
		while(!found)
		{
			searchers cur = Q.front();
			node* tmp = cur.vertex;
			while (tmp != root)
			{
				if (tmp->target)
					cur.visited |= (1 << (tmp->target - 1));
				tmp = tmp->suff;
			}
			if (cur.visited == pow(2, N) - 1)
			{
				found = true;
				cout << cur.Str;
			}

			if (!found)
			{
				Q.pop();

				for (int i = 0; i < all_let.length(); i++)
				{
					if (cur.vertex->letters[i] != NULL)
					{
						searchers tmp = cur;
						tmp.Str += all_let[i];
						tmp.vertex = cur.vertex->letters[i];
						Q.push(tmp);
					}
				}
				if (cur.vertex->target)
				{
					searchers tmp = cur;
					tmp.vertex = tmp.vertex->suff;
					Q.push(tmp);
				}
			}
		}
	}

	string all_strs(int N, string all_let)
	{
		queue<searchers> Q;
		Q.push(searchers(root, 0, ""));
		bfs(Q, N, all_let);
		cout << Q.front().Str;
		return Q.front().Str;
	}

//END OF THE MAIN PART

	node* root;
};
node* automate::set_lett(node* cur, string all_let, char lett)
{
	if (!cur->delts[all_let.find(lett)])
	{
		if (cur == root)
			return root;
		return set_lett(set_suff(cur, all_let), all_let, lett);
	}
	return cur->delts[all_let.find(lett)];
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

	string answer = Auto.all_strs(dict.size(), all_let);


	return EXIT_SUCCESS;
}