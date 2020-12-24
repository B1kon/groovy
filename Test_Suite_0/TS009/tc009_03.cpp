/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>
/////////////////////////////
//#define PL_AGAIN 5
///////////////////////////
PL_Board *board = (PL_Board *)0;
PL_Access *access_nm;
int test_res_thr = Pass;
int res_thr = -10;
char fnd_thr [256];
char exp_thr [256];

unsigned __stdcall ThreadFunc( void * arg) // Функция потока
{
	int res;
	res_thr = PL_OK;
	if ((res = PL_Sync(access_nm, 0 ,NULL)) != PL_TIMEOUT) {
		sprintf_s(fnd_thr, "PL_Sync returns %s %d",
					sp_error_name[res], res);
		sprintf_s(exp_thr, "PL_Sync return PL_TIMEOUT");
		test_res_thr = Fail;
	}
	res_thr = -100; // end the thread
	return(0);
}

int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_CloseAccess returns PL_AGAIN if\n"\
			"other operation LLC is working.";
    const char *TC="009_03";
    char fnd [256];
    char exp [256];
    int res;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;
	
    struct PL_CoreNo No = {2,2};
    unsigned uThreadIDs; //идентификатор потока
    HANDLE hThreads;
    time_t start_time, end_time, ml=0;
    int wait_time = one_sec*3;

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
		} else if ((res = PL_GetAccess(board, &No, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp,"PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res !=Pass){
			PL_ResetBoard(board);
			PL_CloseAccess(access_nm);
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		}

		hThreads = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc,
									0, 0,&uThreadIDs);
									
			
		// wait starting of hThreads
		while (res_thr == -10) // wait start
		{
			wait_mlsec(10); // wait 100 milliseconds
		}
		wait_mlsec(100);
		start_time = GetTickCount();

		if ((res = PL_CloseAccess(access_nm)) != PL_AGAIN) {
			sprintf_s(fnd, "PL_CloseAccess returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_CloseAccess returns PL_AGAIN");
			err_msg(fnd, exp);
			test_res = Fail;
		}else{
			end_time = GetTickCount();
			ml = end_time - start_time; //ml - waiting time in milliseconds

			if( ((DWORD)wait_time) < ml){
				sprintf_s(fnd, "PL_CloseAccess wiats PL_Sync completion ml=%lld",ml);
				sprintf_s(exp, "PL_CloseAccess returns PL_AGAIN");
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

    PL_ResetBoard(board);
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);

}


