#include "region_defs.h"
void dummy(void);
void dummy(void)
{
#ifdef MCUBOOT_PRIMARY_ONLY
#error
#endif
}
