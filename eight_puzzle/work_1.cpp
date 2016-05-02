#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <string.h>
#include <algorithm>
#include <stdio.h>

#define TIME
//#define DEBUG
#define RUN_RESULT
//#define RANDOM
//#define DIAMETER

#if defined(TIME) || defined(DEBUG) || defined(RANDOM)
#	include <time.h>
#	if defined(RANDOM) || defined(DEBUG)
#		include <stdlib.h>
#		ifndef _WIN32
#			include <unistd.h>
#       else
#           include <windows.h>
#		endif
#		ifdef DEBUG
#			include <assert.h>
#		endif
#	endif
#endif

#define INF 0x3f3f3f3f

#ifndef _WIN32
#	define TEXT_RED "\033[1;31m"
#	define TEXT_GREEN "\033[1;32m"
#	define TEXT_BLUE "\033[1;34m"
#	define TEXT_NORMAL "\033[0m"
#else
#	define TEXT_RED ""
#	define TEXT_GREEN ""
#	define TEXT_BLUE ""
#	define TEXT_NORMAL ""
#endif

#define LAYOUT_NONE 0
#define LAYOUT_LIST 2
#define LAYOUT_TABLE 3

using namespace std;

static int layout = LAYOUT_TABLE;

/* definition of class state start */
class state {
	int8_t p[10];
public:
	int pi, depth;

	state& operator = (int a[])
	{
		int i;
#ifdef DEBUG
		int check[10] = {0};
		for(i = 0; i < 9; i++)
		{
			assert(a[i] >= 0 && a[i] <= 8);
			check[a[i]] ++;
			assert(check[a[i]] <= 1);
		}
#endif
		for(i = 1; i <= 9; i++)
		{
			p[i] = a[i - 1];
			if(a[i - 1] == 0)
			{
				p[0] = i;
			}
		}
		return *this;
	}

	bool operator == (const state y) const
	{
		int i;
		for(i = 1; i <= 9; i++)
		{
			if(p[i] != y.p[i])
				return false;
		}
		return true;
	}

	bool operator < (const state y) const
	{
		int i;
		for(i = 1; i <= 9; i++)
		{
			if(p[i] < y.p[i])
				return true;
			else if(p[i] > y.p[i])
				return false;
		}
		return false;
	}

	int operator - (const state y)
	{
		int i, dist = 0;
		int a[9], b[9];

		for(i = 1; i <= 9; i++)
		{
			a[p[i]] = i - 1;
			b[y.p[i]] = i - 1;
		}
		for(i = 0; i < 9; i++)
		{
			dist += abs(a[i] % 3 - b[i] % 3) + abs(a[i] / 3 - b[i] / 3);
		}

		return dist;
	}

	bool operator + (const state y)
	{
		int i, j;
		int rx = 0, ry = 0;
		for(i = 1; i <= 9; i++)
			for(j = i + 1; j <= 9; j++)
			{
				if(p[i] && p[j] && p[i] > p[j])
					rx ++;
				if(y.p[i] && y.p[j] && y.p[i] > y.p[j])
					ry ++;
			}
		return ((rx ^ ry) & 1);
	}

	int8_t& operator [] (const int index)
	{
#ifdef DEBUG
		assert(index >= 0 && index <= 9);
#endif
		return p[index];
	}

	int adjnum()
	{
		if(p[0] == 5)
			return 4;
		if((p[0] & 1) ^ 1)
			return 3;
		return 2;
	}

	state* adj()
	{
		int index = p[0];
#ifdef DEBUG
		assert(index >= 1 && index <= 9);
#endif
		static state m[4];
#define Generate_Code(cond, b, c) \
		do { \
			if(cond) \
				m[b][0] = -1; \
			else \
			{ \
				m[b] = *this; \
				m[b][0] = index + (c); \
				swap(m[b][index], m[b][index + (c)]); \
			} \
		} while(0)

		Generate_Code(index <= 3, 0, -3);
		Generate_Code(index >= 7, 1, +3);
		Generate_Code(index % 3 == 1, 2, -1);
		Generate_Code(index % 3 == 0, 3, +1);

		return m;
	}

	friend ostream & operator << (ostream & of, state x);

};

ostream & operator << (ostream & of, state x)
{
	if(layout == LAYOUT_LIST)
	{
		printf("{%d, %d, %d, %d, %d, %d, %d, %d, %d}\n", x.p[1], x.p[2], x.p[3], x.p[4], x.p[5], x.p[6], x.p[7], x.p[8], x.p[9]);
		return of;
	}
	else if(layout == LAYOUT_TABLE)
	{
		printf("+---+---+---+\n");
		printf("| %d | %d | %d |\n", x.p[1], x.p[2], x.p[3]);
		printf("+---+---+---+\n");
		printf("| %d | %d | %d |\n", x.p[4], x.p[5], x.p[6]);
		printf("+---+---+---+\n");
		printf("| %d | %d | %d |\n", x.p[7], x.p[8], x.p[9]);
		printf("+---+---+---+\n");
	}
	return of;
}

/* definition of class state end */

static state dst;

struct node {
	state qt;
	node(state x)
	{
		qt = x;
	}
};

bool operator < (const node x, const node y)
{
	int t1 = (dst - x.qt) + x.qt.depth;
	int t2 = (dst - y.qt) + y.qt.depth;
	if(t1 != t2)
		return t1 < t2;
	else
		return x.qt < y.qt;
}

#ifdef RANDOM

void random_state(int a[])
{
	int i;
	for(i = 0; i < 9; i++)
		a[i] = i;
	for(i = 0; i < 9; i++)
	{
		int fuck = rand() % (9 - i) + i;
		swap(a[i], a[fuck]);
	}
}

#endif

void print_result(const char* solution, bool result, int depth, int dist, int nodes)
{
#ifdef RUN_RESULT
	cout << solution << " search completed ...\n";
	cout << "search result : " << (result ? TEXT_GREEN"succeess\n"TEXT_NORMAL : TEXT_RED"fail\n"TEXT_NORMAL);
	cout << "search depth (length of path) : " << depth << '\n';
	cout << "distance between s and t : " << dist << '\n';
	cout << "nodes been search : " << nodes << "\n\n";
#endif
}

static int fact[10];
static bool vis[500000];

void init()
{
	int i;
	fact[0] = 1;
	for(i = 1; i <= 9; i++)
	{
		fact[i] = fact[i - 1] * i;
	}
}

int h(state x)
{
	/* hash the state x to a interger */
	int invert[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i, j;
	for(i = 2; i <= 9; i ++)
	{
		for(j = 1; j < i; j ++)
		{
			if(x[j] > x[i])
			{
				invert[i] ++;
			}
		}
	}

	int ret = 0;
	for(i = 1; i <= 9; i++)
	{
		ret += invert[i] * fact[i - 1];
	}
	return ret;
}

void bfs(state t, state s)
{
	int i, tmp = -1;
	vector<state> table;
	queue<state> q;
	memset(vis, 0, sizeof(vis));
	q.push(s);
	while(!q.empty())
	{
		state u = q.front();
		vis[h(u)] = true;
		q.pop();
		table.push_back(u);
		tmp ++;
		if(u == t)
		{
			int fuck = tmp, depth = 0;
			cout << "bfs(minimum) path : \n";
			while(!(table[fuck] == s))
			{
				cout << table[fuck];
				fuck = table[fuck].pi;
				depth ++;
			}
			cout << table[fuck];
			print_result("bfs", true, depth, (s - t), table.size());
			return;
		}
		state *adj = u.adj();
		for(i = 0; i < 4; i++)
		{
			if(adj[i][0] != -1 && !vis[h(adj[i])])
			{
				adj[i].pi = tmp;
				q.push(adj[i]);
			}
		}
	}
	print_result("bfs", false, -1, (s - t), table.size());
	return;
}

void A_star(state t, state s)
{
	int i, tmp = -1;
	set<node> q;
	memset(vis, 0, sizeof(vis));
	vector<state> table;
	s.depth = 0;
	q.insert(node(s));
	while(!q.empty())
	{
		node u = *(q.begin());
		q.erase(q.begin());
		vis[h(u.qt)] = true;
		table.push_back(u.qt);
		tmp ++;
		if(u.qt == t)
		{
			int rpg = tmp, depth = 0;
			cout << "A star path : \n";
			while(!(table[rpg] == s))
			{
				cout << table[rpg];
				rpg = table[rpg].pi;
				depth ++;
			}
			cout << table[rpg];
			print_result("A star", true, depth, (s - t), table.size());
			return;
		}

		state *adj = u.qt.adj();
		for(i = 0; i < 4; i++)
		{
			if(adj[i][0] != -1)
			{
				set<node>::iterator it = q.find(node(adj[i]));
				if(it != q.end())
				{
					if(node(adj[i]) < *it)
					{
						node fuck = *it;
						fuck.qt.pi = tmp;
						fuck.qt.depth = table[tmp].depth + 1;
						q.erase(it);
						q.insert(fuck);

						/*it->qt.pi = tmp;*/
					}
				}

				if(!vis[h(adj[i])])
				{
					node fuck = node(adj[i]);
					fuck.qt.pi = tmp;
					fuck.qt.depth = table[tmp].depth + 1;
					q.insert(fuck);
				}
			}
		}
	}
	return;
}

static set<state> black;
static int dfs_diam = 31;
static int dfs_depth = 0;
static int dfs_nodes = 0;

bool dfs_visit(state s, state t)
{
	int i;
	black.insert(s);
	if(s.depth > dfs_diam)
		return false;
	if(s == t)
	{
		cout << t;
		dfs_depth = s.depth;
		return true;
	}
	state *tadj = s.adj();
	state adj[4] = {tadj[0], tadj[1], tadj[2], tadj[3]};
	for(i = 0; i < 16; i++)
	{
		int tmp = rand() % 4;
		if(adj[tmp][0] != -1 && black.find(adj[tmp]) == black.end())
		{
			adj[tmp].depth = s.depth + 1;
			dfs_nodes ++;
			if(dfs_visit(adj[tmp], t))
			{
				cout << s;
				return true;
			}
		}
	}
	return false;
}

void dfs(state s, state t)
{
	int i;
	for(i = 0; i < 20; i++)
	{
		state ts = s;
		if(dfs_visit(ts, t))
		{
			print_result("dfs", true, dfs_depth, (s - t), dfs_nodes);
			break;
		}
		dfs_diam *= 2;
//		dfs_nodes = 0;
		black.clear();
	}
	return;
}

#ifdef DIAMETER

pair<state, int> diameter(state s, state t)
{
	int i, tmp = -1, max = -1;
	state inf;
	queue<state> q;
	set<state> vis;
	vector<state> table;
	s.depth = 0;
	q.push(s);
	while(!q.empty())
	{
		state u = q.front();
		vis.insert(u);
		q.pop();
		table.push_back(u);
		tmp ++;
		state *adj = u.adj();
		for(i = 0; i < 4; i++)
		{
			if(adj[i][0] != -1 && vis.find(adj[i]) == vis.end())
			{
				adj[i].pi = tmp;
				adj[i].depth = table[tmp].depth + 1;
				if(adj[i].depth > max)
				{
					max = adj[i].depth;
					inf = adj[i];
				}
				q.push(adj[i]);
			}
		}
	}
	return make_pair(inf, max);
}

#endif

#ifdef TIME
void calc_time(void (* func)(state x, state y), state x, state y)
{
	int t = clock();
	func(x, y);
	cout << "time elapsed : " << clock() - t << "us\n\n";
}
#endif

int run_algorithm()
{
#ifdef RANDOM
	int a[9], b[9];
	random_state(a);
	random_state(b);
	cout << TEXT_BLUE"randomly generate src and dst state ...\n\n"TEXT_NORMAL;
#else
	int a[] = {6, 4, 0, 8, 2, 1, 7, 5, 3};
	int b[] = {3, 7, 4, 0, 1, 8, 5, 2, 6};
#endif

	state x, y;
	x = a;
	y = b;
	dst = x;
	x.depth = dst.depth = 0;
#ifdef DIAMETER
	pair<state, int> ans = diameter(x, y);
	int diam = diameter(ans.first, y).second;
	cout << "diameter of the graph : " << diam << "\n";
#endif

	cout << "source state :\n" << x << "destination state : \n" << y << endl;
	if(x + y)
	{
		cout << TEXT_RED"the destination may be unreachable for source state ...\n\n"TEXT_NORMAL;
		return -1;
	}
	else
	{
#ifdef TIME
		calc_time(A_star, x, y);
		calc_time(bfs, x, y);
		calc_time(dfs, x, y);
#else
		A_star(x, y);
		bfs(x, y);
		dfs(x, y);
#endif
	}

	return 0;
}

int main()
{
	init();
#ifdef RANDOM
	srand(time(NULL));
#endif
	while(run_algorithm() < 0)
	{
		cout << TEXT_RED"regenerate again ...\n\n"TEXT_NORMAL;
#ifdef RANDOM
#	ifndef _WIN32
		sleep(1);
#	else
		Sleep(1);
#	endif
#else
		break;
#endif
	}
	return 0;
}
