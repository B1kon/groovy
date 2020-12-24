#include "mc12705load_arm.h"

int main() asm ("main");
int main( )
{
typedef unsigned int WORD32;
#define	ok	2222
    int i;
    int *AddrS;
    WORD32 ck, len;
    int res;

    while(1){
	len = acl_hostSyncArray(1, 0, 0, (void **)&AddrS, &ck);
	// host writes to AddrS
	res = ok;
	for(i = 0; i < len; i++) {
		if (AddrS[i] != ck) {
			res = 1;
			break;
		}
 	}
	acl_hostSync(res);
    }
    return 0;
}
