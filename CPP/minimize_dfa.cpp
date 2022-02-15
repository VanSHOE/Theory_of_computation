// automata_q3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
using namespace std;



void dfs(vector<vector<int>> &to_dfs, int node, int parent, vector<bool> &visited)
{
	if (visited[node]) return;
	visited[node] = true;
	for (auto x : to_dfs[node])
	{
		dfs(to_dfs, x, node, visited);
	}
}
int main(int argc, char* argv[])
{
	if (argc != 3) return 1;
	int n, t, a;
	unordered_set<int> accept;
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
	cin >> n >> t >> a;
	
	for (int i = 0; i < a; i++)
	{
		int q;
		cin >> q;
		accept.insert(q);
	}
	vector<vector<int>> to(n, vector<int>(26, -1));
	vector<vector<int>> to_dfs(n, vector<int>());
	unordered_set<int> alphabets;
	for (int i = 0; i < t; i++)
	{
		int u, v;
		char x;
		scanf("%d %c %d", &u, &x, &v);
		to[u][x - 'a'] = v;
		alphabets.insert(x - 'a');
		to_dfs[u].push_back(v);
	}
	
	vector<bool> reachable(n, false);
//  reachable[0] = true;
	dfs(to_dfs, 0, -1, reachable);
	set<int> valid;
	for (int i = 0; i < n; i++)
	{
		if (reachable[i]) valid.insert(i);
	}
	vector<int> vnodes;
	vector<int> av;
	for (auto x : valid)
	{
		vnodes.push_back(x);
	}
	for (auto x : alphabets)
	{
		av.push_back(x);
	}
	vector<vector<int>> nt(n + 1, vector<int>(alphabets.size()));

	bool need_trap = false;
	
	for (int i = 0; i < vnodes.size(); i++)
	{
		for (int j = 0; j < alphabets.size(); j++)
		{
			nt[n][j] = n;
			nt[vnodes[i]][j] = to[vnodes[i]][av[j]];
			if (valid.find(nt[vnodes[i]][j]) == valid.end() || nt[vnodes[i]][j] == -1)
			{
				need_trap = true;
				nt[vnodes[i]][j] = n;
			}
		}
	}

	vector<vector<int>> sets(2);
	vector<int> pos(n + 1, -1);

	for (int i = 0; i < vnodes.size(); i++)
	{
		if (accept.find(vnodes[i]) != accept.end())
		{
			sets[1].push_back(vnodes[i]);
			pos[vnodes[i]] = 1;
		}
		else
		{
			sets[0].push_back(vnodes[i]);
			pos[vnodes[i]] = 0;
		}
	}
	if (need_trap)
	{
		sets[0].push_back(n);
		pos[n] = 0;
	}
	bool pd = true;
	while (pd)
	{
		pd = false;
		/*
		for (int i = 0; i < sets.size(); i++)
		{
			for (int j = 0; j < sets[i].size(); j++)
			{
				cout << sets[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl << endl;
		*/
		vector<int> o_pos = pos;
		for (int i = 0; i < sets.size(); i++)
		{
			for (int j = 1; j < sets[i].size(); j++)
			{
				bool need_new = true;
				for (int k = 0; k < sets.size(); k++)
				{
					// checking jth item of ith set with kth set
					if ((accept.find(sets[i][j]) == accept.end() && accept.find(sets[k][0]) == accept.end())
						|| (accept.find(sets[i][j]) != accept.end() && accept.find(sets[k][0]) != accept.end()))
					{
						bool eq = true;
						for (int l = 0; l < av.size(); l++)
						{
							if (o_pos[nt[sets[k][0]][l]] != o_pos[nt[sets[i][j]][l]])
							{
								if (i == k) pd = true;
								eq = false;
								break;
							}
						}
						if (eq)
						{
							need_new = false;
							if (i == k) break;
							sets[k].push_back(sets[i][j]);
							pos[sets[i][j]] = k;
							sets[i].erase(sets[i].begin() + j);
							j--;
							break;
						}
					}
				}
				if (need_new)
				{
					sets.push_back(vector<int>());
					sets[sets.size() - 1].push_back(sets[i][j]);
					pos[sets[i][j]] = sets.size() - 1;
					sets[i].erase(sets[i].begin() + j);

					j--;
				}

				// ith set's jth element
			}
		}

	}
	/*
	for (int i = 0; i < sets.size(); i++)
	{
		for (int j = 0; j < sets[i].size(); j++)
		{
			cout << sets[i][j] << " ";
		}
		cout << endl;
	}
	*/
	if (need_trap)
	{
		int td = pos[n];
		
		for (auto x : sets[pos[n]])
		{
			pos[x] = -1;
		}
		for (int i = td + 1; i < sets.size(); i++)
		{
			for (auto x : sets[i])
			{
 				pos[x]--;
			}
		}
		sets.erase(sets.begin() + td);
	}
	vector<vector<int>> ftt(sets.size(), vector<int>(av.size()));
	int new_k = 0;
	unordered_set<int> new_a;
	for (int i = 0; i < sets.size(); i++)
	{
		if (accept.find(sets[i][0]) != accept.end())
		{
			new_a.insert(i);
		}
		for (int j = 0; j < av.size(); j++)
		{
			int to_go = nt[sets[i][0]][j];
			ftt[i][j] = pos[to_go];
			if (pos[to_go] != -1)
			{
				new_k++;
			}
		}
	}
	//cout << endl << endl << endl;
	//cout<<"________________________________________________________\n";
	cout << sets.size() << " " << new_k << " " << new_a.size() << endl;
	for (auto x : new_a)
	{
		cout << x << " ";
	}
	cout << endl;
	for (int i = 0; i < sets.size(); i++)
	{
		for (int j = 0; j < av.size(); j++)
		{
			if(ftt[i][j] != -1)
				cout << i << " " << (char)(av[j] + 'a') << " " << ftt[i][j] << endl;
		}
	}
	/*
	cout << "________________________________________________________\n";

	cout << "#states\n";
	for (int i = 0; i < sets.size(); i++)
	{
		cout << i << endl;
	}
	cout << "#initial\n0\n#accepting\n";
	for (auto x : new_a)
	{
		cout << x << endl;
	}
	cout << "#alphabet\n";
	for (char x : av)
	{
		cout <<(char)(x + 'a') << endl;
	}
	cout << "#transitions\n";
	for (int i = 0; i < sets.size(); i++)
	{
		for (int j = 0; j < av.size(); j++)
		{
			cout << i << ":" << (char)('a' + j) << ">" << ftt[i][j] << endl;
		}
	}
	*/
}

