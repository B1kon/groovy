/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Function PL_GetAccess can create more then\n"\
		"one object Access for the same index.\n"\
		"Calling PL_GetAccess in a one task";
    const char *TC="008_04_0";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access1 = (PL_Access*)0;
    PL_Access *access2 = (PL_Access*)0;
    struct PL_CoreNo coreNo={0,3};
    const char *nm_prog = "nm_prog_04.abs";

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
		} else if ((res = PL_GetAccess(board, &coreNo, &access1)) != PL_OK) {
			sprintf_s(fnd, "First PL_GetAccess returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_GetAccess(board, &coreNo, &access2)) != PL_OK) {
			sprintf_s(fnd, "Second PL_GetAccess returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access1, nm_prog)) != PL_OK) {
			sprintf_s(fnd, "PL_LoadProgramFile with access1 returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			err_msg(fnd, exp);
		} else if ((res = PL_Sync(access2,0,NULL)) != PL_OK) {
			sprintf_s(fnd, "PL_Sync with access2 returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_Sync returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if((res = WaitEndProgram(access2, one_sec)) != PL_OK) {
				sprintf_s(fnd,"nm_prog doesn't finish");
				sprintf_s(exp,"nm_prog has finished");
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
    PL_CloseAccess(access1);
    PL_CloseAccess(access2);
    PL_ResetBoard(board);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


