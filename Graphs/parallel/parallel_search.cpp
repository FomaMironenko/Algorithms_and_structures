//Parallel search starts on line 105
#include "pch.h"
#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <boost\lockfree\queue.hpp>
#include <boost\random.hpp>
#pragma warning(disable: 4996)
using namespace std;

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


long* to(long i, int wei = 1)
{
	long* New = new long[2];
	New[0] = i;
	New[1] = wei;
	return New;
}


struct graph
{
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0), R_2(0)
	{	}


	//for an empty graph created by graph()
	void rand_gen(int n)
	{
		boost::random::mt19937 gen{ static_cast<std::uint32_t>(time(NULL)) };
		V = n + gen() % (n * 2);
	
		adjacency = vector<vector<long*>>(V);
		E = 10 * n + gen() % (n * 20);
	    printf("Generating the graph of %d vertexes and %d edjes", V, E);
		int v1, v2;
		for (int i = 0; i < E; i++)
		{
			v1 = gen() % V;
			v2 = gen() % V;
			if (v1 != v2)
			{
				adjacency[v1].push_back(to(v2));
				adjacency[v2].push_back(to(v1));
			}
		}
		cout << "\r                                                                      ";
		cout << "\rV = " << V << " E = " << E << endl;
	}

	
	//////////////////////////////// FINDING PATHS ////////////////////////////////
	void BFS(int a, int b, int* from)
	{
		queue<int> Q;
		Q.push(a);
		int cur;
		while (!Q.empty())
		{
			cur = Q.front(); Q.pop();
			if (cur == b)
				return;
			for (int i = 0; i < adjacency[cur].size(); i++)
			{
				if (from[adjacency[cur][i][0]] == -1)
				{
					from[adjacency[cur][i][0]] = cur;
					Q.push(adjacency[cur][i][0]);
				}
			}
		}
	}
	float search(int v1, int v2, int* from)
	{
		v1--; v2--;
		stopwatch watch;
		watch.set();
		BFS(v1, v2, from);
		return watch.get();
	}


	void parallel_BFS(int b, boost::lockfree::queue<int>* Q, int* from, atomic<bool>* found)
	{
		int cur;
		while (!*found && !Q->empty())
		{
			if (!(*found))
				Q->pop(cur);
			if (!*found && cur == b)
			{
				*found = true;
				return;
			}
			for (int i = 0; !*found && i < adjacency[cur].size(); i++)
			{
				if (!(*found) && from[adjacency[cur][i][0]] == -1)
				{
					from[adjacency[cur][i][0]] = cur;
					Q->push(adjacency[cur][i][0]);
				}
			}
		}
	}
	float parallel_search(int v1, int v2, int n_threads, int* from)
	{
		v1--; v2--;

		thread* th = new thread[n_threads];
		boost::lockfree::queue<int> Q(0);
		Q.push(v1);
		atomic<bool> found;
		found = false;
		stopwatch watch;

		watch.set();
		for (int i = 0; i < n_threads; i++)
		{
			th[i] = thread(&graph::parallel_BFS, this, v2, &Q, from, &found);
		}
		for (int i = 0; i < n_threads; i++)
		{
			th[i].join();
		}
		this_thread::yield();
		return watch.get();
	}

/////////////// TESTS ///////////////
	bool same_way(int cur, int a, int* from, int* from_par)
	{
		if (a == cur)
			return true;
		if (from[cur] == -1 || from[cur] != from_par[cur])
			return false;
		return same_way(from[cur], a, from, from_par);
	}
	bool correct_way(int cur, int a, int* from)
	{
		if (cur == a)
			return true;
		if (from[cur] == -1 || from[cur] > adjacency.size())
		{
			return false;
		}
		for (int i = 0; i < adjacency[from[cur]].size(); i++)
		{
			if (cur == adjacency[from[cur]][i][0])
				return correct_way(from[cur], a, from);
		}
		return false;
	}
/////////////////////////////////////////////

	void compare(int n_threads, int attempts)
	{
		int v1, v2;
		srand(time(NULL));
		float av_par = 0;//allel
		float av_usu = 0;//al
		int mistakes = 0;


		for (int k = 0; k < attempts; k++)
		{
			cout << "\r                        ";
			cout << "\rGenerating cases: " << k << "/" << attempts;
			int* from = new int[V];
			for (int i = 0; i < V; i++)
				from[i] = -1;
			int* from_par = new int[V];
			for (int i = 0; i < V; i++)
				from_par[i] = -1;

			boost::random::mt19937 gen{ static_cast<std::uint32_t>(time(NULL)) };
			v1 = 1 + gen() % V;
			v2 = 2 + gen() % V;

			av_par += parallel_search(v1, v2, n_threads, from_par);
			av_usu += search(v1, v2, from);

			if (from[v2 - 1] != -1 && !correct_way(v2 - 1, v1 - 1, from))
				mistakes += 1;
			delete[] from_par;
			delete[] from;
		}
		cout << "\r                          ";
		cout << "\r\nFailed:   " << mistakes << "/" << attempts << endl;
		cout << "threads:  " << n_threads << endl;
		printf("\nAverage time of parallel: %.3f\n", av_par / attempts);
		printf("Average time of usual   : %.3f\n", av_usu / attempts);
	}


	long V, E;
	bool dir;						//directed
	bool wei;						//weight
private:
	vector<int*> edjes;				//edjes list
	vector<vector<long*>> adjacency;	//adjacency list

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
	graph a;
	a.rand_gen(10000);
	a.compare(10, 30); // compare(n_threads, attempts)
	cout << endl << endl;

	graph b;
	b.rand_gen(100000);
	b.compare(40, 10); // compare(n_threads, attempts)

	return EXIT_SUCCESS; 
}