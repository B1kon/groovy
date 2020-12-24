/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result PL_Close Access does not depend on\n"\
			"the existence of objects Access other descriptor\n"\
			"module with the same index";
    const char *TC="009_04";
	char fnd [256];
    char exp [256];
    int res, i, j, detal=0;
    int test_res = Pass;
    unsigned int count;
#define test_cnt	100000
#define board_cnt	3
    PL_Board *board[board_cnt];
    PL_Access *access[board_cnt];
    struct PL_CoreNo No = {2,2};
	char cmd[256];
    int pass_cnt = 0;;
#define procnt	5
	STARTUPINFO si[procnt];
    PROCESS_INFORMATION pi[procnt];

    tc_id(TC, ASSERTION);

    __try {// Handler Segmentation Fault

		PL_GetBoardCount(&count);
		if((count == 0) || (count == test_cnt)) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0 and count != test_cnt (%d)",test_cnt);
			err_msg(fnd, exp);
			test_res = UnTested;
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
			} else if((res = PL_GetAccess(board[i],&No,&(access[i]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
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
			result_msg(TC, test_res);
			return(test_res);
		}
		system("del *_res_* ");
		// CreateProcesses
		for(i=0; i<procnt; i++){
			wait_mlsec(50);
			ZeroMemory( &(si[i]), sizeof((si[i])) );
				(si[i]).cb = sizeof((si[i]));
			ZeroMemory( &(pi[i]), sizeof((pi[i])) );
			sprintf_s(cmd,"tc009_04_process.exe %d",i);
			if( !CreateProcess( NULL, cmd, NULL, NULL, FALSE,
                               0, NULL, NULL, &(si[i]), &(pi[i]))) {
				sprintf_s(fnd, 256,	"CreateProcess %d failed (%d)",
					i, GetLastError());
				sprintf_s(exp, 256,	"Start Process");
				err_msg(fnd, exp);
				test_res = Fail;
				TerminateProcess((pi[i]).hProcess,NO_ERROR);
			}
		}
		/////////////////////////////////////////////
	    // parent
	    wait_mlsec(50);
		printf("PARENT\n");
		wait_mlsec(250);
		for(i=0; i<procnt; i++){
			char file_res[256];
			int find_res = 0;

			sprintf_s(file_res,"%d_res_009_04",i);
			if ((find_res = check_proc_res(file_res))<0){
				test_res = 0-find_res;
				break;
			}else if (find_res != 0) pass_cnt++;
        }

		for (i=0; i<board_cnt; i++){
		    if ( (res = PL_CloseAccess(access[i]) != PL_OK) & ( i== 0)) {
				sprintf_s(fnd, "PL_CloseAccess returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, "PL_CloseAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
		    }
		    PL_CloseBoardDesc(board[i]);
		}

printf("STEP 9\n");
		for(i=0; i<procnt; i++){
		   	WaitForSingleObject( (pi[i]).hProcess, INFINITE );
			CloseHandle( (pi[i]).hProcess );
			CloseHandle( (pi[i]).hThread );
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
	if(test_res == Pass) {
		if (pass_cnt < procnt){
			sprintf_s(fnd, "Not all processes with Pass pass_cnt=%d procnt=%d",
                            pass_cnt,procnt);
			sprintf_s(exp, "all processes with Pass");
			err_msg(fnd, exp);
			test_res = Fail;
		}
   }
	result_msg(TC, test_res);
	return(test_res);
}


