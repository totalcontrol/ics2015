#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(pop_i)
make_helper_v(pop_r)


/*make_helper_v(pop_i2rm)
make_helper_v(pop_si2rm)
make_helper_v(pop_r2rm)
make_helper_v(pop_rm2r)*/

