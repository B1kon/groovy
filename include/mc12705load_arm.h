#ifndef __MC12705LOAD_ARM_H__
#define __MC12705LOAD_ARM_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline int acl_getClusterID(void)
{
	unsigned int	cid;

	cid = *((volatile unsigned int *)0x000CB000);
	cid = cid & 0x07;

	return cid;
}

static inline unsigned int acl_irqEnable(void)
{
	unsigned int state;

	asm("MRS %0, CPSR":"=r" (state)::);
	asm("CPSIE i":::"memory");

	return state;
}

static inline unsigned int acl_irqDisable(void)
{
	unsigned int state;

	asm("MRS %0, CPSR":"=r" (state)::);
	asm("CPSID i":::"memory");

	return state;
}

static inline unsigned int acl_irqSetState(unsigned int new_state)
{
	unsigned int old_state;

	asm("MRS %0, CPSR":"=r" (old_state)::);
	asm("MSR CPSR, %0"::"r" (new_state):"memory");

	return old_state;
}

#define ACL_ATOMIC_BLOCK() \
	for (unsigned int state = acl_irqDisable(), flag = 1; \
	     flag; \
	     acl_irqSetState(state), flag = 0)

int acl_hostSync(int value);
int acl_hostSyncArray(int value, void *outArray, unsigned int outLen,
		      void **inArray, unsigned int *inLen);

int acl_isrHandlerSet(int index, void (*handler)(void));
int acl_swiHandlerSet(void (*handler)(int));

#ifdef __cplusplus
};
#endif

#endif // __MC12705LOAD_ARM_H__
