#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAXN 9999
#define MAXM 999999

struct edge {
	int u, v, w;
};

int n, m;
int pe, pi[MAXN];
edge e[MAXM];

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
	int i, u, v, w, left, cut;
	while(scanf("%d%d", &n, &m) != EOF)
	{
		cut = 0;
		left = n;
		init();
		for(i = 0; i < m; i++)
		{
			cin >> u >> v >> w;
			add(u, v, w);
		}

		/* random permutation firstly */
		for(i = 0; i < m; i++)
		{
			int j = rand() % (m - i) + i;
			swap(e[i], e[j]);
		}
		
		for(i = 0; i < m; i++)
		{
			int x = e[i].u;
			int y = e[i].v;
			int px = find(x);
			int py = find(y);
			if(px != py)
			{
				pi[px] = py;
				left --;
			}
			if(left <= 2)
				break;
		}

		for(i = 0; i < m; i++)
		{
			int u = e[i].u;
			int v = e[i].v;
			int w = e[i].w;

			int pu = find(u);
			int pv = find(v);

			if(pu != pv)
				cut += w;
		}
		printf("%d\n", cut);
	}
	return 0;
}
