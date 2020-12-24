#include "mc12705load_nmc.h"

int main() asm ("__main");
int main ()
{

int	nm_id;		// NM number (for any ARM: -1)
int	cluster_id;	// Cluster number (for central ARM: -1)

    nm_id = ncl_getCoreID();
    cluster_id = ncl_getClusterID();

    ncl_hostSyncArray(nm_id, 0, cluster_id, 0, 0);

    return 0;
}
