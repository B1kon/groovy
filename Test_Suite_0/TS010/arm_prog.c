#include "mc12705load_arm.h"

int main() asm ("main");
int main ()
{


int	cluster_id;

    cluster_id = acl_getClusterID();

    acl_hostSyncArray(4, &cluster_id, cluster_id, 0, 0);

    return 0;
}
