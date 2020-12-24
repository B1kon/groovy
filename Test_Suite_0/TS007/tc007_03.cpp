/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main() {

    const char *ASSERTION="If at least one the argument of version is NULL,\n"\
		"the result PL_GetFirmwareVersion = PL_ERROR";
    const char *TC="007_03";
    char fnd [256];
    char exp [256];
    unsigned int res;
    unsigned int count;
    PL_Board *board = (PL_Board *)0;
    int test_res=Pass;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if (( res = PL_GetBoardCount(&count)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardCount returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardCount returns PL_OK %d", PL_OK);
			err_msg(fnd, exp);
			test_res = UnTested;
		} else if(count == 0) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0");
			err_msg(fnd, exp);
			test_res = UnTested;
		} else if ((res = PL_GetBoardDesc(0, &board)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d",
						sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if  ((res = PL_GetFirmwareVersion(board, NULL, NULL)) == PL_OK) {
			sprintf_s(fnd, "PL_GetFirmwareVersion returns %s %d",
							sp_error_name[res], res);
			sprintf_s(exp, "PL_GetFirmwareVersion returns PL_ ERROR");
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
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


