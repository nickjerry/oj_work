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
		e[pe].vis = false;
		head[u] = pe++;
		degree[u]++;

		e[pe].from = v;
		e[pe].to = u;
		e[pe].weight = w;
		e[pe].next = head[v];
		e[pe].vis = false;
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


int n, m;
graph g, cg, mintree;

/* code for generate min tree */

int pi[MAXN];

bool cmp(edge a, edge b)
{
	if(a.weight < b.weight)
		return true;
	else if(a.weight > b.weight)
		return false;
	else
		return a.from < b.from;
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
			mintree.add(cg.e[i].from, cg.e[i].to, cg.e[i].weight);
			pi[fp] = tp;
		}
	}
}

/* code for generate min tree */

/* code for perfectmatching */

graph odd;
bool is_odd[MAXN];

void make_odd()
{
	odd.clear();
	memset(is_odd, 0, sizeof(is_odd));

	int i, j;
	for(i = 0; i < n; i++)
	{
		if(mintree.head[i] == -1 || (mintree.degree[i] & 1))
		{
			is_odd[i] = true;
		}

	}

	for(i = 0; i < n; i++)
	{
		if(g.head[i] == -1 || !is_odd[i])
			continue;

		for(j = g.head[i]; j != -1; j = g.e[j].next)
		{
			if(!g.e[j].vis && is_odd[g.e[j].to])
			{
				g.e[j].vis = true;
				g.e[j ^ 1].vis = true;
				odd.add(g.e[j].from, g.e[j].to, g.e[j].weight);
			}
		}
	}
}

bool pm_vis[MAXN];

void make_perfect_match()
{
	make_odd();
	memset(pm_vis, 0, sizeof(pm_vis));
	sort(&odd.e[0], &odd.e[odd.pe], cmp);

	int i;
	for(i = 0; i < odd.pe; i += 2)
	{
		if(!pm_vis[odd.e[i].from] && !pm_vis[odd.e[i].to])
		{
			mintree.add(odd.e[i].from, odd.e[i].to, odd.e[i].weight);
			pm_vis[odd.e[i].from] = true;
			pm_vis[odd.e[i].to] = true;
		}
	}
}

/* code for perfectmatching */

/* code for eulerian tour */

int nodetop, pathtop;
int eulerpath[MAXN], eulernode[MAXN];

void euler_dfs(int s)
{
	int i;
	eulernode[nodetop ++] = s;

	for(i = mintree.head[s]; i != -1; i = mintree.e[i].next)
	{
		if(!mintree.e[i].vis)
		{
			mintree.e[i].vis = true;
			mintree.e[i ^ 1].vis = true;
			euler_dfs(mintree.e[i].to);
			break;
		}
	}
}

void make_euler_tour()
{
	nodetop = 0;
	pathtop = 0;
	eulernode[nodetop ++] = 0;

	int i, flag;
	while(nodetop > 0)
	{
		flag = 0;
		int u = eulernode[nodetop - 1];
		for(i = mintree.head[u]; i != -1; i = mintree.e[i].next)
		{
			if(!mintree.e[i].vis)
			{
				flag = 1;
			}
		}

		if(flag) euler_dfs(eulernode[-- nodetop]);
		else eulerpath[pathtop ++] = eulernode[-- nodetop];
	}
}


/* code for eulerian tour */

/* code for input , output and init */

bool vis[MAXN];

void output_path()
{
	memset(vis, 0, sizeof(vis));

	printf("hamiltonian path: ");

	int i;
	for(i = 0; i < pathtop; i++)
	{
		if(!vis[eulerpath[i]])
		{
			vis[eulerpath[i]] = true;
			printf("%d ", eulerpath[i]);
		}
	}

	printf("%d\n\n", eulerpath[0]);
}

void init()
{
	g.clear();
	cg.clear();
	mintree.clear();
	odd.clear();
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
			g.add(u, v, w);
			cg.add(u, v, w);
		}

		make_mintree();
		make_perfect_match();
		make_euler_tour();
		output_path();
	}
	return 0;
}

