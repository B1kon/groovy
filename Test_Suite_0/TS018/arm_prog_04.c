#include "mc12705load_arm.h"

int main() asm ("main");
int main( )
{
typedef unsigned int WORD32;
#define	ok	2222
    int i;
    WORD32 *AddrS;
    WORD32 ck;
    int res;


    res = acl_hostSyncArray(1, 0, 0, (void **)&AddrS, &ck);
    if (res == 1) {
	// host writes to AddrS
	res = ok;
	if ((AddrS[0] && 0x0000000F) != (ck && 0x0000000F)) {
		res = 1;
	}
    }
    acl_hostSync(res);
    acl_hostSync(AddrS[0]);
    return 0;
}
