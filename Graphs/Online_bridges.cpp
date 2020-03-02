#include "pch.h"
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <conio.h>
using namespace std;


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

	int Union(int x, int y)
	{
		x = Find(x);
		y = Find(y);
		if (x == y)
			return y;
		if (size[y] < size[x])
			swap(x, y);
		parent[x] = y;
		size[y] = max(size[y], 1 + size[x]);
		return y;
	}

	void StatUnion(int x, int y)
	{
		x = Find(x);
		y = Find(y);
		if (x == y)
			return;
		parent[y] = x;
	}

	int* parent;
	int* size;
};

struct node
{
	node(): adj(vector<node*>(0))
	{	}
	node(int rep): adj(vector<node*>(0)), representative(rep)
	{	}

	void Del(node* new_rep)
	{
		for (int i = 0; i < adj.size(); i++)
		{
			adj[i]->adj.erase(find(adj[i]->adj.begin(), adj[i]->adj.end(), this));
			if (find(adj[i]->adj.begin(), adj[i]->adj.end(), new_rep) == adj[i]->adj.end())
			{
				adj[i]->adj.push_back(new_rep);
				new_rep->adj.push_back(adj[i]);
			}
		}
		this->~node();
	}

	~node()
	{
		adj.clear();
	}

	vector<node*> adj;
	int representative;
};

struct Forest
{
	Forest(int V): V(V)
	{
		verts = new node*[V];
		for (int i = 0; i < V; i++)
			verts[i] = new node(i);
	}

	void connect(int v1, int v2)
	{
		verts[v1]->adj.push_back(verts[v2]);
		verts[v2]->adj.push_back(verts[v1]);
	}

	void terminate_cycle(int v1, int v2, DSU* biconn, vector<pair<int, int>>& bridges)
	{
		int new_rep = biconn->Union(v1, v2);
		if (!verts[v1] || !verts[v2])
		{
			cout << "ERROR: already deleted";
			return;
		}
		int* mark = new int[V];
		int*parent = new int[V];
		for (int i = 0; i < V; i++)
			mark[i] = 0;
		queue<int> Q1;
		queue<int> Q2;
		Q1.push(v1);
		Q2.push(v2);
		int cur1;
		int cur2;
		mark[v1] = 1;
		mark[v2] = 2;
		while (!Q1.empty() && !Q2.empty())
		{
			cur1 = Q1.front();
			cur2 = Q2.front();
			Q1.pop(); Q2.pop();
			for (int i = 0; i < verts[cur1]->adj.size(); i++)
			{
				if (mark[verts[cur1]->adj[i]->representative] == 0)
				{
					Q1.push(verts[cur1]->adj[i]->representative);
					parent[verts[cur1]->adj[i]->representative] = cur1;
					mark[verts[cur1]->adj[i]->representative] = 1;
				}
				else if (mark[verts[cur1]->adj[i]->representative] == 2)
				{
					cur2 = verts[cur1]->adj[i]->representative;
					goto found;
				}
			}
			for(int i = 0; i < verts[cur2]->adj.size(); i++)
			{
				if (mark[verts[cur2]->adj[i]->representative] == 0)
				{
					Q2.push(verts[cur2]->adj[i]->representative);
					parent[verts[cur2]->adj[i]->representative] = cur2;
					mark[verts[cur2]->adj[i]->representative] = 2;
				}
				else if (mark[verts[cur2]->adj[i]->representative] == 1)
				{
					cur1 = verts[cur2]->adj[i]->representative;
					goto found;
				}
			}
		}
	found:;
		//удаляем все вешины из пути, прицепляя их соседей к новому представителю.
		//представитель выбирается исходя из результата Union
		while (cur1 != v1)
		{
			if (verts[cur1] == 0)
			{
				cout << "Error 1";
				cur1 = parent[cur1];
				continue;
			}
			verts[cur1]->Del(verts[new_rep]);
			verts[cur1] = 0;
			biconn->StatUnion(new_rep, cur1);
			cur1 = parent[cur1];
		}
		while (cur2 != v2)
		{
			if (verts[cur1] == 0)
			{
				cout << "Error 2";
				cur2 = parent[cur2];
				continue;
			}
			verts[cur2]->Del(verts[new_rep]);
			verts[cur2] = 0;
			biconn->StatUnion(new_rep, cur2);
			cur2 = parent[cur2];
		}

		if (new_rep == v2)
			v2 = v1;
		//now: new_rep - new representative of bicon class
		//v2 - vertex that should be deleted
		verts[v2]->Del(verts[new_rep]);
		verts[v2] = 0;
		int i = 0;
		while (i < bridges.size())
		{
			if (biconn->Find(bridges[i].first) == biconn->Find(bridges[i].second) && biconn->Find(bridges[i].first) == new_rep)
			{
				printf("\x1B[31m(%d,%d) \033[0m", bridges[i].first + 1, bridges[i].second + 1);
				bridges.erase(bridges.begin() + i);
			}
			else
			{
				cout << "(" << bridges[i].first + 1 << "," << bridges[i].second + 1 << ") ";
				i++;
			}
		}
		cout << endl;
	}

	int V;
	node** verts;
};



void online_bridges()
{
	int V;
	cout << "Enter the number of vertexes: ";
	cin >> V;

	DSU conn(V);
	DSU biconn(V);
	vector<pair<int, int>> bridges(0);
	
	Forest forest(V);

	int v1, v2;
	int comp1, comp2;
	int Bcomp1, Bcomp2;
	cout << "Enter the edjes as v1 v2.\nIn the end of input enter ctrl+z (on Windows)\n";
	while (cin >> v1 >> v2)
	{
		v1--; v2--;
		comp1 = conn.Find(v1);
		comp2 = conn.Find(v2);
		Bcomp1 = biconn.Find(v1);
		Bcomp2 = biconn.Find(v2);

		if (comp1 != comp2)
		{
			for (int i = 0; i < bridges.size(); i++)
				cout << "(" << bridges[i].first + 1 << "," << bridges[i].second + 1 << ") ";
			bridges.push_back(pair<int, int>(v1, v2));
			printf("\x1B[32m(%d,%d)\n\033[0m", bridges[bridges.size() - 1].first + 1, bridges[bridges.size() - 1].second + 1);
			forest.connect(Bcomp1, Bcomp2);
			conn.Union(comp1, comp2);
		}
		else if (Bcomp1 != Bcomp2)
			forest.terminate_cycle(Bcomp1, Bcomp2, &biconn, bridges);
		else
		{
			for (int i = 0; i < bridges.size(); i++)
				cout << "(" << bridges[i].first + 1 << "," << bridges[i].second + 1 << ") ";
			cout << endl;
		}
	}
	cout << "Components:\n";
	for (int i = 0; i < V; i++)
		cout << i + 1 << " in " << biconn.Find(i) + 1 << endl;
}

int main()
{
	online_bridges();
    return EXIT_SUCCESS; 
}