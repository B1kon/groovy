/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_GetBoardDesc can create descriptors "\
		"for all modules, ready to work.";
    const char *TC="003_06";
    char fnd [256];
    char exp [256];
    int res, i;
    unsigned int count, cur;
    int test_res = Pass;
    PL_Board *board[100];

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
		}
		if (test_res != Pass) {
			result_msg(TC, test_res);
			return(test_res);
		}
		for(i=0; i<(int)count; i++) {
			if ((res = PL_GetBoardDesc(i, &(board[i]))) != PL_OK) {
				sprintf_s(fnd, "index=%d PL_GetBoardDesc returns %s %d",
							i, sp_error_name[res], res);
				sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
		}
		cur = i-1;
		if(cur >=0) {
			for(i=0; i<(int)count; i++) {
				PL_CloseBoardDesc(board[i]);
			}
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


