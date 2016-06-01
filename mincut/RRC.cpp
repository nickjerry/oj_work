#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <algorithm>

using namespace std;

#define sqrt_2 1.41421356237309504880
#define INF 0x3f3f3f3f
#define MAXN 9999
#define MAXM 999999

struct edge {
	int u, v, w;
};

int n, m;
int pe, pi[MAXN];
edge e[MAXM];

/* variable for determinated algorithm */
#define LEFTN 16
int s, t;
int combine[LEFTN];
int wet[LEFTN], vis[LEFTN];

int search(int **adj)
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
		/* 上一行的意思是上2-9行关于j的循环无法更新tmp，
		 * 即所有点均为vis状态或combine状态，
		 * 由于combine的点不断增加，所以在后期search时，
		 * 可被vis的点基本小于n，这一步即可提前结束掉循环。
		 * 如果这一行不加，则下一行s会被更新为t，
		 * 这会影响solve的正确运行
		 */
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

int solve(int **adj)
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

void add(int u, int v, int w)
{
	e[pe].u = u;
	e[pe].v = v;
	e[pe].w = w;
	pe ++;
}

int find(int s)
{
	if(s != pi[s])
		pi[s] = find(pi[s]);
	return pi[s];
}

void init()
{
	int i;
	pe = 0;
	for(i = 0; i < n; i++)
		pi[i] = i;
}

int main()
{
	srand(time(NULL));
	int i, u, v, w, left;
	while(scanf("%d%d", &n, &m) != EOF)
	{
		left = n;
		init();
		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			add(u, v, w);
		}

	}
	return 0;
}
