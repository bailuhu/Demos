#include <stdio.h>

int main()
{
	int i = 1, c;
	while((c = getchar()) != EOF)
	{
		if (c == '"') 
		{ 
			printf("%s",i?"``":"''"); 
			i = !i;
		}
		else printf("%c", c);
	}
	return 0;
}