#include "mc12705load_arm.h"

typedef unsigned int WORD32;
int main() asm ("main");
int main ()
{
    WORD32 Fill = 0x93939392;

    WORD32 *AddrStore = (WORD32 *) 0xCC010;

         AddrStore[0] = Fill;


    acl_hostSyncArray(1, AddrStore, 0, 0, 0);

    return 0;
}
