/*
    Реализация тестового набора для БЗО МС12705.
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {
    char *ASSERTION="Process for tc017_01";
    char *TC="017_01_Process";
#define test_cnt	100000
    char fnd [256];
    char exp [256];
    PL_Board *board = (PL_Board *)0;
    PL_Access *access_nm;
    int test_res = 11111;

    unsigned int count = test_cnt;
    struct PL_CoreNo NM_PC = {2,2};
    int wait_time = one_sec*2; //3;
    int res;
     char* pid;
    FILE *fd;
    char file_res[128];
    char result[128];
    time_t start_time, end_time, ml=0;

	tc_id(TC, ASSERTION);
	__try {// Handler Segmentation Fault
    wait_mlsec(50);

 		printf("argc=%d argv[1]=%s\n",argc,argv[1]);
		pid=argv[1];
		sprintf_s(file_res,"%s_res_017_01",pid);

		PL_GetBoardCount(&count);
		if((count == 0) || (count == test_cnt)) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0 and count != test_cnt (%d)",test_cnt);
			err_msg(fnd, exp);
			test_res = UnTested;
			sprintf_s(result,"%s\n","Fail");
		} else if ((res = PL_GetBoardDesc(0, &board)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d",
						sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			sprintf_s(result,"%s\n","Fail");
		}else if ((res = PL_GetAccess(board, &NM_PC, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
			sprintf_s(exp,"PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			sprintf_s(result,"%s\n","Fail");
		}else {
			start_time = GetTickCount();
			if ((res = PL_Sync(access_nm, 0 ,NULL)) != PL_TIMEOUT) {
				sprintf_s(fnd, "PL_Sync returns %s %d",
					sp_error_name[res], res);
				sprintf_s(exp, "PL_Sync return PL_TIMEOUT");
				err_msg(fnd, exp);
				test_res = Fail;
				sprintf_s(result,"%s\n","Fail");
			}else {
				end_time = GetTickCount();
				ml = end_time - start_time; //ml - waiting time in milliseconds
				if( ((DWORD)wait_time) > (ml+100)){
					sprintf_s(result,"%s\n","Fail");
				}else{
					sprintf_s(result,"%s\n","Pass");
				}
			}
		}
	} // End __try
	__except((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? \
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
	{
		printf("ERROR: Found SIGSEGV\n");
		sprintf_s(fnd, 256, "Created Process. ERROR: Found SIGSEGV");
		sprintf_s (exp, 256, "the test without any exceptions");
		err_msg(fnd, exp);
		test_res = UnTested;
	}
#pragma warning( push )
#pragma warning( disable : 4996 )
	if((fd=fopen(file_res, "wb")) == NULL){
		sprintf_s(fnd, "can not open %s",file_res);
		sprintf_s(exp, "open %s",file_res);
		err_msg(fnd, exp);
		test_res = UnTested;
		system("echo \"Fail\" > _res_017_01");
	} else {
		fputs(result,fd);
	}
#pragma warning( pop )
	fclose(fd);
       printf("step 2 pid=%d\n",atoi(pid));
	wait_mlsec(6000); // 1 min

    return(test_res);
}
