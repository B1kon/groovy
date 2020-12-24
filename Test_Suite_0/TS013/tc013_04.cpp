/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

PL_Access *access_nm;
PL_Board *board = (PL_Board *)0;

int test_res_thr = Pass;
int res_thr = -10;
char fnd_thr [256];
char exp_thr [256];
int j;
//#define proc  2
//#define core  2
#define proc  2
#define core  1
int sync_res[proc], syncs;
int sync_j[proc];

unsigned __stdcall ThreadFunc( void * arg) // Функция потока
{
    int res = *(int*)arg;
    int v;
	printf("************** ThreadFunc   index = %d j = %d\n",res, j);
    sync_res[res] =  PL_Sync(access_nm,0, &v);
    return(0);
}

int main(int argc, char* argv[]) {

    const char *ASSERTION="Two PL_Sync with the same Access are called.\n"\
		"After call nsl_hostSync (acl_hostSync) response function,\n"\
		"one PL_Sync function will finish with the result PL_OK,\n"\
		"the other with the result PL_TIMEOUT.";

    const char *TC="013_04";
    char fnd [256];
    char exp [256];
    int i, res;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;

    int wait_time = one_sec*3;

    unsigned uThreadIDs[proc]; //идентификатор потока
    HANDLE hThreads[proc];

    struct PL_CoreNo No[] =  {{-1,2}};
    const char* processor[] = { "ARM"};
    const char *prog[] = {"arm_prog.elf"};

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

		for(j=0; j<proc; j++) sync_res[j]=-2;
			syncs = 0;
		for(i=0; i<core; i++){
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
			for(j=0; j<proc; j++){
				sync_j[j]=j;
				hThreads[j] = (HANDLE)_beginthreadex( NULL, 0, &ThreadFunc,
									&sync_j[j], 0,&uThreadIDs[j]);
			}
			
			// wait starting of the ThreadFunc
			wait_mlsec(one_sec); // wait 1 second

			if ((res = PL_LoadProgramFile(access_nm, prog[i])) != PL_OK) {
				sprintf_s(fnd, "PL_LoadProgramFile returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp,"PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				for(j=0; j<proc; j++){
					TerminateThread( hThreads[j], -1);
					WaitForSingleObject(hThreads[j], 10000L); // 10 sec
				}
				break;
			}
			wait_mlsec(one_sec*2);
			wait_mlsec(100);

			for(j=0; j<proc; j++){
				if(sync_res[j] == PL_OK){
					syncs += PL_OK;
				}else if(sync_res[j] == PL_TIMEOUT){
					syncs += PL_TIMEOUT;
				}
			}
			if ( syncs != (PL_OK + PL_TIMEOUT)) {
				sprintf_s(fnd,
					"Core %s PL_Sync results sync_res[%d]=%d sync_res[%d]=%d",
					processor[i],0,sync_res[0],1,sync_res[1]);
				sprintf_s(exp,
					"Result of the one PL_Sync is PL_OK, other one is PL_TIMEOUT");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			for(j=0; j<proc; j++){
				TerminateThread( hThreads[j], -1);
				WaitForSingleObject(hThreads[j], 10000L); // 10 sec
			}

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


