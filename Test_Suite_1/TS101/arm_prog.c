#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{
typedef unsigned int WORD32;
#define	len 3
#define	ok	2222
    int i;
    int *AddrS;
    WORD32 ck;
    int res;
    int cluster_id=0;

    cluster_id = acl_getClusterID();

    res = acl_hostSyncArray(4, 0, cluster_id, (void **)&AddrS, &ck);
    if (res == 1) {
	// host writes to AddrS
	res = ok;
	for(i = 0; i < len; i++) {
	    if (AddrS[i] != ck) {
		res = 1;
		break;
	    }
 	}
    }
    acl_hostSync(res);


    return 0;
}
