/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Function PL_GetAccess can create more then\n"\
		"one object Access for the same index.\n"\
		"Calling PL_GetAccess in a other tasks";
    const char *TC="008_04_1";
    char fnd [256];
    char exp [256];
    int res;
    int test_res = Pass;
    PL_Access *access_nm;
    PL_Board *board = (PL_Board *)0;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    struct PL_CoreNo NM_PC = {2,2};
    const char *nm_prog = "nm_prog_04.abs";
 //   struct stat buf;
	char cmd[256];
    int wait_time = one_sec*3;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	int pass_cnt = 0;
	char file_res[256];

    tc_id(TC, ASSERTION);

    system("del *_res_* result_008_05");

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
		if (test_res != Pass) {
			result_msg(TC, test_res);
			return(test_res);
		}

		// CreateProcesses
		ZeroMemory( &(si), sizeof(si) );
		(si).cb = sizeof((si));
		ZeroMemory( &(pi), sizeof(pi) );
		sprintf_s(cmd,"tc008_04_1_process.exe %d",0);
		if( !CreateProcess( NULL, cmd, NULL, NULL, FALSE,
					0, NULL, NULL, &(si), &(pi))) {
			sprintf_s(fnd, 256,	"CreateProcess failed (%d)", GetLastError());
			sprintf_s(exp, 256,	"Start Process");
			err_msg(fnd, exp);
			test_res = Fail;
			TerminateProcess((pi).hProcess,NO_ERROR);
			
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		}
		/////////////////////////////////////////////
		wait_mlsec(50);
		// parent
		if ((res = PL_GetAccess(board, &NM_PC, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp,"PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if ((res = PL_LoadProgramFile(access_nm, nm_prog)) != PL_OK) {
			sprintf_s(fnd, "PL_LoadProgramFile with access_2_nm returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			err_msg(fnd, exp);
		}else{
			wait_mlsec(50);
			printf("PARENT\n");
			wait_mlsec(250);
			sprintf_s(file_res,"0_res_008_04_1");
			if ((pass_cnt = check_proc_res(file_res))<0){
				test_res = 0-pass_cnt;
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
	
	WaitForSingleObject( (pi).hProcess, INFINITE );
	CloseHandle( (pi).hProcess );
	CloseHandle( (pi).hThread );
	
	if(test_res == Pass) {
		if (pass_cnt < 1){
			sprintf_s(fnd, "Not all processes with Pass pass_cnt=%d procnt=1",
                            pass_cnt);
			sprintf_s(exp, "all processes with Pass");
			err_msg(fnd, exp);
			test_res = Fail;
		}
	}

    PL_ResetBoard(board);
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


