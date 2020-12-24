/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>


int main(int argc, char* argv[]) {

    const char *ASSERTION="Stres test. Work with ARM,NM. Use only clusters\n"\
	"For each processor separate process and separate Access object.\n"\
	"Argument:\n"\
	"        working time in seconds\n"\
	"        processors count";
    const char *TC="102_01";
    char fnd [256];
    char exp [256];
	PL_Board *board = (PL_Board *)0;
    int res, i, k;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
	int process_res = Pass;
	//ccpu + clusteers*5 = 21
	#define   pc_max	21
    int pc_cnt, nm, arm, pass_cnt;
	char file_res [256];
	char cmd [256];
	STARTUPINFO si[pc_max];
    PROCESS_INFORMATION pi[pc_max];

    const char *result_file="result_102_01";
    int work_time, work; //in seconds
    int wait_time = one_sec*3;
	time_t start_time, end_time, ml=0;

    tc_id(TC, ASSERTION);
	__try {// Handler Segmentation Fault

		if(argc != 3){
			printf("Argument count != 3. argc=%d\n",argc);
			test_res = UnTested;
			result_msg(TC, test_res);
			return(test_res);
		}

		work_time = atoi(argv[1]);
		pc_cnt = atoi(argv[2]);
		work = work_time *1000; //in msec
		printf("working time = %dsec  %dms  pc count = %d\n",work_time,work,pc_cnt);
		if(pc_cnt > pc_max){
			printf("Argument argv[2]=%s  more pc_max=%d\n",argv[2],pc_max);
			test_res = UnTested;
			result_msg(TC, test_res);
			return(test_res);
		}

		system("del  coreout_* result_102_01 res_102_01");
		for(i=0; i<pc_cnt; i++){
			ZeroMemory( &(si[i]), sizeof((si[i])) );
			(si[i]).cb = sizeof((si[i]));
			ZeroMemory( &(pi[i]), sizeof((pi[i])) );
			sprintf_s(cmd,"tc102_01_process.exe %d %d",i,work);
			printf("CreateProcess %d  %s\n",i,cmd);
			if( CreateProcess( NULL, cmd, NULL, NULL, FALSE,
					0, NULL, NULL, &(si[i]), &(pi[i])) != TRUE) {
				sprintf_s(fnd,"CreateProcess failed (%d)",GetLastError());
				sprintf_s(exp,	"Start Process");
				err_msg(fnd, exp);
				test_res = Fail;
				TerminateProcess((pi[i]).hProcess,NO_ERROR);
			}
		}
			// parent

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
		wait_mlsec(500);
		if(test_res != Pass) {
			for(k=0; k<pc_cnt; k++){
				TerminateProcess((pi[k]).hProcess,NO_ERROR);
			}
			PL_ResetBoard(board);
			PL_CloseBoardDesc(board);
		    result_msg(TC, test_res);
			return(test_res);
		}

	    for(k=0; k<(work_time*2); k++){
			wait_mlsec(500); //0.5s
	    }
    // test finish
		for(k=0; k<pc_cnt; k++){
			TerminateProcess((pi[k]).hProcess,NO_ERROR);
		}
		wait_mlsec(500);
		for(k=0; k<pc_cnt; k++){
			process_res = Pass;
			arm = k/5;
			nm = k%5;
			if (nm == 0) arm--;
			nm--;
			sprintf_s(file_res,"coreout_%d,%d",nm,arm);
			if ((pass_cnt = check_proc_res(file_res))<0){
				process_res = 0-pass_cnt;
			}
			if( (process_res != Pass) || (pass_cnt < 1)){
				sprintf_s(fnd, "process nmb=%d nm=%d arm=%d has Faild\n"\
				"process_res=%d pass_cnt=%d\n",k,nm,arm,process_res,pass_cnt);
				sprintf_s(exp, "All processes are passed");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
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
	PL_ResetBoard(board);
	PL_CloseBoardDesc(board);
	result_msg(TC, test_res);
    return(test_res);

}


