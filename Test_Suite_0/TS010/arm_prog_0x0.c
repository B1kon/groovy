#include "mc12705load_arm.h"

int main() asm ("main");
int main( )
{
	int a = 1;
	int b = 2;
	int c;

	c = a + b;

	return c;
}
