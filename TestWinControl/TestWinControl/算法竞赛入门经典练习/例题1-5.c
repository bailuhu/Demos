// ÈıÕûÊıÅÅĞò
#include <stdio.h>
#include <math.h>

int main()
{
	int a, b, c;
	scanf("%d%d%d", &a, &b, &c);
	if (a > b)
	{
		a = a+b;
		b = a-b;
		b = a-b;
	}
	if (a > c)
	{
		a = a+c;
		c = a-c;
		c = a-c;
	}
	if (b > c)
	{
		b = b+c;
		c = b-c;
		c = b-c;
	}
	printf("%d %d %d", a, b, c);
	return 0;
}