// 三位数反转
#include <stdio.h>
#include <math.h>

int main()
{
	int a, b;
	scanf("%d%d", &a, &b);
	a = a+b;
	b = a-b;
	a = a-b;
	printf("%d %d", a, b);
	return 0;
}