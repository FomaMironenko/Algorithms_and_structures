#include "pch.h"
#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <boost\lockfree\queue.hpp>
#include <boost\random.hpp>
#pragma warning(disable: 4996)
using namespace std;

#define adj vector<vector<pair<int, int>>>
#define triple tuple<int, int, int>
#define prior priority_queue<triple, vector<triple>, greater<triple>>
#define MAX_W 200 // max weight of edje in graph

struct DSU
{
	DSU(int V)
	{
		parent = new atomic<int>[V];
		for (int i = 0; i < V; i++)
			Set(i);
	}

	int Set(int x)
	{
		return parent[x] = x;
	}

	int Find(int x)
	{
		if (parent[x] == x)
			return x;
		return parent[x] = Find(parent[x]);
	}

	void HardUnion(int x, int y)
	{
		x = Find(x);
		y = Find(y);
		if (x == y)
		{
			return;
		}
		parent[x] = y;
	}

	atomic<int>* parent;
};


struct stopwatch
{
	stopwatch()
	{	}

	void set()
	{
		sec = clock();
	}
	float get()
	{
		return (float(clock()) - sec) / CLOCKS_PER_SEC;
	}

	time_t sec;
};

struct graph
{
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0), R_2(0)
	{	}

////////////  FOR GRAPH GENERATION  /////////////
	void DFS(int v, bool* visited)
	{
		visited[v] = true;
		for (int i = 0; i < adjacency[v].size(); i++)
		{
			if (!visited[adjacency[v][i].second])
			{
				DFS(adjacency[v][i].second, visited);
			}
		}
	}

	bool check_conn()
	{
		bool* visited = new bool[V];
		for (int i = 0; i < V; i++)
		{
			visited[i] = false;
		}
		DFS(0, visited);
		for (int i = 0; i < V; i++)
		{
			if (!visited[i])
			{
				return false;
			}
		}
		return true;
	}

	void connectization() // объединение компонент связности
	{
		boost::random::mt19937 gen{ static_cast<std::uint32_t>(time(NULL)) };
		bool* visited = new bool[V];
		for (int i = 0; i < V; i++)
		{
			visited[i] = false;
		}
		for (int i = 0; i < V; i++)
		{
			if (!visited[i])
			{
				DFS(i, visited);
				if (i != 0)
				{
					int w = gen() % MAX_W;
					adjacency[i].push_back(pair<int, int>(w, i - 1));
					adjacency[i - 1].push_back(pair<int, int>(w, i));
				}
			}
		}
	}

	// for an empty graph created by graph()
	// generates a random connected graph
	void rand_gen(int n)
	{
		boost::random::mt19937 gen{ static_cast<std::uint32_t>(time(NULL)) };
		V = n + gen() % (n * 2);

		adjacency = adj(V);
		E = 10 * n + gen() % (n * 20);
		printf("Generating the graph of %d vertexes and %d edjes", V, E);
		int v1, v2, w;
		for (int i = 0; i < E; i++)
		{
			v1 = gen() % V;
			v2 = gen() % V;
			w  = gen() % MAX_W + 1;
			if (v1 != v2)
			{
				adjacency[v1].push_back(pair<int, int>(w, v2));
				adjacency[v2].push_back(pair<int, int>(w, v1));
			}
		}
		connectization();
		cout << "\r                                                                      ";
		cout << "\rV = " << V << " E = " << E << endl;
	}

	//generates a random (randomly weighted) full graph
	void rand_full(int n)
	{
		boost::random::mt19937 gen{ static_cast<std::uint32_t>(time(NULL)) };
		V = n + gen() % (n * 2);

		adjacency = adj(V);
		E = V*(V + 1) / 2;
		printf("Generating the graph of %d vertexes and %d edjes", V, E);
		int w;
		for (int v1 = 0; v1 < V; v1++)
		{
			for (int v2 = v1 + 1; v2 < V; v2++)
			{
				w = gen() % MAX_W + 1;
				adjacency[v1].push_back(pair<int, int>(w, v2));
				adjacency[v2].push_back(pair<int, int>(w, v1));
			}
		}
		cout << "\r                                                                      ";
		cout << "\rV = " << V << " E = " << E << endl;
	}

	// for manual input of a graph
	void read(int V, int E)
	{
		this->V = V;
		this->E = E;
		adjacency = adj(V);
		int v1, v2, w;
		for (int i = 0; i < E; i++)
		{
			cin >> v1 >> v2 >> w;
			v1--; v2--;
			if (v1 != v2)
			{
				adjacency[v1].push_back(pair<int, int>(w, v2));
				adjacency[v2].push_back(pair<int, int>(w, v1));
			}
		}
	}


	void Borovka()
	{
		spanning_tree = new graph();
		spanning_tree->adjacency.clear();
		spanning_tree->adjacency = adj(V);
		spanning_tree->V = V;
		DSU forest(V);
		prior* component = new prior[V];
		
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				component[i].push(triple(adjacency[i][j].first, adjacency[i][j].second, i));
			}
		}
		
		while (spanning_tree->E < V - 1)
		{
			for (int i = 0; i < V; i++)
			{
				triple min;
				if (i != forest.Find(i) || component[i].empty())
				{
					continue;
				}
				while (!component[i].empty())
				{
					min = component[i].top(); 
					component[i].pop();
					if (i != forest.Find(get<1>(min)))
					{
						break;
					}
				}
				if (i != forest.Find(get<1>(min)))
				{
					int head = forest.Find(get<1>(min));
					while (!component[head].empty())
					{
						if (forest.Find(get<1>(component[head].top())) != i)
						{
							component[i].push(component[head].top());
						}
						component[head].pop();
					}
					forest.HardUnion(head, i);
					(spanning_tree->adjacency[get<2>(min)]).push_back(pair<int, int>(get<0>(min), get<1>(min)));
					(spanning_tree->adjacency[get<1>(min)]).push_back(pair<int, int>(get<0>(min), get<2>(min)));
					spanning_tree->E++;
				}
			}
		}
	}

	void parallel_fill(int k, int n_threads, prior* component)
	{
		for (int i = k; i < V; i += n_threads)
		{
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				component[i].push(triple(adjacency[i][j].first, adjacency[i][j].second, i));
			}
		}
	}

	void parallel_relaxation(int k, int n_threads, DSU* forest, prior* component)
	{
		for (int i = k; i < V; i += n_threads)
		{
			//check if min not changed
			triple min;
			if (i != forest->Find(i) || component[i].empty())
			{
				continue;
			}
			blck[i].lock();
			//search for the min neighbour
			while (!component[i].empty())
			{
				min = component[i].top();
				component[i].pop();
				if (i != forest->Find(get<1>(min)))
				{
					break;
				}
			}
			blck[i].unlock();

			if (i == forest->Find(i) && i != forest->Find(get<1>(min)))
			{
				blck[i].lock();
				int head = forest->Find(get<1>(min));
				blck[head].lock();
				//blck[head].lock();
				while (head != forest->Find(head))
				{
					blck[head].unlock();
					head = forest->Find(head);
					blck[head].lock();
				}

				while (!component[head].empty())
				{
					if (forest->Find(get<1>(component[head].top())) != i)
					{
						component[i].push(component[head].top());
					}
					component[head].pop();
				}
				forest->HardUnion(head, i);
				(spanning_tree->adjacency[get<2>(min)]).push_back(pair<int, int>(get<0>(min), get<1>(min)));
				(spanning_tree->adjacency[get<1>(min)]).push_back(pair<int, int>(get<0>(min), get<2>(min)));
				spanning_tree->E++;
				blck[i].unlock();
				blck[head].unlock();
			}
		}
	}


	void parallel_Borovka(int n_threads)
	{
		spanning_tree = new graph();
		spanning_tree->adjacency.clear();
		spanning_tree->adjacency = adj(V);
		spanning_tree->V = V;

		DSU forest(V);
		prior* component = new prior[V];
		thread* th = new thread[n_threads];
		for (int k = 0; k < n_threads; k++)
		{
			th[k] = thread(&graph::parallel_fill, this, k, n_threads, component);
		}
		for (int k = 0; k < n_threads; k++)
		{
			th[k].join();
		}
		this_thread::yield();
		blck = new mutex[V];

		while (spanning_tree->E < V - 1)
		{
			for (int k = 0; k < n_threads; k++)
			{
				th[k] = thread(&graph::parallel_relaxation, this, k, n_threads, &forest, component);
			}
			for (int k = 0; k < n_threads; k++)
			{
				th[k].join();
			}
			this_thread::yield();
		}
	}

	long long sum()
	{
		long long sum = 0;
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				sum += adjacency[i][j].first;
			}
		}
		return sum/2;
	}

	void compare(int n_threads)
	{
		stopwatch timer;
		float tmp;

		timer.set();
		Borovka();
		tmp = timer.get();
		printf("\nOrdinary: %.3f;   sum = ", tmp);
		printf("%d\n\n", spanning_tree->sum());

		timer.set();
		parallel_Borovka(n_threads);
		tmp = timer.get();
		printf("Parallel: %.3f;   sum = ", tmp);
		printf("%d\n", spanning_tree->sum());
		printf("tree : %d\n", (int)spanning_tree->check_conn());
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

	graph* get_tree()
	{
		return spanning_tree;
	}
	
	long V, E;
	bool dir;						//directed
	bool wei;						//weight
private:
	vector<int*> edjes;				//edjes list
	adj adjacency;	//adjacency list

	int** AdjMatr;
	int** IncMatr;

	//    TOPOLOGY SORT		//
	vector<int> top_sort;	// sequence of vertexes in order of topology sort

	//    CONDENSATION		//
	int* out;				// time out
	//int* component;			// component of the vertex
	vector<vector<int>> comp_list;	// list of vertexes from source graph for each component
	graph* Cond;			// grahp-condensation

	//        2-SAT			//
	vector<string> vars;	// variables from expression in order of vertexes
	bool* values;			// value for each variable from vars

	//			A*			//
	int** R_2;				//

	//        Borovka		//
	graph* spanning_tree;
	mutex* blck;
};


void manual_graph(graph* gr, int n_threads) // manual input to check correctness
{
	int V, E;
	cin >> V >> E;
	gr->read(V, E);
	gr->parallel_Borovka(n_threads);
	graph* tree = gr->get_tree();
	tree->print();
	cout << "\n";
}


int main()
{
	graph a;
	// to manual input
	//manual_graph(&a, 4);
	

	//or use    rand_full(n)    to generate a random full graph
	a.rand_gen(1200);
	a.compare(6);

	return EXIT_SUCCESS;

}

//4 6
//1 2 2
//1 3 1
//1 4 1
//2 3 3
//2 4 4
//3 4 2
