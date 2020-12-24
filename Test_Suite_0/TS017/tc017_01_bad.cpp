/*
    Реализация тестового набора для БЗО МС12705.
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Set  Timeout is for all assecc objects";
    const char *TC="017_01";
#define test_cnt	100000

 //   PL_Board *board = (PL_Board *)0;
//    PL_Access *access_nm;
    int i;
    int test_res_th = 11111;

    unsigned int count = test_cnt;
    struct PL_CoreNo NM_PC = {2,2};
    int wait_time = one_sec*2; //3;
    char fnd [256];
    char exp [256];
	char cmd [256];
    int test_res = Pass;
    int pass_cnt = 0;
#define procnt	5
	STARTUPINFO si[procnt];
    PROCESS_INFORMATION pi[procnt];
	
    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		system("del -f *_res_*  result_017_01  res_*");
		PL_SetTimeout(wait_time);

		// CreateProcesses
		for(i=0; i<procnt; i++){
			wait_mlsec(50);
			ZeroMemory( &(si[i]), sizeof(si[i]) );
			(si[i]).cb = sizeof((si[i]));
			ZeroMemory( &(pi[i]), sizeof(pi[i]) );
			sprintf_s(cmd,"tc017_01_process.exe %d\n",1);
			if( !CreateProcess( NULL, cmd, NULL, NULL, FALSE,
					0, NULL, NULL, &(si[i]), &(pi[i]))) {
				sprintf_s(fnd, 256,	"CreateProcess failed (%d)", GetLastError());
				sprintf_s(exp, 256,	"Start Process");
				err_msg(fnd, exp);
				test_res = Fail;
				TerminateProcess((pi[i]).hProcess,NO_ERROR);
			}
		}
		// parent
	
		wait_mlsec(wait_time);
		wait_mlsec(500);
	    printf("PARENT\n");

		// check processes result
		for(i=0; i<procnt; i++){
			char file_res[256];
			int find_res = 0;

			sprintf_s(file_res,"%d_res_017_01",i);
			if ((find_res = check_proc_res(file_res))<0){
				test_res = 0-find_res;
				break;
			}else if (find_res != 0) pass_cnt++;
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
//printf("STEP 9\n");
	for(i=0; i<procnt; i++){
	   	WaitForSingleObject( (pi[i]).hProcess, INFINITE );
		CloseHandle( (pi[i]).hProcess );
		CloseHandle( (pi[i]).hThread );
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


