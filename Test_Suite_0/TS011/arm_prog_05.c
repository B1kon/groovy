#include "mc12705load_arm.h"

int main() asm ("main");
int main( )
{
typedef unsigned int WORD32;
#define	len 3
#define	ok	2222
    int i;
    int *AddrS;
    WORD32 ck;
    int res;


	res = acl_hostSyncArray(1, 0, 0, (void **)&AddrS, &ck);
	if (res == 1) {
		// host writes to AddrS
		res = ok;
		for(i = 0; i < len; i++) {
			if (AddrS[i] != ck) {
				res = 1;
				break;
			}
 		}
		if (res == ok) {
			if ((*(AddrS - 1) == 0) || (*(AddrS + len) == 0)) {
				res = 2;
			}
		}
	}
	acl_hostSync(res);

    return 0;
}
