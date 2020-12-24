#include "mc12705load_nmc.h"

typedef unsigned int WORD32;
#define	len 3
#define	ok	2222

//int main() asm ("__main");
int main()
{
    int i;
    int *AddrS;
    WORD32 ck;
    int res;


	res = ncl_hostSyncArray(1, 0, 0, (void **)&AddrS, &ck);
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
			if ((*(AddrS - 1) != 0) || (*(AddrS + len) != 0)) {
				res = 2;
			}
		}
	}
	ncl_hostSync(res);


	// endless loop to avoid machine hang up. Need  stack installation
//	while(1) {
//		int endless = 0;
//		for(int loop = 0; loop < 100; loop++)
// 		{
// 			endless += loop;
// 		}
//	}
    return 0;
}
