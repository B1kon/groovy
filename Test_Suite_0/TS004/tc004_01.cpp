/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If the result PL_CloseBoardDesc - PL_OK,\n"\
		"a descriptor of the module is removed";
    const char *TC="004_01";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    struct PL_CoreNo No = {-1,-1};

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		PL_GetBoardCount(&count);
		if((count == 0) || (count == test_cnt)) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0 and count != test_cnt (%d)",test_cnt);
			err_msg(fnd, exp);
			test_res = UnTested;
		} else if ((res = PL_GetBoardDesc(0, &board)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d",
					sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_GetAccess(board, &No, &access)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp,"After PL_GetBoardDes, PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_CloseAccess(access)) != PL_OK) {
			sprintf_s(fnd, "PL_CloseAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_CloseAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_CloseBoardDesc(board)) != PL_OK) {
			sprintf_s(fnd, "PL_CloseBoardDesc returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_CloseBoardDesc returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_GetAccess(board, &No, &access)) == PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp,"After PL_CloseBoardDesc, PL_GetAccess returns PL_ ERROR");
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


