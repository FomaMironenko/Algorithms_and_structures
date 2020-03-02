//#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

#define adj vector<vector<pair<int, string>>>

template <class T>
vector<T> combine(vector<T> a, vector<T> b)
{
	vector<T> tmp = b;
	for (auto ita = a.begin(); ita < a.end(); ita++)
	{
		for (auto itb = b.begin(); itb < b.end(); itb++)
		{
			if (*ita == *itb)
			{
				break;
			}
			tmp.push_back(*ita);
		}
	}
	return tmp;
}

//an auxiliary functions for beautifol output
int symbs(int n)
{
	return 2 + 3 * n;
}
void spaces(int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << " ";
	}
}

//will be inherited from Graph
struct Tensor;

struct Graph
{
	Graph() : V(0), E(0)
	{	}
	Graph(int V) : V(V)
	{
		AdjMatr = new vector<string>*[V];
		for (int i = 0; i < V; i++)
		{
			AdjMatr[i] = new vector<string>[V];
		}
	}
	Graph(int V, bool CFgr): V(V)
	{
		int v1;
		int v2;
		string term;
		E = 0;
		if (CFgr)
		{
			starts = set<int>();
			finals = set<int>();
			nonterms = map<int, set<string>>();

			cout << "enter starts. In each row: <number of nonterminals> <vertex number> <nonterminals>\n";
			while (cin >> v1, cin >> v2)
			{
				starts.insert(v2);
				nonterms[v2] = set<string>();
				cin >> term;
				nonterms[v2].insert(term);
				for (int i = 1; i < v1; i++)
				{
					cin >> term;
					nonterms[v2].insert(term);
				}
			}
			cin.clear();
			cout << "enter finals. In each row: <number of nonterminals> <vertex number> <nonterminals>\n";
			while (cin >> v1, cin >> v2)
			{
				finals.insert(v2);
				if (v1 == 0)
				{
					continue;
				}
				cin >> term;
				if (nonterms.count(v2) <= 0)
				{
					nonterms[v2] = set<string>();
					nonterms[v2].insert(term);
				}
				else
				{
					nonterms[v2].insert(term);
				}
				for (int i = 1; i < v1; i++)
				{
					cin >> term;
					nonterms[v2].insert(term);
				}
			}
			cin.clear();
		}

		AdjMatr = new vector<string>*[V];
		for (int i = 0; i < V; i++)
		{
			AdjMatr[i] = new vector<string>[V];
		}

		//entering graph by the edje list
		cout << "enter the edge list\n";
		while (cin >> v1, cin >> v2, cin >> term)
		{
			AdjMatr[v1][v2].push_back(term);
		}
		cin.clear();
		cout << "\n";
	}

	void transitive_closure()
	{
		for (int k = 0; k < V; k++)
		{
			for (int i = 0; i < V; i++)
			{
				for (int j = 0; j < V; j++)
				{
					if (AdjMatr[i][k].empty() || AdjMatr[k][j].empty())
					{
						continue;
					}
					vector<string> tmp = combine<string>(AdjMatr[i][k], AdjMatr[k][j]);
					for (auto it = tmp.begin(); it < tmp.end(); it++)
					{
						if (find(AdjMatr[i][j].begin(), AdjMatr[i][j].end(), *it) == AdjMatr[i][j].end())
						{
							AdjMatr[i][j].push_back(*it);
						}
					}
				}
			}
		}
	}

	Tensor* operator*(Graph const & right);
	
	void print()
	{
		//for beautiful output
		//длина максимального списка в колонке
		int* lens = new int[V];
		for (int j = 0; j < V; j++)
		{
			lens[j] = 0;
			for (int i = 0; i < V; i++)
			{
				if (AdjMatr[i][j].size() > lens[j])
				{
					lens[j] = AdjMatr[i][j].size();
				}
			}
		}

		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				cout << "{";
				if (!AdjMatr[i][j].empty())
				{
					for (int k = 0; k < AdjMatr[i][j].size(); k++)
					{
						cout << AdjMatr[i][j][k] << ", ";
					}
				}
				cout << "}  ";
				spaces(symbs(lens[j] - AdjMatr[i][j].size()));
			}
			cout << "\n";
		}
		cout << "\n\n";
	}

	~Graph()
	{
		for (int i = 0; i < V; i++)
		{
			delete [] AdjMatr[i];
		}
		starts.clear();
		finals.clear();
		nonterms.clear();
	}

	int V, E;				//adjacency list
	set<int> starts;
	set<int> finals;
	map<int, set<string>> nonterms; //matches nonterminals for start/final vertex
	vector<string>** AdjMatr;
};

//tensor product
struct Tensor : Graph
{
	//index in left factor
	int left_ind(int i)
	{
		return i / rightV;
	}
	//index in right factor
	int right_ind(int i)
	{
		return i % rightV;
	}
	//index in tensor from indexes in right and left factors
	int ind(int l, int r)
	{
		return l * rightV + r;
	}

	int leftV;
	int rightV;
};


Tensor* Graph::operator*(Graph const & right)
{
	Tensor* tens = new Tensor; // tensor product
	tens->V = this->V * right.V;
	tens->leftV = this->V;
	tens->rightV = right.V;
	tens->AdjMatr = new vector<string>*[tens->V];
	for (int i = 0; i < tens->V; i++)
	{
		tens->AdjMatr[i] = new vector<string>[tens->V];
		for (int j = 0; j < tens->V; j++)
		{
			auto left_symb = &(this->AdjMatr[tens->left_ind(i)][tens->left_ind(j)]);
			auto right_symb = &(right.AdjMatr[tens->right_ind(i)][tens->right_ind(j)]);
			for (auto itl = left_symb->begin(); itl < left_symb->end(); itl++)
			{
				for (auto itr = right_symb->begin(); itr < right_symb->end(); itr++)
				{
					if (*itl == *itr)
					{
						tens->AdjMatr[i][j].push_back(*itl);
					}
				}
			}
		}
	}
	return tens;
}


void solve(Graph* CF, Graph* SM)
{
	bool changed = true;
	while (changed)
	{
		changed = false;
		Tensor* T = (*CF) * (*SM);
		T->transitive_closure();

		//перебираем только нужные блоки тензорного произведения
		//циклы только выглядят страшно
		//блоки, соответствующие начальным и конечным вершинам:
		for (auto cfi = CF->starts.begin(); cfi != CF->starts.end(); cfi++)
		{
			for (auto cfj = CF->finals.begin(); cfj != CF->finals.end(); cfj++)
			{
				//перебор элементов блоков
				for (int smi = 0; smi < SM->V; smi++)
				{
					for (int smj = 0; smj < SM->V; smj++)
					{
						if (!(T->AdjMatr[T->ind(*cfi, smi)][T->ind(*cfj, smj)]).empty())
						{
							//добавление в граф нетерминалов из пересечения нетерминалов 
							//для начальной и конечной вершин рекурсивного автомата
							for (auto k = CF->nonterms[*cfi].begin(); k != CF->nonterms[*cfi].end(); k++)
							{
								if (CF->nonterms[*cfj].find(*k) != CF->nonterms[*cfj].end() && 
									find(SM->AdjMatr[smi][smj].begin(), SM->AdjMatr[smi][smj].end(), *k) == SM->AdjMatr[smi][smj].end())
								{
									changed = true;
									SM->AdjMatr[smi][smj].push_back(*k);
								}
							}
						}
					}
				}
			}
		}
	}
	SM->print();
	
}

void check_clousure()
{
	Graph gr(16, false);
	gr.transitive_closure();
	gr.print();
}

int main()
{
	cout << "The vertex numeration starts with 0\nIn the end of each part of input enter crtl+z\n\n";
	cout << "** CF recursive automate **\n";
	Graph CF(4, true);
	cout << "**     State machine     **\n";
	Graph SM(4, false);
	
	solve(&CF, &SM);

    return EXIT_SUCCESS; 
}

/*
!!!! input for the example from the conspect !!!!

1 0 S

1 3 S

0 1 a
1 2 S
1 3 b
2 3 b

0 1 a
1 2 a
2 0 a
2 3 b
3 2 b
*/