/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>



int main(int argc, char* argv[]) {

    const char *ASSERTION="Process for tc010_07_1";
    const char *TC="010_07_1_Process";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;

    #define test_cnt	100000
    unsigned int count = test_cnt;
	PL_Access *access_nm;
    PL_Board *board = (PL_Board *)0;
    const char* processor[] = {"NM", "ARM"};
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char *nm_prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    FILE *fd;
    char file_res[128];
    char result[128];
    const char *result_file="result_010_07";
    char *pid;
    int wait_time = one_sec*3;

	tc_id(TC, ASSERTION);
	__try {// Handler Segmentation Fault

		printf("argc=%d argv[1]=%s\n",argc,argv[1]);
		pid=argv[1];
		sprintf_s(file_res,"%s_res_010_07",pid);
		i=atoi(pid);
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
		} else if ((res = PL_GetAccess(board, &(No[i]), &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
							sp_error_name[res], res);
			sprintf_s(exp,"PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			sprintf_s(result,"%s\n","Fail");
		}else if ((res = PL_LoadProgramFile(access_nm, nm_prog[i])) != PL_OK){
			sprintf_s(fnd, "Case %s. PL_LoadProgramFile returns %s %d",
							processor[i],sp_error_name[res], res);
			sprintf_s(exp, "First PL_LoadProgramFile return PL_OK");
			test_res = Fail;
			sprintf_s(result,"%s\n","Fail");
		}else {
			sprintf_s(result,"%s\n","Pass");
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
			system("echo \"Fail\" > _res_010_07");
	} else {
			fputs(result,fd);
	}
#pragma warning( pop )
	fclose(fd);
	printf("step 2 pid=%s\n",pid);
	wait_mlsec(6000); // 1 min
    return(test_res);
}


