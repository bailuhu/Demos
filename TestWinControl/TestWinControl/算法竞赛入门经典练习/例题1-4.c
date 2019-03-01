// ผฆอรอฌมý
#include <stdio.h>
#include <math.h>

int main()
{
	int x, y, m, n;
	scanf("%d%d", &n, &m);
	x = (4*n - m)/2;
	y = n - x;
	if (m%2 == 1 || x < 0 || y < 0)
		printf("No answer");
	else
		printf("%d %d", x, y);
	return 0;
}