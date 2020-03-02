#include "pch.h"
#include <limits>
#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

struct Edge
{
	Edge(): v1(-1), v2(-1), w(1) 
	{	}

	void get(bool wei)
	{
		cin >> v1 >> v2;
		v1--; v2--;
		if (wei)
		{
			cin >> w;
		}
	}

	int v1;
	int v2;
	int w;
};

struct graph
{
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0), R_2(0)
	{	}
	//enter the edj list
	//fills: edjes, adjacency
	graph(int V, int E, bool dir = false, bool wei = false) :
		dir(dir), wei(wei), V(V), E(E), AdjMatr(0), Cond(0), R_2(0)
	{
		edges = vector<Edge>(E);
		for(int i = 0; i < E; i++)
		{
			edges[i].get(wei);
		}
	}

	//Ford-Bellman
	bool has_neg_cycle() // finds a negative cycle
	{
		if (V == 1)
		{
			return false;
		}
		int* dist = new int[V];
		for (int i = 1; i < V; i++)
		{
			dist[i] = numeric_limits<int>::max();
		}
		dist[0] = 0;
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < E; j++)
			{
				if (dist[edges[j].v2] > dist[edges[j].v1] + edges[j].w)
				{
					if (i == V - 1)
					{
						delete[] dist;
						return true;
					}
					dist[edges[j].v2] = dist[edges[j].v1] + edges[j].w;
				}
			}
		}
		delete[] dist;
		return false;
	}


	int V, E;
	bool dir;						//directed
	bool wei;						//weight
private:
	vector<Edge> edges;				//edjes list
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

	//			A*			//
	int** R_2;		// 
};

int main()
{
	int n;
	int V, E;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> V >> E;
		graph universe(V, E, true, true);
		if (universe.has_neg_cycle())
		{
			cout << "possible" << endl;
		}
		else
		{
			cout << "not possible" << endl;
		}
	}

    return EXIT_SUCCESS; 
}