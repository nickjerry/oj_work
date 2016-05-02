#include <stdio.h>
#include <string.h>
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
	}
};


int n, m;
graph g, cg, mintree;

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

/* code for construct T3 */

int tree3_par;
bool tree3_vis[MAXN];

void tree3_bfs(int s, int depth)
{
	int i;
	if(depth > 3)
		return;

	for(i = mintree.head[s]; i != -1; i = mintree.e[i].next)
	{
		if(!tree3_vis[mintree.e[i].to])
		{
			tree3_vis[mintree.e[i].to] = true;
			if(depth > 0)
				mintree.add(tree3_par, mintree.e[i].to, 1);
			tree3_bfs(mintree.e[i].to, depth + 1);
		}
	}
}

void make_tree3()
{
	int pt = 0, tmp[MAXN];
	memset(tree3_vis, 0, sizeof(tree3_vis));

	int i;
	for(i = 0; i < n; i++)
	{
		if(mintree.head[i] == -1)
			continue;

		tree3_par = i;
		tree3_bfs(i, 0);
	}
}

/* code for construct T3 */


/* code for hamiltonian tour in T3 */


/* code for hamiltonian tour in T3 */

/* code for input , output and init */

void init()
{
	g.clear();
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
		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			g.add(u, v, w);
			cg.add(u, v, w);
		}

		make_mintree();
		make_tree3();
	}
	return 0;
}

