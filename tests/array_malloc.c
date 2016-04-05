#include <stdio.h>
#include <stdlib.h>

int main()
{
	int* a = (int*) malloc( sizeof(int) * 100 );
	a[10] = 6;
	int i = 6;
	a[2*i + 5] = 45;
	return 0;
}
