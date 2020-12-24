/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If board is NULL and the driver of the board\n"\
			"is available, the result PL_GetBoardDesc - PL_OK";

    const char *TC="003_03";
    char fnd [256];
    char exp [256];
    int res;
    int test_res = Pass;
    int exp_res = PL_OK;

    tc_id(TC, ASSERTION);

    __try {// Handler Segmentation Fault
		res = PL_GetBoardDesc(0,NULL);
		if ((res != PL_OK) & (exp_res == PL_OK)) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d, "\
					"but the driver is installed",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardDesc returns PL_OK %d", PL_OK);
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if ((res == PL_OK) & (exp_res != PL_OK)) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d, "\
					"but the driver is not installed",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardDesc returns PL_ERROR %d", PL_ERROR);
			err_msg(fnd, exp);
			test_res = Fail;
		}
    } // End __try
    __except((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? \
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
    {
        printf("ERROR: Found SIGSEGV\n");
        sprintf_s(fnd, "ERROR: Found SIGSEGV");
        sprintf_s (exp, "the test without any exceptions");
        err_msg(fnd, exp);
        test_res = UnTested;
    }
    result_msg(TC, test_res);
    return(test_res);
}


