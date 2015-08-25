#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(data_size) {
	ops_decoded.is_data_size_16 = true;  //changed data size   add by tiger
	int instr_len = exec(eip + 1);    //execute instruction after prefix
	ops_decoded.is_data_size_16 = false;   //renew data size to 32
	return instr_len + 1;
}
