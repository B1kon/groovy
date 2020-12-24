/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

PL_Access *access_nm;
PL_Board *board = (PL_Board *)0;
time_t start_time, end_time, ml=0;
int test_res_thr = Pass;
int res_thr = -10;
char fnd_thr [256];
char exp_thr [256];

unsigned __stdcall ThreadFunc( void * arg) // Функция потока
{
    int res;
    start_time = GetTickCount();
    res_thr = PL_OK;
    if ((res = PL_Sync(access_nm, 0 ,NULL)) != PL_TIMEOUT) {
		sprintf_s(fnd_thr, "PL_Sync returns %s %d",
				sp_error_name[res], res);
		sprintf_s(exp_thr, "PL_Sync return PL_TIMEOUT");
		test_res_thr = Fail;
    }
    end_time = GetTickCount();
    res_thr = -100; // end the thread
    return(0);
}


int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_SetTimeou changes Timeout value without\n"\
			"matter on PL_Sync or PL_SyncArray";
    const char *TC="017_02";
    char fnd [256];
    char exp [256];
    int res;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;
	
    struct PL_CoreNo No={-1,2};
	
    unsigned uThreadIDs; //идентификатор потока
    HANDLE hThreads;
    int wait_long = one_sec*4;
    int wait_short = one_sec*2;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		PL_SetTimeout(wait_long);
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
		}else if ((res = PL_GetAccess(board, &No, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
			return(test_res);
		}

		hThreads = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc,
									0, 0,&uThreadIDs);
			
		// wait starting of hThreads
		while (res_thr == -10) // wait start
		{
			wait_mlsec(10); // wait 10 milliseconds
		}

		wait_mlsec(one_sec);
		//    PL_SetTimeout(wait_short);
		if ((res = PL_SetTimeout(wait_short)) != PL_OK) {
			sprintf_s(fnd, "PL_SetTimeout returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_SetTimeout returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else{
			// wait the end of hThreads
			while (res_thr != -100) {// wait end
				wait_mlsec(10); // wait 10 milliseconds
			}
			ml = end_time - start_time; //ml - waiting time in milliseconds
	
			if( (((DWORD)wait_short) > ml) || (( ((DWORD)wait_short)+100) < ml)){
printf("end_time %lld\n", end_time);
printf("start_time %lld\n",start_time);
printf("                  %lld\n",ml);
				sprintf_s(fnd,"PL_Sync wiated %lld mls",ml);
				sprintf_s(exp,"PL_Sync wiated abaut %d",wait_short);
				err_msg(fnd, exp);
				test_res = Fail;
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
	TerminateThread( hThreads, -1);
	WaitForSingleObject(hThreads, 10000L); // 10 sec
printf("PL_Sync wiated %lld mls\n", ml);
    PL_ResetBoard(board);
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);

}


