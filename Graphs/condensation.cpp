//#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#pragma warning(disable: 4996)

using namespace std;


int* to(int i, int wei = 1)
{
	int* New = new int[2];
	New[0] = i;
	New[1] = wei;
	return New;
}

int index(string A, vector<string> vars)
{
	return find(vars.begin(), vars.end(), A) - vars.begin();
}

struct graph
{
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0)
	{	}
	//enter the edj list
	//fills: edjes, adjacency
	graph(int V, bool dir = false, bool wei = false) : dir(dir), wei(wei),
		V(V), AdjMatr(0), Cond(0)
	{
		adjacency = vector<vector<int*>>(V);
		int v1;
		int v2;
		E = 0;

		while (cin >> v1, cin >> v2)
		{
			E++;
			int* tmp = new int[3];
			tmp[0] = v1 - 1; tmp[1] = v2 - 1;
			edjes.push_back(tmp);

			if (wei)
			{
				cin >> tmp[2];			//weight
				adjacency[v1 - 1].push_back(to(v2 - 1, tmp[2]));
				if (!dir)
					adjacency[v2 - 1].push_back(to(v1 - 1, tmp[2]));
			}
			else
			{
				adjacency[v1 - 1].push_back(to(v2 - 1));
				if (!dir)
					adjacency[v2 - 1].push_back(to(v1 - 1));
			}
		}
		cin.clear();
		this->E = E;
	}


	void transpose()
	{
		int* out_deg = new int[V];
		for (int i = 0; i < V; i++)
			out_deg[i] = adjacency[i].size();
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < out_deg[i]; j++)
			{
				int* New = new int[2];
				New[0] = i;
				New[1] = adjacency[i][0][1];
				adjacency[adjacency[i][0][0]].push_back(New);
				adjacency[i].erase(adjacency[i].begin());
			}
		}
	}


	//////////////////////////////// CONDENSATION ////////////////////////////////
	void DFS_out(int cur)
	{
		static int T = 0;
		static int* notVisited = new int[V];

		if (!notVisited[cur])
			return;
		notVisited[cur] = 0;
		for (int j = 0; j < adjacency[cur].size(); j++)
			if (notVisited[adjacency[cur][j][0]])
				DFS_out(adjacency[cur][j][0]);
		out[T++] = cur;
	}

	void DFS_comp(int cur, graph& cond)
	{
		component[cur] = cond.V - 1;
		comp_list[cond.V - 1].push_back(cur);
		for (int i = 0; i < adjacency[cur].size(); i++)
		{
			if (component[adjacency[cur][i][0]] < 0)
				DFS_comp(adjacency[cur][i][0], cond);
			else if (component[adjacency[cur][i][0]] != cond.V - 1)
			{
				for (int j = 0; j < cond.adjacency[component[adjacency[cur][i][0]]].size(); j++)
					if (cond.adjacency[component[adjacency[cur][i][0]]][j][0] == cond.V - 1)
						goto doubleedge;
				cond.E++;
				cond.adjacency[component[adjacency[cur][i][0]]].push_back(to(cond.V - 1, adjacency[cur][i][1]));
			doubleedge:;
			}
		}
	}

	//fills: component, out, comp_list, Cond
	graph* condensation()
	{
		out = new int[V];
		for (int i = 0; i < V; i++)
			DFS_out(i);

		graph* cond = new graph;
		Cond = cond;
		cond->dir = true;
		cond->V = 0; cond->E = 0;
		cond->adjacency = vector<vector<int*>>(0);

		this->transpose();
		component = new int[V];
		comp_list = vector<vector<int>>(0);
		for (int i = V - 1; i >= 0; i--)
		{
			if (component[out[i]] < 0)
			{
				cond->V++;
				comp_list.push_back(vector<int>(0));
				cond->adjacency.push_back(vector<int*>(0));
				DFS_comp(out[i], *cond);
			}
		}
		this->transpose();
		return cond;
	}
	//////////////////////////////////////////////////////////////////////////////


	//only after condensation()
	void print_condensation()
	{
		cout << "\nCondensation\n\n";
		for (int i = 0; i < Cond->V; i++)
		{
			cout << i + 1 << " -> ";
			for (int j = 0; j < Cond->adjacency[i].size(); j++)
			{
				cout << Cond->adjacency[i][j][0] + 1 << " ";
			}
			cout << "\nConsists of: ";
			for (int j = 0; j < comp_list[i].size(); j++)
			{
				if (!vars.empty())
					cout << vars[comp_list[i][j]] << " ";
				else
					cout << comp_list[i][j] + 1 << " ";
			}
			cout << endl;
		}
	}
	////////////////////////////////////////////////////////////////////////


	int V, E;
	bool dir;						//directed
	bool wei;						//weight
private:
	vector<int*> edjes;				//edjes list
	vector<vector<int*>> adjacency;	//adjacency list

	int** AdjMatr;
	int** IncMatr;

	//    TOPOLOGY SORT		//
	vector<int> top_sort;	// sequence of vertexes in order of topology sort

	//    CONDENSATION		//
	int* out;				// time out
	int* component;			// component of the vertex
	vector<vector<int>> comp_list;	// list of vertexes from source graph for each component
	graph* Cond;			// grahp-condensation

	//        2-SAT			//
	vector<string> vars;	// variables from expression in order of vertexes
	bool* values;			// value for each variable from vars
};



int main()
{
	int V;
	cout << "Enter the number of vertexes ";
	cin >> V;
	cout << "Enter the edges like  v1 v2. In the end of input enter ctrl+z on Windows\n";
	graph a(V, true);

	graph* cond = a.condensation();
	a.print_condensation();
}