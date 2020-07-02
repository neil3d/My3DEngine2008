#ifndef _NEXUS_MEMORY_H_
#define _NEXUS_MEMORY_H_

#define nNew new
#define	nMalloc(sz) ::malloc(sz)
#define	nFree(ptr)	::free(ptr)

#endif //_NEXUS_MEMORY_H_