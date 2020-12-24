/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main() {

    const char *ASSERTION="If PL_GetVersion returns PL_OK, the value >0\n"\
		"was written by at least into one reference version _*";

    const char *TC="001_01";
    char fnd [256];
    char exp [256];
    int res;

    unsigned int version_major = 0;
    unsigned int version_minor = 0;
    int test_res=Pass;

    tc_id(TC, ASSERTION);

     res = PL_GetVersion(&version_major, &version_minor);
    if (res != PL_OK) {
    	sprintf_s(fnd, "PL_GetVersion returns %s %d",sp_error_name[res], res);
    	sprintf_s(exp, "PL_GetVersion returns PL_OK %d", PL_OK);
    	err_msg(fnd, exp);
    	test_res = Fail;
    } else if((version_minor == 0) & (version_major == 0)) {
		sprintf_s(fnd, "version_major=%d version_minor=%d",
			version_major, version_major);
    	sprintf_s(exp, "at least one of versions >0");
    	err_msg(fnd, exp);
    	test_res = Fail;
    }

    result_msg(TC, test_res);
    return(test_res);
}


