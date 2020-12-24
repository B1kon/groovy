#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{
	int res;
	int err_arm = 111;

	if ((res=acl_hostSync(10)) != 30) {
		acl_hostSync(err_arm);
	}else if ((res=acl_hostSync(11)) != 31) {
		acl_hostSync(err_arm);
	}else if ((res=acl_hostSync(12)) != 32) {
		acl_hostSync(err_arm);

	}


    return 0;
}
