#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{volatile int a=0;
    acl_hostSync(4444);

//  while (1) {
 //   a += 1;
 // }


    return 0;
}
