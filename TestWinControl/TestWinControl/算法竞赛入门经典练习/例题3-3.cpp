#include "stdafx.h"

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#define MAXN 10
int a[MAXN][MAXN];

int main()
{
	int i, ok, abc, de, x, y, z, count = 0;
	char s[20], buf[99];
	cin >> s;
	for (abc = 111; abc <= 999; abc++)
		for (de = 11; de <= 99; de++)
		{
			x = abc*(de%10);
			y = abc*(de/10);
			z = abc*de;
			sprintf_s(buf, "%d%d%d%d%d", abc, de, x, y, z);
			ok = 1;
			for (i = 0; i < strlen(buf); i++)
				if(strchr(s, buf[i]) == NULL)
					ok = 0;
			if (ok)
			{
				cout << "<" << ++count << endl;
				cout << setw(5) << abc << endl
					 << setw(4) << de << endl
					 << "-----" << endl
					 << setw(5) << x << endl
					 << setw(4) << y << endl
					 << setw(5) << z << endl
					 << endl;
			}
		}
		cout << "The number of solutions = " << count;
	return 0;
}