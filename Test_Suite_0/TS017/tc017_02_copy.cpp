/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>
#include <setjmp.h>

static sigjmp_buf sj_env;
volatile int Int_count = 0;
void handler (int signum) {
	Int_count++;
	printf("8888 %d\n",Int_count);
	siglongjmp (sj_env, 1);
}

PL_Access *access_nm;
PL_Board *board = (PL_Board *)0;
struct timeval start_time, end_time;
time_t s, ms, ml=0;
int test_res_thr = Pass;
int res_thr = -10;
char fnd_thr [256];
char exp_thr [256];

void * ThreadFunc( void * arg) // Функция потока
{
    int res;
    gettimeofday(&start_time, NULL);
    res_thr = PL_OK;
    if ((res = PL_Sync(access_nm, 0 ,NULL)) != PL_TIMEOUT) {
	sprintf(fnd_thr, "PL_Sync returns %s %d",
			sp_error_name[res], res);
	sprintf(exp_thr, "PL_Sync return PL_TIMEOUT");
	test_res_thr = Fail;
    }
    gettimeofday(&end_time, NULL);
    res_thr = -100; // end the thread
    return(0);
}


int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_SetTimeou changes Timeout value without\n"\
			"matter on PL_Sync or PL_SyncArray";
    const char *TC="017_02";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;
	
    struct PL_CoreNo No={-1,2};
	
    pthread_t thread; //идентификатор потока
    int uThreadIDs;
    int wait_long = one_sec*4;
    int wait_short = one_sec*2;
    PL_Word  result;

    struct sigaction act;

    tc_id(TC, ASSERTION);

    act.sa_handler = handler;
    act.sa_flags = SA_RESETHAND;
    // signal(SIGSEGV, handler);
    if ((res = sigaction(SIGSEGV, &act, NULL)) < 0) {
    	sprintf(fnd, "sigaction returns %d", res);
    	sprintf (exp, "sigaction returns 0");
	err_msg(fnd, exp);
	test_res = UnTested;
	result_msg(TC, test_res);
	return(test_res);
    }
    // place for long jump
    if (sigsetjmp(sj_env, 1)) {
	sprintf(fnd, "SIGSEGV has happened");
	sprintf (exp, "SIGSEGV should not be");
	err_msg(fnd, exp);
	test_res = Fail;
	result_msg(TC, test_res);
	return(test_res);
    }


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
    }
    if (test_res !=Pass){
	result_msg(TC, test_res);
        return(test_res);
    }

     if ((res = PL_GetAccess(board, &No, &access_nm)) != PL_OK) {
	    sprintf_s(fnd, "PL_GetAccess returns %s %d",
		sp_error_name[res], res);
	    sprintf_s(exp, "PL_GetAccess returns PL_OK");
	    err_msg(fnd, exp);
	    test_res = Fail;
    }

    pthread_create(&thread, NULL, ThreadFunc, &uThreadIDs);
			
    // wait starting of hThreads
    while (res_thr == -10) // wait start
    {
		wait_mlsec(10); // wait 10 milliseconds
    }

    wait_mlsec(one_sec);
    PL_SetTimeout(wait_short);
    // wait the end of hThreads
    while (res_thr != -100) // wait end
    {
		wait_mlsec(10); // wait 10 milliseconds
    }
    s = end_time.tv_sec - start_time.tv_sec;
    ms = end_time.tv_usec - start_time.tv_usec;
    if(ms<0){ s--; ms+=1000000; }
    ml = (s*1000)+(ms/1000); //ml - waiting time in milliseconds
	
    if( (((DWORD)wait_short) > ml) || (( ((DWORD)wait_short)+100) < ml)){
printf("end_time.tv_sec %d\n", end_time.tv_sec);
printf("start_time.tv_sec %d\n",start_time.tv_sec);
printf("                  %d\n",s);
	sprintf(fnd,"PL_Sync wiated %d mls",ml);
	sprintf(exp,"PL_Sync wiated abaut %d",wait_short);
	err_msg(fnd, exp);
	test_res = Fail;
    }
    pthread_cancel(thread);
    pthread_join(thread, NULL);
printf("PL_Sync wiated %d mls\n", ml);
    PL_ResetBoard(board);
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);

}


