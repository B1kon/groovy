#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{


int	cluster_id;

    cluster_id = acl_getClusterID();

    acl_hostSync(cluster_id);

    return 0;
}
