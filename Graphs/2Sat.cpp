#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#pragma warning(disable: 4996)

using namespace std;

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
	graph() : V(0), E(0), dir(false), wei(false), AdjMatr(0), Cond(0)
	{	}
	//4 2-sat
	//fills: vars, adjacency
	graph(bool sat) : V(0), E(0), dir(true), wei(false),
					  adjacency(vector<vector<int*>>(0)),
					  vars(vector<string>(0)), Cond(0)
	{
		string A; 
		string B;
		int iA, iB, i_A, i_B;
		cout << "Enter the expresion with any letters. In the end of input enter ctrl+z on Windows.\n";
		cout << "Example of input:\na -b\nU -a\nb -c\n-c -U\nmeans (a\\/-b) /\\ (U\\/-a) /\\ (b\\/-c) /\\ (-c\\/-U)\n\nUour input:\n";
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
			if (vars[2*i][0] != '-')
				cout << vars[2*i] << " = " << int(values[2*i]) << endl;
			else
				cout << vars[2*i + 1] << " = " << int(values[2*i + 1]) << endl;
		}
	}
	///////////////////////////////////////////////////////////////////////




	//////////////////////////////// OUTPUT ////////////////////////////////
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
};



int main()
{
	graph a(true);
	a._2_sat();
	return 0;
}