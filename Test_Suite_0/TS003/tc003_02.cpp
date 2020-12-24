/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result PL_GetBoardDesc - PL_ERROR if\n"\
		" value of the index parameter is less than 0 \n"\
		"or more count-1 value from the PL_GetBoardCount";
    const char *TC="003_02";
    char fnd [256];
    char exp [256];
    int res;
    unsigned int count;

    int test_res = Pass, test_res1 = Pass, test_res2 = Pass;
    PL_Board *board = (PL_Board *)0;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault
printf("1111111\n");
		if (( res = PL_GetBoardCount(&count)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardCount returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardCount returns PL_OK %d", PL_OK);
			err_msg(fnd, exp);
			test_res = UnTested;
			result_msg(TC, test_res);
			return(test_res);
		}
printf("2222222\n");
		if ((res = PL_GetBoardDesc(count-count-1, &board)) != PL_ERROR) {
//		if ((res = PL_GetBoardDesc(99, &board)) != PL_ERROR) {
			sprintf_s(fnd, "index=%d PL_GetBoardDesc returns %s %d",
					count-count-1, sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_ERROR");
			err_msg(fnd, exp);
			test_res1 = Fail;
		}
printf("333333\n");
		if ((res = PL_GetBoardDesc(count, &board)) != PL_ERROR) {
			sprintf_s(fnd, "index=%d PL_GetBoardDesc returns %s %d",
						count, sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_ERROR");
			err_msg(fnd, exp);
			test_res2 = Fail;
		}
printf("4444444\n");
		if ((test_res1 != Pass) || (test_res2 != Pass)) test_res = Fail;

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
printf("5555555\n");
    if (test_res == Pass) {
	PL_CloseBoardDesc(board);
    }
    result_msg(TC, test_res);
    return(test_res);
}


