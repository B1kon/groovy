/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If NM or ARM program is not completed or\n"\
			"not started, then the result PL_GetStatus= PL_OK\n"\
			"and status is not equal PROGRAM_FINISHED.";
    const char *TC="015_02";
    char fnd [256];
    char exp [256];
    char msg [256];
    int res, i, j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    int wait_time = 500; // 0.5 sec
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char* processor[] = {"NM", "ARM"};
    const char *prog[] = {"nm_prog.abs","arm_prog.elf"};
    int start[] = {0,1}; // 0-NM or ARM program is not started
			 // 1-NM or ARM program is not completed
//	const char* prog = "nm_prog.abs";
    PL_Word  status;
	int syncValue;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		PL_SetTimeout(wait_time);
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

		for(j=0; j<2; j++){ //0-not started; 1-not completed
			for(i=0; i<2; i++){
				status = 1000;
				if ((res = PL_GetAccess(board, &(No[i]), &access)) != PL_OK) {
					sprintf_s(fnd, "PL_GetAccess returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp, "PL_GetAccess returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
				if(j == 1) { //1-not completed
					sprintf_s(msg,"program is started but not completed ");
					if ((res = PL_LoadProgramFile(access, prog[i])) != PL_OK) {
						sprintf_s(fnd,"PL_LoadProgramFile returns %s %d",
								sp_error_name[res], res);
						sprintf_s(exp,"PL_LoadProgramFile returns PL_OK");
						err_msg(fnd, exp);
						test_res = Fail;
						break;
					}
				}else sprintf_s(msg,"program is not started ");
	    
				if((res = PL_GetStatus(access, &status)) != PL_OK) {
					sprintf_s(fnd,"PL_GetStatus returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp,"PL_GetStatus returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
				}else if (status == PROGRAM_FINISHED) {
					sprintf_s(fnd, "%s but status == PROGRAM_FINISHED",msg);
					sprintf_s(exp,"status -is not equal PROGRAM_FINISHED");
					err_msg(fnd, exp);
					test_res = Fail;
				}
				if (test_res != Pass) break;
				if(j == 1) { //1-not completed
					PL_Sync(access,0,&syncValue);
				}
			}
			PL_CloseAccess(access);
			if (test_res != Pass) break;
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
    PL_Sync(access,0,&syncValue);
    PL_ResetBoard(board);
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


