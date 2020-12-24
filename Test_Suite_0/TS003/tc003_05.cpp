/*
    Реализация тестового набора для БЗО МС12705.
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result of a repeated call of PL_GetBoardDesc\n"\
		"with the same index, but in other task, will be PL_OK";
    const char *TC="003_05";
    char fnd [256];
    char exp [256];
    int res;
    int test_res = Pass;
    #define test_cnt	100000
    PL_Board *board = (PL_Board *)0;
    int i;
    unsigned int count = test_cnt;
    const char *res_file="res_003_05";
#define procnt	5
    STARTUPINFO si[procnt];
	PROCESS_INFORMATION pi[procnt];
    char cmd[256];
	int procerr = 0;
	int pass_cnt = 0;
	
    tc_id(TC, ASSERTION);
	__try {// Handler Segmentation Fault

		system("del *_res_* result_003_05");

		PL_GetBoardCount(&count);
		if((count == 0) || (count == test_cnt)) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0 and count != test_cnt (%d)",test_cnt);
			err_msg(fnd, exp);
			test_res = UnTested;
		} else if ((res = PL_GetBoardDesc(0, &board)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardDes returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res != Pass) {
			result_msg(TC, test_res);
			return(test_res);
		}

		for(i=0; i<procnt; i++){
			wait_mlsec(50);
			// CreateProcesses
			ZeroMemory( &(si[i]), sizeof((si[i])) );
				(si[i]).cb = sizeof((si[i]));
			ZeroMemory( &(pi[i]), sizeof((pi[i])) );
			sprintf_s(cmd,"tc003_05_process.exe %d",i);
			if( !CreateProcess( NULL, cmd, NULL, NULL, FALSE,
                               0, NULL, NULL, &(si[i]), &(pi[i]))) {
				sprintf_s(fnd, 256,	"CreateProcess %d failed (%d)",
					i, GetLastError());
				sprintf_s(exp, 256,	"Start Process");
				err_msg(fnd, exp);
				test_res = Fail;
				procerr++;
				TerminateProcess((pi[i]).hProcess,NO_ERROR);
			}
		}

		wait_mlsec(50);
        printf("PARENT\n");
        wait_mlsec(250);
        // check processes result
		for(i=0; i<procnt; i++){
			char file_res[256];
            int find_res = 0;

			sprintf_s(file_res,"%d_res_003_05",i);
            if ((find_res = check_proc_res(file_res))<0){
               test_res = 0-find_res;
               break;
            }else if (find_res != 0) pass_cnt++;
        }
    printf("pass_cnt=%d test_res=%d\n",pass_cnt,test_res);

//printf("STEP 9\n");
		// Close process and thread handles.
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
	PL_CloseBoardDesc(board);
	result_msg(TC, test_res);
	return(test_res);
}




