// regex2nfa.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <ctype.h>
#include <set>
using namespace std;

int p(char c)
{
	if (c == '*') return 3;
	if (c == '.') return 2;
	if (c == '+') return 1;
	return 0;
}

class NFA
{
public:
	int n, k, a;
	vector<int> accept;
	vector<vector<vector<int>>> trans;
	NFA(int input)
	{
		a = 1;
		k = 0;
		n = input;
		trans.assign(n, vector<vector<int>>(27, vector<int>()));
	}
};

void Star(NFA& i1, NFA& q);

void Concat(NFA& i1, NFA& q, NFA& i2);

void Union(NFA& i1, NFA& q, NFA& i2);

void i2p(std::string& regex, std::string& postfix);

int main(int argc, char* argv[])
{
	if (argc != 3) return 1;
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
	string pregex;
	cin >> pregex;
	pregex.insert(0, 1, '(');
	pregex.insert(pregex.end(), 1, ')');

	string regex;
	regex.append(1, pregex[0]);
	set<char> left;
	set<char> right;
	set<int> alphabets;
	for (char j = 'a'; j <= 'z'; j++)
	{
		left.insert(j);
		right.insert(j);
	}
	left.insert(')');
	left.insert('*');
	right.insert('(');
	for (int i = 1; i < pregex.size(); i++)
	{
		if (pregex[i] == ' ') continue;
		/*
		if (islower(pregex[i - 1]) && islower(pregex[i]))
		{
			regex.append(1, '.');
		}
		else if (pregex[i - 1] == ')' && islower(pregex[i]))
		{
			regex.append(1, '.');
		}
		else if (islower(pregex[i - 1]) && pregex[i] == '(')
		{
			regex.append(1, '.');
		}
		else if (pregex[i - 1] == ')' && pregex[i] == '(')
		{
			regex.append(1, '.');
		}
		*/
		if (left.find(pregex[i - 1]) != left.end() && right.find(pregex[i]) != right.end())
		{
			regex.append(1, '.');
		}
		regex.append(1, pregex[i]);
	}
	//cout << regex;
	string postfix;
	i2p(regex, postfix);
	vector<NFA> fs;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (postfix[i] >= 'a' && postfix[i] <= 'z')
		{
			NFA q(2);
			q.accept.push_back(1);
			q.k++;
			q.trans[0][postfix[i] - 'a'].push_back(1);
			fs.push_back(q);
			alphabets.insert(postfix[i]);
		}
		else if (postfix[i] == '+')
		{
			NFA i2 = fs.back();
			fs.pop_back();
			NFA i1 = fs.back();
			fs.pop_back();
			NFA q(2 + i1.n + i2.n);

			Union(i1, q, i2);

			fs.push_back(q);
		}
		else if (postfix[i] == '.')
		{
			NFA i2 = fs.back();
			fs.pop_back();
			NFA i1 = fs.back();
			fs.pop_back();
			NFA q(i1.n + i2.n);

			Concat(i1, q, i2);

			fs.push_back(q);
		}
		else if (postfix[i] == '*')
		{
			NFA i1 = fs.back();
			fs.pop_back();
			NFA q(i1.n + 2);

			Star(i1, q);

			fs.push_back(q);
		}
	}
	NFA ans = fs.back();
	ans.k = 0;
	for (int i = 0; i < ans.n; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			for (auto x : ans.trans[i][j])
			{
				ans.k++;
			}
		}
	}
	cout << ans.n << " " << ans.k << " " << ans.a << endl;
	cout << ans.n - 1 << endl;
	for (int i = 0; i < ans.n; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			for (auto x : ans.trans[i][j])
			{
				if (j != 26)
					cout << i << " " << (char)('a' + j) << " " << x << endl;
				else
					cout << i << " E " << x << endl;
			}
		}
	}
	/*
	cout << "#states\n";
	for (int i = 0; i < ans.n; i++)
	{
		cout << i << endl;
	}
	cout << "#initial\n0\n#accepting\n" << ans.n - 1 << endl;
	cout << "#alphabet\n";
	for (char x : alphabets)
	{
		cout << x << endl;
	}
	cout << "#transitions\n";
	for (int i = 0; i < ans.n; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			for (auto x : ans.trans[i][j])
			{
				if (j != 26)
					cout << i << ":" << (char)('a' + j) << ">" << x << endl;
				else
					cout << i << ":$>" << x << endl;
			}
		}
	}
	*/
	return 0;
}

void i2p(std::string& regex, std::string& postfix)
{
	vector<char> st;
	for (int i = 0; i < regex.size(); i++)
	{

		if (regex[i] == '(')
		{
			st.push_back('(');
		}
		else if (regex[i] >= 'a' && regex[i] <= 'z')
		{
			postfix.append(1, regex[i]);
		}
		else if (regex[i] == ')')
		{
			while (st.back() != '(')
			{
				postfix.append(1, st.back());
				st.pop_back();
			}
			st.pop_back();
		}
		else
		{
			while (st.size() && p(regex[i]) <= p(st.back()))
			{
				postfix.append(1, st.back());
				st.pop_back();
			}
			st.push_back(regex[i]);
		}
	}
	//cout << endl << postfix << endl;
}

void Union(NFA& i1, NFA& q, NFA& i2)
{
	for (int j = 0; j < i1.n; j++)
	{
		for (int k = 0; k < 27; k++)
		{
			for (auto x : i1.trans[j][k])
			{
				q.trans[1 + j][k].push_back(x + 1);
			}
		}
	}
	for (int j = 0; j < i2.n; j++)
	{
		for (int k = 0; k < 27; k++)
		{
			for (auto x : i2.trans[j][k])
			{
				q.trans[1 + i1.n + j][k].push_back(x + 1 + i1.n);
			}
		}
	}
	q.k = i1.k + i2.k + 4;
	q.trans[0][26].push_back(1);
	q.trans[0][26].push_back(1 + i1.n);
	q.trans[i1.n][26].push_back(q.n - 1);
	q.trans[i1.n + i2.n][26].push_back(q.n - 1);
}

void Concat(NFA& i1, NFA& q, NFA& i2)
{
	for (int j = 0; j < i1.n; j++)
	{
		for (int k = 0; k < 27; k++)
		{
			for (auto x : i1.trans[j][k])
			{
				q.trans[j][k].push_back(x);
			}
		}
	}
	for (int j = 0; j < i2.n; j++)
	{
		for (int k = 0; k < 27; k++)
		{
			for (auto x : i2.trans[j][k])
			{
				q.trans[j + i1.n][k].push_back(x + i1.n);
			}
		}
	}
	q.trans[i1.n - 1][26].push_back(i1.n);
	q.k = i1.k + i2.k + 1;
}

void Star(NFA& i1, NFA& q)
{
	for (int j = 0; j < i1.n; j++)
	{
		for (int k = 0; k < 27; k++)
		{
			for (auto x : i1.trans[j][k])
			{
				q.trans[1 + j][k].push_back(x + 1);
			}
		}
	}
	q.k += 4;
	q.trans[0][26].push_back(1);
	q.trans[0][26].push_back(q.n - 1);
	q.trans[q.n - 2][26].push_back(1);
	q.trans[q.n - 2][26].push_back(q.n - 1);
}
