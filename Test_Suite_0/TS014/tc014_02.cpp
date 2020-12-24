/*
    Реализация тестового набора для БЗО МС12705.
*/
#include <common_mc12705_tc.h>

PL_Access *access_nm;
PL_Board *board = (PL_Board *)0;

int test_res_thr = Pass;
int res_thr = -10;
char fnd_thr [256];
char exp_thr [256];
DWORD wait_time = one_sec*3;
int v;
PL_Word L, A;

unsigned __stdcall ThreadFunc( void * arg) // Функция потока
{
	int res;
	res_thr = PL_OK;
	if ((res = PL_SyncArray(access_nm, 5,5,5, &v,&A,&L)) != PL_TIMEOUT) {
		sprintf_s(fnd_thr, "PL_SyncArray returns %s %d",
					sp_error_name[res], res);
		sprintf_s(exp_thr, "PL_SyncArray return PL_TIMEOUT");
		test_res_thr = Fail;
	}
	res_thr = -100; // end the thread
	return(0);
}

int main(int argc, char* argv[]) {

    const char *ASSERTION="If synchronization has not done in time,\n"\
			"defined by function PL_SetTimeout,\n"\
			"then PL_SyncArray returns PL_TIMEOUT";
    const char *TC="014_02";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    unsigned uThreadIDs; //идентификатор потока
    HANDLE hThreads;

    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char* processor[] = {"NM", "ARM"};

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

		for(i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &(No[i]), &access_nm)) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}
			if (test_res !=Pass){
				break;
			}

			hThreads = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc,
									0, 0,&uThreadIDs);
		
			wait_mlsec(wait_time-one_sec);
			if (res_thr == -100) {
				sprintf_s(fnd,"PL_SyncArray dosn't wait timeout");
				sprintf_s(exp, "PL_SyncArray waits timeout");
				err_msg(fnd, exp);
				test_res = Fail;
			}else {
				wait_mlsec(one_sec*2);
				// should be TIMEOUT
				if (res_thr != -100) {
					sprintf_s(fnd, "PL_SyncArray steel wait");
					sprintf_s(exp, "PL_SyncArray returns PL_TIMEOUT");
					err_msg(fnd, exp);
					test_res = Fail;
				}else if ((test_res == Pass) && (test_res_thr != Pass) ){
					test_res = test_res_thr;
					err_msg(fnd_thr, exp_thr);
				}
			}
			TerminateThread( hThreads, -1);
			WaitForSingleObject(hThreads, 10000L); // 10 sec

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


