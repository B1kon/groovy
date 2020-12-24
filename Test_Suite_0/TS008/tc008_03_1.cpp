/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If coreNo.nm_id value is equal -1,\n"\
		"and coreNo.cluster_id value is in segment [-1, 3], then\n"\
		"result of PL_GetAccess = PL_OK and the argument 'access'\n"\
		"refers to ARM object according to coreNo";
    const char *TC="008_03_1";
    char fnd [256];
    char exp [256];
    int res, i;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access;
    const char *arm_prog = "arm_prog_03.elf";

    struct PL_CoreNo coreNo[2];
    int cluster_list[2];
    int cluster_id;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

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
			test_res = UnTested;
		} 
		if(test_res != Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		coreNo[0]={-1,-1};
		coreNo[1]={-1,3};
		cluster_list[0]=4;
		cluster_list[1]=3;

		for (i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &coreNo[i], &access)) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d, coreNo={%d,%d}",
				sp_error_name[res], res, coreNo[i].nm_id, coreNo[i].cluster_id);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_LoadProgramFile(access, arm_prog)) != PL_OK) {
				sprintf_s(fnd, "PL_LoadProgramFile returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_Sync(access,0,&cluster_id)) != PL_OK) {
				sprintf_s(fnd, "PL_Sync returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_Sync returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}else if((cluster_id !=cluster_list[i])){
				sprintf_s(fnd, "Access refers to ARM object of cluster=%d",
						 cluster_id);
				sprintf_s(exp, "Access refers to ARM object of cluster=%d",
							cluster_list[i]);
				err_msg(fnd, exp);
				test_res = Fail;
			} 
			PL_CloseAccess(access);
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
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


