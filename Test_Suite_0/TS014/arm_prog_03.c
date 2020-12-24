#include "mc12705load_arm.h"

typedef unsigned int WORD32;
int main() asm ("main");
int main ()
{
    int res, j;
    int err_nm = 111;
    int Value_nm[] = {10, 11, 12};
    WORD32 Length_nm[] = {13, 14, 15};
    WORD32 *Addr_nm[] = {(WORD32 *)16, (WORD32 *)17, (WORD32 *)18};
    int Value;

    WORD32 Length_pc[3];
    WORD32 *Addr_pc[3];
    int exp_Value_pc[] = {30, 31, 32};
    WORD32 exp_Length_pc[] = {40, 41, 42};
    WORD32 *exp_Addr_pc[] = {(WORD32 *)50, (WORD32 *)51, (WORD32 *)52};


    for (j=0; j<3; j++) {
	Value = Value_nm[j];
	if ((res = acl_hostSyncArray(Value, Addr_nm[j], Length_nm[j], 
				(void **)(&(Addr_pc[j])), &(Length_pc[j])
						)) != exp_Value_pc[j]) {
	    Value = err_nm;
	}else if ((Addr_pc[j] != exp_Addr_pc[j]) || 
		   (Length_pc[j] != exp_Length_pc[j]) ) {
	    Value = err_nm;
	}
    }

    return 0;
}
