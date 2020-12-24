#ifndef __MC12705LOAD_NMC_H__
#define __MC12705LOAD_NMC_H__

#ifdef __cplusplus
extern "C" {
#endif

struct ncl_ProcessorNo {
	int cluster_id;
	int core_id;
};

static inline int ncl_getCoreID(void)
{
	unsigned int	nid;

	nid = *((volatile unsigned int *)0x00020000);
	nid = (nid >> 24) & 0x03;

	return nid;
}

static inline int ncl_getClusterID(void)
{
	unsigned int	cid;

	cid = *((volatile unsigned int *)0x00020000);
	cid = (cid >> 26) & 0x03;

	return cid;
}

void ncl_getProcessorNo(struct ncl_ProcessorNo *ProcessorNo);

int ncl_hostSync(int value);
int ncl_hostSyncArray(int value, void *outArray, unsigned int outLen,
		      void **inArray, unsigned int *inLen);

#ifdef __cplusplus
};
#endif

#endif // __MC12705LOAD_NMC_H__
