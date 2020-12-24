/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>


int main() {

    const char *ASSERTION="If count is NULL, function’s result - PL_OK\n"\
			"if a driver of this type of the board is available";
    const char *TC="002_02";
    char fnd [256];
    char exp [256];
    int res;
    int test_res=Pass;
    FILE *fd;
    int exp_res = PL_OK;

    tc_id(TC, ASSERTION);

    system("lsmod|grep mc12705 >/dev/null; rm -f oo nn; if [ $? -eq 0 ]; then echo oo >oo; else echo nn >nn;fi");
    if (!(fd=fopen("oo", "r"))){
	exp_res = PL_ERROR;
    }
    res = PL_GetBoardCount(NULL);
    if ((res != PL_OK) & (exp_res == PL_OK)) {
    	sprintf_s(fnd, "PL_GetBoardCount returns %s %d, "\
			"but the driver is installed",
			sp_error_name[res], res);
    	sprintf_s(exp, "PL_GetBoardCount returns PL_OK %d", PL_OK);
    	err_msg(fnd, exp);
    	test_res = Fail;
     }
    if ((res == PL_OK) & (exp_res != PL_OK)) {
    	sprintf_s(fnd, "PL_GetBoardCount returns %s %d, "\
			"but the driver is not installed",
			sp_error_name[res], res);
    	sprintf_s(exp, "PL_GetBoardCount returns PL_ERROR %d", PL_ERROR);
    	err_msg(fnd, exp);
    	test_res = Fail;
     }
    result_msg(TC, test_res);
    return(test_res);
}


