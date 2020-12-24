/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

PL_Access *access_nm;
PL_Board *board = (PL_Board *)0;

int test_res_thr = 11111;
int res_thr = 0;
char fnd_thr [256];
char exp_thr [256];

unsigned __stdcall ThreadFunc( void * arg) // Функция потока
{
    int res;
	test_res_thr = 11111;
    res_thr = -10;
    if ((res = PL_Sync(access_nm, 0 ,NULL)) != PL_TIMEOUT) {
		sprintf_s(fnd_thr, "PL_Sync returns %s %d",sp_error_name[res], res);
		sprintf_s(exp_thr, "PL_Sync return PL_TIMEOUT");
		test_res_thr = Fail;
    }else{
		test_res_thr = Pass;
    }
    res_thr = -100; // end the thread
    return(0);
}


int main(int argc, char* argv[]) {

    const char *ASSERTION="If two PL_Sync(PL_SyncArray) functions are running\n"\
		"for the same Access and response function ncl_hostSync (acl_hostSync)\n"\
		"are not running, then the result of this functions are PL_TIMEOUT.\n"\
		"Timeout is set.";
    const char *TC="014_04";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;
	
    unsigned uThreadIDs; //идентификатор потока
    HANDLE hThreads;

    int wait_time = one_sec*2;
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char* processor[] = {"NM", "ARM"};
    PL_Word L, A;
	int v;

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
			hThreads = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc,0, 0,&uThreadIDs);
			
			// wait starting of the first ThreadFunc
			while (res_thr != -10) {// wait start
				wait_mlsec(10); // wait 10 milliseconds
			}

			if ((res = PL_SyncArray(access_nm, 5,5,5, &v,&A,&L)) != PL_TIMEOUT) {
				sprintf_s(fnd,"PL_SyncArray returns %s %d",sp_error_name[res], res);
				sprintf_s(exp,"PL_SyncArray returns PL_AGAIN");
				err_msg(fnd, exp);
				test_res = Fail;
			} 

			while (res_thr != -100) {// wait start
				wait_mlsec(10); // wait 10 milliseconds
			}
			TerminateThread( hThreads, -1);
			WaitForSingleObject(hThreads, 10000L); // 10 sec

			PL_CloseAccess(access_nm);
			if((test_res_thr != Pass) || (test_res !=Pass)){
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
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);

}


