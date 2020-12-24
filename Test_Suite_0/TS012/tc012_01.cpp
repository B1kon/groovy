/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If argument access is not created by PL_GetAccess\n"\
			"then PL_ReadMemBlock returns PL_ERROR";
    const char *TC="012_01";
    char fnd [256];
    char exp [256];
    int res;
	
    int test_res = Pass;
    char brd [256];
    PL_Access *access = (PL_Access *)brd;
    PL_Word block[3];

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if ((res = PL_ReadMemBlock (access, block, 0x555, 1)) == PL_OK) {
			sprintf_s(fnd, "PL_ReadMemBlock  returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_ReadMemBlock  returns PL_ ERROR");
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


