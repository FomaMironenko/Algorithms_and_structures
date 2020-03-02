#include "pch.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

typedef tuple<int, int, int> triple;
typedef vector<vector<pair<int, int>>> adj;

struct DSU
{
	DSU(int V)
	{
		parent = new int[V];
		size = new int[V];
		for (int i = 0; i < V; i++)
			Set(i);
	}

	int Set(int x)
	{
		size[x] = 1;
		return parent[x] = x;
	}

	int Find(int x)
	{
		if (parent[x] == x)
			return x;
		return parent[x] = Find(parent[x]);
	}

	void Union(int x, int y)
	{
		x = Find(x);
		y = Find(y);
		if (x == y)
			return;
		if (size[y] < size[x])
			swap(x, y);
		parent[x] = y;
		size[y] = max(size[y], 1 + size[x]);
		return;
	}

	~DSU()
	{
		delete[] parent;
		delete[] size;
	}

	int* parent;
	int* size;
};

struct graph
{
	graph()
	{	}
	//enter the edj list
	//fills: edjes, adjacency
	void input(int V)
	{
		this->V = V;
		cout << "Enter the edjes list\n";
		int v1, v2, w;
		E = 0;

		while (cin >> v1 >> v2 >> w)
		{
			v1--; v2--;
			E++;
			edjes.push_back(triple(w, v1, v2));
		}
		cin.clear();
	}

	graph* kruskall()
	{
		sort(edjes.begin(), edjes.end());
		DSU forest(V);
		graph* tree = new graph;
		tree->adjacency = adj(V);
		tree->V = V;
		tree->W = 0;
		for (int i = 0; i < E; i++)
		{
			if (forest.Find(get<1>(edjes[i])) != forest.Find(get<2>(edjes[i])))
			{
				forest.Union(get<1>(edjes[i]), get<2>(edjes[i]));
				tree->W += get<0>(edjes[i]);
				tree->adjacency[get<1>(edjes[i])].push_back(pair<int, int>(get<0>(edjes[i]), get<2>(edjes[i])));
				tree->adjacency[get<2>(edjes[i])].push_back(pair<int, int>(get<0>(edjes[i]), get<1>(edjes[i])));
			}
		}
		return tree;
	}

	void print()
	{
		for (int i = 0; i < V; i++)
		{
			cout << i + 1 << " -> ";
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				cout << adjacency[i][j].second + 1 << ", ";
			}
			cout << endl;
		}
	}

	int V, E;						
	int W;
private:
	vector<triple> edjes;				//edjes list
	adj adjacency;						//adjacency list
};


int main()
{
	graph gr;
	int n;
	cout << "enter the number of vertexes ";
	cin >> n;
	gr.input(n);
	graph* tree = gr.kruskall();
	cout << tree->W << endl;
	tree->print();

	return EXIT_SUCCESS; 
}