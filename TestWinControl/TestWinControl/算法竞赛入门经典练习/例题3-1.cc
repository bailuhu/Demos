#include "stdafx.h"
#include <iostream>
using namespace std;

#define MAXN 1000 + 10
int a[MAXN];

int main()
{
	int i, j, n, k, first = 1;
	memset(a, 0, sizeof(a));
	cin >> n >> k;
	for (i = 1; i <= k; i++)
		for (j = 1; j <= n; j++)
			if(j % i == 0) 
				a[j] = !a[j];
	for (i = 1; i <= n; i++)
		if(a[i])
		{
			if (first)
				first = 0;
			else
				cout << " ";
			cout << i;
		}
		cout << endl;
		return 0;
}