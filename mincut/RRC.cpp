#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

#define sqrt_2 1.41421356237309504880
#define INF 0x3f3f3f3f
#define MAXN 9999
#define MAXM 999999

struct edge {
	int u, v, w;
	edge(int x = 0, int y = -1, int z = -2){u = x; v = y; w = z;}
};

/* variable for determinated algorithm */
#define LEFTN 16
int s, t;
int pi[MAXN];
int adj[MAXN][MAXN];
int combine[LEFTN];
int wet[LEFTN], vis[LEFTN];

int search(int n)
{
	int i, j, max, tmp = 0;
	memset(wet, 0, sizeof(wet));
	memset(vis, 0, sizeof(vis));
	s = t = -1;
	for(i = 0; i < n; i++)
	{
		max = -INF;
		for(j = 0; j < n; j++)
		{
			if(!combine[j] && !vis[j] && wet[j] > max)
			{
				max = wet[j];
				tmp = j;
			}
		}
		if(t == tmp) return wet[tmp];
		s = t;
		t = tmp;
		vis[tmp] = 1;

		for(j = 0; j < n; j++)	
		{
			if(!combine[j] && !vis[j])
			{
				wet[j] += adj[tmp][j];
			}
		}	
	}

	return wet[t];
}

int solve(vector<edge> v, int n)
{
	int i, j, ans = INF, m = v.size();
	memset(adj, 0, sizeof(adj));

	for(i = 0; i < m; i++)
	{
		adj[v[i].u][v[i].v] += v[i].w;
		adj[v[i].v][v[i].u] += v[i].w;
	}

	memset(combine, 0, sizeof(combine));
	for(i = 0; i < n - 1; i++) //最后要留两个点
	{
		int ret = search(n);
		if(ret < ans)
			ans = ret;
		if(ans == 0) //disconnected graph
			return 0;
		combine[t] = 1;
		for(j = 0; j < n; j ++)
		{
			if(!combine[j])
			{
				adj[s][j] += adj[t][j];
				adj[j][s] += adj[j][t];
			}
		}
	}
	return ans;
}

int find(int s)
{
	if(s != pi[s])
		pi[s] = find(pi[s]);
	return pi[s];
}

bool tarjan(vector<edge>& e, vector<edge> &ne, int &n)
{
	int i, j, pe, m = e.size();

	if(n <= 6)
		return true;

	/* init the pi array */
	for(i = 0; i < n; i++)
		pi[i] = i;

	/* random permutation firstly */
	for(i = 0; i < m; i++)
	{
		int j = rand() % (m - i) + i;
		swap(e[i], e[j]);
	}
	
	/* tarjan to n / sqrt(2) vertices */
	int left = n;
	int ln = (int)(1 + n / sqrt_2);
	ln = max(ln, 2);
	for(i = 0; i < m; i++)
	{
		if(left <= ln)
			break;
		int x = e[i].u;
		int y = e[i].v;
		int px = find(x);
		int py = find(y);
		if(px != py)
		{
			pi[px] = find(py);
			left --;
		}
	}

	if(left > ln)
		return false;

	/* construct new graph with ln's vertices */
	pe = 0;
	memset(adj, 0, sizeof(adj));
	memset(vis, -1, sizeof(vis));
	for(i = 0; i < n; i++)
		if(i == find(i))
			vis[find(i)] = pe ++;
	for(i = 0; i < n; i++)
		vis[i] = vis[find(i)];

	ne.clear();
	for(i = 0; i < ((int)e.size()); i++)
	{
		int vu = vis[e[i].u];
		int vv = vis[e[i].v];
		if(vu != vv)
			ne.push_back(edge(vu, vv, e[i].w));
	}

	n = ln;
	return true;
}

int RRC(vector<edge>& v, int n)
{
	static int date = 0;
	date ++;
	if(n <= 6)
	{
		return solve(v, n);
	}
	int cn;
	vector<edge> e;

	int a;
	cn = n;
	if(tarjan(v, e, cn))
		a = RRC(e, cn);
	else
		a = 0;

	int b;
	cn = n;
	if(tarjan(v, e, cn))
		b = RRC(e, cn);
	else
		b = 0;
	return min(a, b);
}

int main()
{
	srand(time(NULL));
	vector<edge> e;
	int i, u, v, w, n, m;
	while(scanf("%d%d", &n, &m) != EOF)
	{
		e.clear();
		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			edge tmp;
			tmp.u = u; tmp.v = v; tmp.w = w;
			e.push_back(tmp);
		}

		printf("%d\n", RRC(e, n));
	}
	return 0;
}
