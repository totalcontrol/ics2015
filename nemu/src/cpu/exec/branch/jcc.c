#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

/*make_helper_v(jcc_i2a)
make_helper_v(jcc_i2rm)
make_helper_v(jcc_si2rm)
make_helper_v(jcc_r2rm)
make_helper_v(jcc_rm2r)*/

