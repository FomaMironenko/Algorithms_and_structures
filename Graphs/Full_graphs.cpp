#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <math.h>
#include <time.h>
#pragma warning(disable: 4996)

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
			return 0;
		if (size[y] < size[x])
			swap(x, y);
		parent[x] = y;
		size[y] = max(size[y], 1 + size[x]);
		return 1;
	}

	int* parent;
	int* size;
};


string neg(string a)
{
	string tmp = a;
	if (a[0] == '-')
		return tmp.erase(0, 1);
	else
		return "-" + tmp;
}
int neg(int i)
{
	if (i % 2 == 0)
		return i + 1;
	else
		return i - 1;
}

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
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0), R_2(0)
	{	}
	//enter the edj list
	//fills: edjes, adjacency
	graph(int V, bool dir = false, bool wei = false, int Ed = -1) : 
		dir(dir), wei(wei), V(V), AdjMatr(0), Cond(0), R_2(0)
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
			if (E == Ed)
				return;
		}
		cin.clear();
	}
	//4 2-sat
	//fills: vars, adjacency
	graph(bool sat) : V(0), E(0), dir(true), wei(false),
		adjacency(vector<vector<int*>>(0)),
		vars(vector<string>(0)), Cond(0), R_2(0)
	{
		string A;
		string B;
		int iA, iB, i_A, i_B;
		while (cin >> A, cin >> B)
		{
			if (find(vars.begin(), vars.end(), A) == vars.end())
			{
				V += 2;
				adjacency.push_back(vector<int*>(0));
				adjacency.push_back(vector<int*>(0));
				vars.push_back(A);
				vars.push_back(neg(A));
			}
			if (find(vars.begin(), vars.end(), B) == vars.end())
			{
				V += 2;
				adjacency.push_back(vector<int*>(0));
				adjacency.push_back(vector<int*>(0));
				vars.push_back(B);
				vars.push_back(neg(B));
			}
			iA = find(vars.begin(), vars.end(), A) - vars.begin();
			iB = find(vars.begin(), vars.end(), B) - vars.begin();
			i_A = find(vars.begin(), vars.end(), neg(A)) - vars.begin();
			i_B = find(vars.begin(), vars.end(), neg(B)) - vars.begin();

			E += 2;
			adjacency[i_A].push_back(to(iB)); //-A => B
			adjacency[i_B].push_back(to(iA)); //-B => A
		}
		cin.clear();
	}

	//for an empty graph created by graph()
	float Dist(int u, int v)
	{
		return sqrt(pow(float(R_2[u][0]) - R_2[v][0], 2) + pow(R_2[u][1] - R_2[v][1], 2));
	}

	void rand_gen(int n)
	{
		srand(time(NULL));
		V = n + rand() % 2*n;
		adjacency = vector<vector<int*>>(V);
		E = 2*n + rand() % (10*n);
		R_2 = new int*[V];
		for (int i = 0; i < V; i++)
		{
			R_2[i] = new int[2];
			R_2[i][0] = rand() % (10*n) - 5*n;
			R_2[i][0] = rand() % (10*n) - 5*n;
		}

		int v1, v2, w, d;
		for (int i = 0; i < E; i++)
		{
			v1 = rand() % V;
			v2 = rand() % V;
			if (v1 != v2)
			{
				d = Dist(v1, v2);
				w = d + rand() % (d/2 + 1);
				adjacency[v1].push_back(to(v2, w));
				adjacency[v2].push_back(to(v1, w));
			}
		}
	}

	//fills: AdjMatr
	void set_AdjMatr()
	{
		if (!AdjMatr)
		{
			AdjMatr = new int*[V];
			for (int i = 0; i < V; i++)
			{
				AdjMatr[i] = new int[V];
			}
		}

		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				AdjMatr[i][j] = numeric_limits<int>::max();
			}
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				AdjMatr[i][adjacency[i][j][0]] = adjacency[i][j][1];
			}
		}
	}


	//////////////////////////////// FINDING PATHS ////////////////////////////////
	void BFS(int a, int b, int L, int* from)
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
				if (from[adjacency[cur][i][0]] == -1 && adjacency[cur][i][1] <= L)
				{
					from[adjacency[cur][i][0]] = cur;
					Q.push(adjacency[cur][i][0]);
				}
			}
		}
	}

	void way(int cur, int a, int* from)
	{
		if (from[cur] < 0)
			return;
		if (cur == a)
		{
			cout << a + 1 << " ";
			return;
		}
		way(from[cur], a, from);
		cout << cur + 1 << " ";
	}

	void bad_bank(int a, int b, int L)
	{
		a--; b--;
		int* from = new int[V];
		for (int i = 0; i < V; i++)
			from[i] = -1;
		from[a] = a;
		BFS(a, b, L, from);
		if (from[b] == -1)
		{
			cout << "\nNot found";
			return;
		}
		cout << "\nFound\n";
		way(b, a, from);
	}


	////////// DIJKSTRA //////////
	void dijkstra(int v, int u = 0, bool output = true)
	{
		v--; u--;
		int* dist = new int[V];
		int* parent = new int[V];
		bool* mark = new bool[V];
		//auto cmp = [&dist](int left, int right) 
		//{ 
		//	return dist[left] < dist[right];
		//};
		//std::priority_queue<int, std::vector<int>, decltype(cmp)> verts(cmp);
		//for (int i = 0; i < V; i++)
		//	verts.push(i);

		for (int i = 0; i < V; i++)
		{
			dist[i] = numeric_limits<int>::max();
			mark[i] = false;
		}
		dist[v] = 0; parent[v] = v;
		
		for (int i = 0; i < V; i++)
		{
			int min = 0;
			int M = numeric_limits<int>::max();
			for (int j = 0; j < V; j++)
			{
				if (dist[j] < M && !mark[j])
				{
					min = j;
					M = dist[j];
				}
			}
			mark[min] = true;
			if (min == u)
				goto found;
			for (int j = 0; j < adjacency[min].size(); j++)
			{
				if (dist[adjacency[min][j][0]] > dist[min] + adjacency[min][j][1])
				{
					parent[adjacency[min][j][0]] = min;
					dist[adjacency[min][j][0]] = dist[min] + adjacency[min][j][1];
				}
			}
		}
		for (int i = 0; output && i < V; i++)
		{
			cout << i+1 << ": " << dist[i] << endl;
			way(i, v, parent);
			cout << endl << endl;
		}
		return;
	found:;
		if (output)
		{
			int cur = u;
			cout << dist[u] << endl;
			way(u, v, parent);
		}
	}


	////////// A* //////////
	void Astar(int v, int u, bool output = true)
	{
		v--; u--;
		if (!R_2)
		{
			R_2 = new int*[V];
			cout << "Enter the coordinates";
			for (int i = 0; i < V; i++)
			{
				R_2[i] = new int[2];
				cout << i + 1 << ": ";
				cin >> R_2[i][0] >> R_2[i][1];
			}
		}
		float* h = new float[V];
		int* dist = new int[V];
		int* parent = new int[V];
		bool* mark = new bool[V];

		for (int i = 0; i < V; i++)
		{
			dist[i] = numeric_limits<int>::max();
			h[i] = numeric_limits<float>::max();
			mark[i] = false;
		}
		h[v] = 0; parent[v] = v; dist[v] = 0;

		for (int i = 0; i < V; i++)
		{
			int min = 0;
			float M = numeric_limits<float>::max();
			for (int j = 0; j < V; j++)
			{
				if (h[j] < M && !mark[j])
				{
					min = j;
					M = h[j];
				}
			}
			mark[min] = true;
			if (min == u)
				goto found;
			for (int j = 0; j < adjacency[min].size(); j++)
			{
				if (dist[adjacency[min][j][0]] > dist[min] + adjacency[min][j][1])
				{
					parent[adjacency[min][j][0]] = min;
					dist[adjacency[min][j][0]] = dist[min] + adjacency[min][j][1];
					h[adjacency[min][j][0]] = dist[adjacency[min][j][0]] + Dist(adjacency[min][j][0], u);
				}
			}
		}
	found:;
		if (output)
		{
			int cur = u;
			cout << h[u] << endl;
			way(u, v, parent);
		}
	}

	void compare(int n)
	{
		int v, u;
		srand(time(NULL));
		float avD = 0;
		float avA = 0;
		time_t t;
		for (int i = 0; i < n; i++)
		{
			cout << "Please wait, generating cases: " << i + 1 << "/" << n;
			v = 1 + rand() % V;
			u = 1 + rand() % V;

			t = clock();
			dijkstra(v, u, false);
			avD += (float(clock()) - t) / CLOCKS_PER_SEC;

			t = clock();
			Astar(v, u, false);
			avA += (float(clock()) - t) / CLOCKS_PER_SEC;
			cout << "\r";
		}
		cout << "                                                 " << "\r";
		cout << "V = " << V << ";  E = " << E << endl;
		cout << "Average time of Dijkstra: " << avD / n << endl; 
		cout << "Average time of A*:	   " << avA / n << endl << endl;
	}
	///////////////////////////////////////////////////////////////////////////////


	//////////////////////////////// TOPOLOGY SORT ////////////////////////////////
	void DFS_sort(int cur)
	{
		static bool cycle = false;
		static int T = 1;
		// not notVsited[i] initialises with nonzero value,
		// so notVisited[i] = true
		static int* notVisited = new int[V];
		static int* tOut = new int[V];

		if (!notVisited[cur] && tOut[cur] >= 1)
			return;
		notVisited[cur] = 0;
		for (int j = 0; j < adjacency[cur].size() && !cycle; j++)
		{
			if (notVisited[adjacency[cur][j][0]])
				DFS_sort(adjacency[cur][j][0]);
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
	void sort()
	{
		top_sort.clear();
		for (int i = 0; i < V; i++)
		{
			DFS_sort(i);
		}
	}
	///////////////////////////////////////////////////////////////////////////////


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



	//////////////////////////////// ONLINE BGIDGES ////////////////////////////////
	void online_bridges()
	{
		DSU conn(V);
		DSU biconn(V);
		graph bic_tree;
		vector<int*> bridges(0);

		bic_tree.adjacency = vector<vector<int*>>(V);

		int v1, v2;
		int comp1, comp2;
		int Bcomp1, Bcomp2;
		while (cin >> v1 >> v2)
		{
			v1--; v2--;
			comp1 = conn.Find(v1);
			comp2 = conn.Find(v2);
			Bcomp1 = conn.Find(v1);
			Bcomp2 = conn.Find(v2);

			if (comp1 != comp2)
			{

			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////// 2 SAT ////////////////////////////////
	void _2_sat()
	{
		graph* cond = this->condensation();
		values = new bool[V];
		for (int i = 0; i < V; i++)
			values[i] = false;
		cond->transpose();
		vector<vector<int*>> parents = cond->adjacency;
		cond->transpose();

		//note that vertexes in cond are sorted in order of 
		//decreasing of tOut, hence, in order of topology sort
		for (int i = 0; i < comp_list[0].size(); i++)
		{
			if (component[comp_list[0][i]] == component[neg(comp_list[0][i])])
			{
				cout << "Unsolvable:\n" << vars[comp_list[0][i]] << " => " << vars[neg(comp_list[0][i])];
				cout << "\n" << vars[neg(comp_list[0][i])] << " => " << vars[comp_list[0][i]];
				return;
			}
			values[comp_list[0][i]] = false;
			values[neg(comp_list[0][i])] = true;
		}

		for (int comp = 1; comp < cond->V; comp++)
		{
			bool prev = false;
			//we have already visited all parenst
			for (int i = 0; i < parents[comp].size(); i++)
			{
				if (values[comp_list[parents[comp][i][0]][0]] == true)
				{
					prev = true;
					break;
				}
			}
			bool cur_val = prev;
			for (int i = 0; i < comp_list[comp].size(); i++)
			{
				if (component[neg(comp_list[comp][i])] < comp)
				{
					cur_val = !values[neg(comp_list[comp][i])];
					break;
				}
			}

			for (int i = 0; i < comp_list[comp].size(); i++)
			{
				if (component[comp_list[comp][i]] == component[neg(comp_list[comp][i])])
				{
					cout << "Unsolvable:\n" << vars[comp_list[comp][i]] << " => " << vars[neg(comp_list[comp][i])];
					cout << "\n" << vars[neg(comp_list[comp][i])] << " => " << vars[comp_list[comp][i]];
					return;
				}
				values[comp_list[comp][i]] = cur_val;
			}
		}

		cout << endl;
		for (int i = 0; i < V / 2; i++)
		{
			if (vars[2 * i][0] != '-')
				cout << vars[2 * i] << " = " << int(values[2 * i]) << endl;
			else
				cout << vars[2 * i + 1] << " = " << int(values[2 * i + 1]) << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////




	//////////////////////////////// OUTPUT ////////////////////////////////
	void print()
	{
		for (int i = 0; i < V; i++)
		{
			cout << i + 1 << " -> ";
			for (int j = 0; j < adjacency[i].size(); j++)
			{
				cout << adjacency[i][j][0] + 1 << ", ";
			}
			cout << endl;
		}
	}

	void print_AdjMatr()
	{
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				printf("%-11d  ", AdjMatr[i][j]);
			}
			cout << endl;
		}
		cout << "\n\n";
	}

	void print_top()
	{
		for (int i = 0; i < top_sort.size(); i++)
			cout << top_sort[i] << " ";
		cout << endl;
	}

	//only after condensation()
	void print_condensation()
	{
		cout << "Condensation\n";
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

	void print_2sat()
	{
		cout << endl;
		for (int i = 0; i < V; i++)
		{
			cout << vars[i] << " -> ";
			for (int j = 0; j < adjacency[i].size(); j++)
				cout << vars[adjacency[i][j][0]] << " ";
			cout << endl << endl;
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

	//			A*			//
	int** R_2;		// 
};


int main()
{

	
	return EXIT_SUCCESS;
}
