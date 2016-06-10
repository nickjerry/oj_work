#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <algorithm>

using namespace std;

template<typename T>

#define MAXM 20000000

void random_permutation(T a[], int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		int j = rand() % (n - i) + i;
		swap(a[i], a[j]);
	}
}

struct edge {
	int u, v, w;
	edge(int x = 0, int y = -1, int z = -2) {u = x; v = y; w = z;}
} e[MAXM];

int main()
{
	int i, j, k, m, n, T = 3, iT;
	srand(time(NULL));
	for(iT = 0; iT < T; iT++)
	{
		int pe = 0;
		n = rand() % 50 + 2;
		m = rand() % 200 + 1;
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
			{
				if(i == j)
					continue;
				int tc = rand() % 3 + 1;
				for(k = 0; k < tc; k++)
				{
					int w = rand() % 16 + 1;
					e[pe ++] = edge(i, j, w);
				}
			}
		if(pe < m) m = pe;
		printf("%d %d\n", n, m);
		random_permutation(e, pe);
		for(i = 0; i < m; i++)
			printf("%d %d %d\n", e[i].u, e[i].v, e[i].w);
	}
	return 0;
}
