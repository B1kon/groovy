#include "mc12705load_nmc.h"

int main ()
{
	int res;
	int err_nm = 111;

	if ((res=ncl_hostSync(10)) != 30) {
		ncl_hostSync(err_nm);
	}else if ((res=ncl_hostSync(11)) != 31) {
		ncl_hostSync(err_nm);
	}else if ((res=ncl_hostSync(12)) != 32) {
		ncl_hostSync(err_nm);

	}


    return 0;
}
