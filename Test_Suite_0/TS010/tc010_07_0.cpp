/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result PL_LoadProgramFile = PL_ERROR if\n"\
			"loading is performed during operation of the other\n"\
			"nm-program (for NM-processor) or "\
			"arm-program (for ARM processor)\n"\
			"Case – one Access in one task.";
    const char *TC="010_07_0";
    char fnd [256];
    char exp [256];
    int res,i;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    const char *nm_prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    const char* processor[] = {"NM", "ARM"};
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};

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
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for(i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &(No[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_LoadProgramFile(access, nm_prog[i])) != PL_OK) {
				sprintf_s(fnd, "Case %s. PL_LoadProgramFile returns %s %d",
						processor[i],sp_error_name[res], res);
				sprintf_s(exp, "First PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_LoadProgramFile(access, nm_prog[i])) != PL_ERROR) {
				sprintf_s(fnd, "Case %s.PL_LoadProgramFile returns %s %d",
						processor[i],sp_error_name[res], res);
				sprintf_s(exp, "Second PL_LoadProgramFile returns PL_ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
			}
			PL_CloseAccess(access);
			if (test_res !=Pass)  break;
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
    PL_ResetBoard(board);
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


