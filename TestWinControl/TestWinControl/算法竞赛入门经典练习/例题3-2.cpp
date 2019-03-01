#include "stdafx.h"

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#define MAXN 10
int a[MAXN][MAXN];

int main()
{
	int n, x, y, tot = 0;
	cin >> n;
	memset(a, 0, sizeof(a));
	tot = a[x=0][y=n-1] = 1;	// 起始位标记
	while(tot < n*n)		
	{// n*n -1次循环
		while(x+1 < n  && !a[x+1][y])	// 判断越界和标记
			a[++x][y] = ++tot;

		while(y-1 >= 0 && !a[x][y-1])
			a[x][--y] = ++tot;

		while(x-1 >= 0 && !a[x-1][y])
			a[--x][y] = ++tot;

		while(y+1 < n  && !a[x][y+1])
			a[x][++y] = ++tot;
	}
	for (x = 0; x < n; x++)
	{
		for (y = 0; y < n; y++)
			cout << setw(3) << a[x][y];
		cout << endl;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << setw(3) << a[i][j];
		}
	}
	return 0;
}