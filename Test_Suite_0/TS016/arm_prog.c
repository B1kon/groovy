#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{
    acl_hostSync(4444);

    return 5555;
}
