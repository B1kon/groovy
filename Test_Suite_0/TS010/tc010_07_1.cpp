/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Created two accesses with the same index "\
			"in different tasks.\n"\
			"If the program is loaded on one Access, then result\n"\
			"of loading the program on another Access is PL_ERROR.\n";
    const char *TC="010_07_1";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    const char* processor[] = {"NM", "ARM"};
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char *nm_prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    const char *result_file="result_010_07";
	char cmd[256];
	int pass_cnt = 0;
	char file_res[256];
 #define procnt	2
 	PL_Access *access_nm;
    PL_Board *board = (PL_Board *)0;
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

//    pthread_t thread; //идентификатор потока
//    int uThreadIDs;
//    struct timeval start_time, end_time;
//    time_t s, ms, ml=0;
    int wait_time = one_sec*3;


    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault
		system("del /Q *_res_* result_010_07");
		
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

		for(i=0; i<procnt; i++){
			ZeroMemory( &(si), sizeof(si) );
			si.cb = sizeof((si));
			ZeroMemory( &(pi), sizeof(pi) );
			sprintf_s(cmd,"tc010_07_1_process.exe %d",i);
			if( !CreateProcess( NULL, cmd, NULL, NULL, FALSE,
					0, NULL, NULL, &(si), &(pi))) {
				sprintf_s(fnd, 256,	"CreateProcess failed (%d)", GetLastError());
				sprintf_s(exp, 256,	"Start Process");
				err_msg(fnd, exp);
				test_res = Fail;
				TerminateProcess((pi).hProcess,NO_ERROR);
			}
		
			// parent
			wait_mlsec(250);
			printf("PARENT\n");
			// check processes result

			sprintf_s(file_res,"%d_res_010_07",i);
			if ((pass_cnt = check_proc_res(file_res))<0){
				test_res = 0-pass_cnt;
			}

			if ((res = PL_GetAccess(board, &(No[i]), &access_nm)) != PL_OK) {
				sprintf_s(fnd, "step=%d PL_GetAccess returns %s %d",
						i,sp_error_name[res], res);
				sprintf_s(exp,"PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}else if((res=PL_LoadProgramFile(access_nm,nm_prog[i])) != PL_ERROR){
				sprintf_s(fnd, "Case %s. PL_LoadProgramFile returns %s %d",
								processor[i],sp_error_name[res], res);
				sprintf_s(exp, "Second PL_LoadProgramFile returns PL_ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
				err_msg(fnd, exp);
			}else if (pass_cnt < 1){
				sprintf_s(fnd, "Not all processes with Pass pass_cnt=%d procnt=1",
                            pass_cnt);
				sprintf_s(exp, "all processes with Pass");
				err_msg(fnd, exp);
				test_res = Fail;
			}
			WaitForSingleObject( (pi).hProcess, INFINITE );
			CloseHandle( (pi).hProcess );
			CloseHandle( (pi).hThread );
			PL_CloseAccess(access_nm);
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
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


