/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>


int main(int argc, char* argv[]) {

    const char *ASSERTION="If coreNo.nm_id value is in segment [0, 3],\n"\
		"and coreNo.cluster_id value is in segment [0, 3], then\n"\
		"result of PL_GetAccess = PL_OK and the argument 'access'\n"\
		"refers to NeoroMatrix object according to coreNo";
    const char *TC="008_03_0";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    const char *nm_prog = "nm_prog_03.abs";

    struct PL_CoreNo coreNo={0,3};
    PL_Word cluster_id;
	int nm_id;

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
		} else if ((res = PL_GetAccess(board, &coreNo, &access)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d, coreNo={%d,%d}",
						sp_error_name[res], res, coreNo.nm_id, coreNo.cluster_id);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access, nm_prog)) != PL_OK) {
			sprintf_s(fnd, "PL_LoadProgramFile returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_SyncArray(access,0,NULL,0,\
					&nm_id,NULL,&cluster_id)) != PL_OK) {
			sprintf_s(fnd, "PL_SyncArray returns %s %d",
							sp_error_name[res], res);
			sprintf_s(exp, "PL_SyncArray returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if((nm_id != coreNo.nm_id) || (cluster_id != coreNo.cluster_id)){
			sprintf_s(fnd, "Access refers to NeoroMatrix object with coreNo={%d,%d}",
						 nm_id, cluster_id);
			sprintf_s(exp, "Access refers to NeoroMatrix object with coreNo={%d,%d}",
						coreNo.nm_id, coreNo.cluster_id);
			err_msg(fnd, exp);
			test_res = Fail;
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


