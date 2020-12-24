/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result of PL_CloseBoardDesc is independent of\n"\
			"other module descriptors with the same index.";
    const char *TC="004_04";
    char fnd [256];
    char exp [256];
    int res, i, j, detal=0;
    int test_res = Pass;
    unsigned int count;

#define board_cnt	3
    PL_Board *board[board_cnt];
    PL_Access *access[board_cnt];
    struct PL_CoreNo No = {-1,-1};

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
		if(test_res != Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for (i=0; i<board_cnt; i++){
			if ((res = PL_GetBoardDesc(0, &(board[i]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetBoardDesc step %d returns %s %d",
							i, sp_error_name[res], res);
				sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
				err_msg(fnd, exp);
				test_res = UnTested;
				detal=1;
				break;
			}
			if ((res = PL_GetAccess(board[i], &No, &(access[i]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess step %d returns %s %d",
							i, sp_error_name[res], res);
				sprintf_s(exp,"After PL_GetBoardDes, PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = UnTested;
				detal=2;
				break;
			}
		}
		if(test_res != Pass){
			for (j=0; j<(i-detal-1); j++){
				PL_CloseAccess(access[j]);
			}
			for (j=0; j<i; j++){
			PL_CloseBoardDesc(board[j]);
			}
		}else{
			if ((res = PL_CloseAccess(access[0])) != PL_OK) {
				sprintf_s(fnd, "PL_CloseAccess returns %s %d",
							sp_error_name[res], res);
				sprintf_s(exp, "PL_CloseAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_CloseBoardDesc(board[0])) != PL_OK) {
				sprintf_s(fnd, "PL_CloseBoardDesc returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, "PL_CloseBoardDesc returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_GetAccess(board[0], &No, &(access[0]))) == PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp,
					"After PL_CloseBoardDesc, PL_GetAccess returns PL_ ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
			}
			for (j=1; j<i; j++){
				PL_CloseAccess(access[j]);
			}
			for (j=1; j<i; j++){
				PL_CloseBoardDesc(board[j]);
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


