#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int* to(int i, int wei = 1)
{
	int* New = new int[2];
	New[0] = i;
	New[1] = wei;
	return New;
}

struct graph
{
	//fills: edjes, adjacency
	graph(int V): V(V)
	{
		adjacency = vector<vector<int*>>(V);
		int v1;
		int v2;
		E = 0;

		while (cin >> v1, cin >> v2)
		{
			E++;
			adjacency[v1 - 1].push_back(to(v2 - 1));
		}
		cin.clear();
	}


	void DFS_sort(int cur, bool* visited, int* tOut)
	{
		static bool cycle = false;
		static int T = 1;

		if (visited[cur] && tOut[cur] >= 1)
		{
			return;
		}
		visited[cur] = true;
		for (int j = 0; j < adjacency[cur].size() && !cycle; j++)
		{
			if (!visited[adjacency[cur][j][0]])
			{
				DFS_sort(adjacency[cur][j][0], visited, tOut);
			}
			else if (tOut[adjacency[cur][j][0]] <= 0)
			{
				top_sort.insert(top_sort.begin(), -1);
				cycle = true;
				return;
			}
		}
		if (!cycle)
		{
			tOut[cur] = T++;
			top_sort.insert(top_sort.begin(), cur + 1);
		}
	}

	//fills: top_sort
	void check_sort()
	{
		top_sort.clear();
		static bool* visited = new bool[V];
		static int* tOut = new int[V];
		for (int i = 0; i < V; i++)
		{
			visited[i] = false;
			tOut[i] = -1;
		}
		for (int i = 0; i < V; i++)
		{
			DFS_sort(i, visited, tOut);
		}
		delete[] visited;
		delete[] tOut;

		if (top_sort[0] == -1)
		{
			cout << "There's a cycle";
			return;
		}
		
		for (int i = 0; i < V - 1; i++)
		{
			vector<int*>* cur = &adjacency[top_sort[i] - 1];
			bool ok = false;
			for(int j = 0; j < cur->size(); j++)
			{
				if ((*cur)[j][0] == top_sort[i + 1] - 1)
				{
					ok = true;
				}
			}
			if (!ok)
			{
				cout << "There are more then 1 ways to sort";
				return;
			}
		}
		cout << "There is the only one way to sort";
	}

	int V, E;
private:
	vector<vector<int*>> adjacency;	//adjacency list

	//    TOPOLOGY SORT		//
	vector<int> top_sort;	// sequence of vertexes in order of topology sort 
};


int main()
{
	cout << "Enter the number of vertexes\n";
	int V;
	cin >> V;
	cout << "Enter the edjes list. In the end enter ctrl + z\n";
	graph gr(V);
	
	gr.check_sort();

    return EXIT_SUCCESS; 
}