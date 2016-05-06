#include <stdio.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAXN 100
#define MAXM 10000

struct edge {
	int from, to, next, weight;
	bool vis;
};

class graph {
public:
	int pe, head[MAXN], degree[MAXN];
	edge e[MAXM];

	void clear()
	{
		pe = 0;
		memset(head, -1, sizeof(head));
		memset(degree, 0, sizeof(degree));
	}

	void del(int u, int v)
	{
		int i, prev = head[u];
		if(e[prev].to == v)
			head[u] = e[prev].next;
		for(i = e[prev].next; i != -1; i = e[i].next)
		{
			if(e[i].to == v)
			{
				e[prev].next = e[i].next;
				e[i].next = -1;
				break;
			}
			prev = i;
		}

		prev = head[v];
		if(e[prev].to == u)
			head[v] = e[prev].next;
		for(i = e[head[v]].next; i != -1; i = e[i].next)
		{
			if(e[i].to == u)
			{
				e[prev].next = e[i].next;
				e[i].next = -1;
				break;
			}
			prev = i;
		}
	}

	void add(int u, int v, int w)
	{
		e[pe].from = u;
		e[pe].to = v;
		e[pe].weight = w;
		e[pe].next = head[u];
		head[u] = pe++;
		degree[u]++;

		e[pe].from = v;
		e[pe].to = u;
		e[pe].weight = w;
		e[pe].next = head[v];
		head[v] = pe++;
		degree[v]++;
	}

	void print()
	{
		int i, j;
		for(i = 0; i < MAXN; i ++)
		{
			if(head[i] == -1)
				continue;

			printf("%d:", i);
			for(j = head[i]; j != -1; j = e[j].next)
			{
				printf(" %d", e[j].to);
			}
			printf("\n");
		}
		printf("\n");
	}
};


int n, m, adj[MAXN][MAXN];
graph cg, mintree;

/* code for generate min tree */

int pi[MAXN];

bool cmp(edge a, edge b)
{
	return a.weight < b.weight;
}

int find_set(int s)
{
	if(s != pi[s])
	{
		pi[s] = find_set(pi[s]);
	}
	return pi[s];
}

void make_mintree()
{
	sort(&cg.e[0], &cg.e[m], cmp);

	int i;
	for(i = 0; i < m; i++)
	{
		int fp = find_set(cg.e[i].from), tp = find_set(cg.e[i].to);
		if(fp != tp)
		{
			mintree.add(cg.e[i].from, cg.e[i].to, 1);
			pi[fp] = tp;
		}
	}
}

/* code for generate min tree */

/* code for generate brother tree */

queue<int> q;
graph brother_tree;
int d[MAXN];

void brother_tree_bfs()
{
	int i;
	memset(d, -1, sizeof(d));
	q.push(0);
	d[0] = 0;
	while(!q.empty())
	{
		int v, oldv, u = q.front();
		q.pop();
		oldv = u;
		for(i = mintree.head[u]; i != -1; i = mintree.e[i].next)
		{
			v = mintree.e[i].to;
			if(d[v] == -1)
			{
				d[v] = d[u] + 1;
				q.push(v);
				brother_tree.add(oldv, v, 1);
				oldv = v;
			}
		}
	}
}

void make_brother_tree()
{
	brother_tree_bfs();
	brother_tree.print();
}

/* code for generate brother tree */

/* code for hamiltonian tour in T3 */

graph hamilpath;

void make_recursive(int s)
{
	if(brother_tree.head[s] == -1)
		return;

	int i, left, leftson, right = s;
	left = leftson = brother_tree.e[brother_tree.head[s]].to;

	for(i = brother_tree.head[s]; i != -1; i = brother_tree.e[i].next)
	{
		if(d[i] == d[left])
		{
			right = i;
		}
		if(d[i] == d[left] + 1)
		{
			leftson = i;
		}
	}

	printf("[%d, %d, %d, %d] ", s, right, left, leftson);
	return;
	brother_tree.del(s, left);
	brother_tree.del(left, right);
	brother_tree.add(s, right, 1);
	make_recursive(left);
	make_recursive(s);
	hamilpath.del(s, right);
	hamilpath.del(left, leftson);
	hamilpath.add(left, s, 1);
	hamilpath.add(leftson, right, 1);
}

void make_hamilpath()
{
	make_brother_tree();
	make_recursive(0);
	hamilpath.print();
}

/* code for hamiltonian tour in T3 */

/* code for input , output and init */

void init()
{
	cg.clear();
	mintree.clear();
	brother_tree.clear();
	memset(adj, 0, sizeof(adj));
	int i;
	for(i = 0; i < n; i++)
	{
		pi[i] = i;
	}
}

int main()
{
	int T, iT, i, u, v, w;
	cin >> T;
	for(iT = 0; iT < T; iT++)
	{
		cin >> n >> m;
		
		init();

		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			adj[u][v] = adj[v][u] = w;
			cg.add(u, v, w);
		}

		make_mintree();
		mintree.print();
		make_hamilpath();
	}
	return 0;
}

