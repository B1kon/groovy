/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main() {

    const char *ASSERTION="At result of PL_GetBoardCount = PL_OK,\n"\
			"the count pointer contains number of the modules,\n"\
			"ready to work";

    const char *TC="002_01";
    char fnd [256];
    char exp [256];
    int res;
#define test_count	100000
    unsigned int count = test_count;
    int test_res=Pass;

    tc_id(TC, ASSERTION);

    res = PL_GetBoardCount(&count);
    if (res != PL_OK) {
    	sprintf_s(fnd, "PL_GetBoardCount returns %s %d",
				sp_error_name[res], res);
    	sprintf_s(exp, "PL_GetBoardCount returns PL_OK %d", PL_OK);
    	err_msg(fnd, exp);
    	test_res = Fail;
     } else if(count == test_count) {
		sprintf_s(fnd, "count = %d",count);
    	sprintf_s(exp, "count != test_count (%d)", test_count);
    	err_msg(fnd, exp);
    	test_res = Fail;
	 }
printf("count = %d",count);
    result_msg(TC, test_res);
    return(test_res);
}

