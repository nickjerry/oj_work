#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

#define MAXN 6000
#define INF 0x3f3f3f3f

int n, m, s, t;
int combine[MAXN], adj[MAXN][MAXN];
int wet[MAXN], vis[MAXN];

int search()//可用数学归纳法证明这样做的正确性
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

int solve()
{
	int i, j, ans = INF;
	memset(combine, 0, sizeof(combine));
	for(i = 0; i < n - 1; i++) //最后要留两个点
	{
		int ret = search();
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


int main()
{
	int i, u, v, w;
	while(scanf("%d%d", &n, &m) != EOF)
	{
		memset(adj, 0, sizeof(adj));
		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			adj[u][v] += w;
			adj[v][u] += w;
		}

		cout << solve() << '\n';
	}
	return 0;
}
