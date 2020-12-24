#include "mc12705load_nmc.h"

typedef unsigned int WORD32;
#define	ok	2222

//int main() asm ("__main");
int main()
{
    int i;
    int *AddrS;
    WORD32 ck, len;
    int res;

    while(1){
	len = ncl_hostSyncArray(1, 0, 0, (void **)&AddrS, &ck);
	// host writes to AddrS
	res = ok;
	for(i = 0; i < len; i++) {
		if (AddrS[i] != ck) {
			res = 1;
			break;
		}
 	}
	ncl_hostSync(res);
    }
    return 0;
}
