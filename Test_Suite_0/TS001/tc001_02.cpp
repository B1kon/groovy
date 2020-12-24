/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main() {

    const char *ASSERTION="If the arguments of version is NULL, "\
				"the result PL_GetVersion = PL_OK.";

    const char *TC="001_02";
    char fnd [256];
    char exp [256];
    int res;

    int test_res=Pass;

    tc_id(TC, ASSERTION);

    res = PL_GetVersion(NULL, NULL);
    if (res != PL_OK) {
    	sprintf_s(fnd, "PL_GetVersion returns %s %d",sp_error_name[res], res);
    	sprintf_s(exp, "PL_GetVersion returns PL_OK %d", PL_OK);
    	err_msg(fnd, exp);
    	test_res = Fail;
    }

    result_msg(TC, test_res);
    return(test_res);
}


