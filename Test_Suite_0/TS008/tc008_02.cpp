/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="A) Result of PL_GetAccess = PL_ERROR, if\n"\
		    "        coreNo.nm_id value is out of segment [-1, 3],\n"\
		    "        and coreNo.cluster_id value in segment [0, 3].\n"\
		    "B) Result of PL_GetAccess = PL_ERROR, if\n"\
		    "        coreNo.nm_id value is in segment [-1, 3], and\n"\
		    "        coreNo.cluster_id value out of segment [-1, 3].\n"\
		    "C) Result of PL_GetAccess = PL_ERROR, if\n"\
		    "        coreNo.nm_id value is not equal -1, but\n"\
		    "        coreNo.cluster_id value is equal -1.";
    const char *TC="008_02";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int i;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;

    struct PL_CoreNo coreNo[2];

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
		/* ASSERTION A)
			coreNo.nm_id value is in segment [-1, 3]
			coreNo.cluster_id value out of segment [-1, 3]
			coreNo={3,-3}
			coreNo={0,5}
		*/
		coreNo[0]={3,-3};
		coreNo[1]={0,5};
		for (i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &coreNo[i], &access)) == PL_OK) {
				sprintf_s(fnd, "A) PL_GetAccess returns %s %d, coreNo={%d,%d}",
				sp_error_name[res],res, coreNo[i].nm_id, coreNo[i].cluster_id);
				sprintf_s(exp, "PL_GetAccess returns PL_ ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
				PL_CloseAccess(access);
				PL_CloseBoardDesc(board);
				result_msg(TC, test_res);
				return(test_res);
			}
		}

		/* ASSERTION B)
			coreNo.nm_id value is out of segment [-1, 3]
			coreNo.cluster_id value in segment [0, 3]
			coreNo={-3,3}
			coreNo={5,0}
		*/
		coreNo[0]={-3,3};
		coreNo[1]={5,0};
		for (i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &coreNo[i], &access)) == PL_OK) {
				sprintf_s(fnd, "B) PL_GetAccess returns %s %d, coreNo={%d,%d}",
					sp_error_name[res],res, coreNo[i].nm_id, coreNo[i].cluster_id);
				sprintf_s(exp, "PL_GetAccess returns PL_ ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
				PL_CloseAccess(access);
				PL_CloseBoardDesc(board);
				result_msg(TC, test_res);
				return(test_res);
			}
		}
		/* ASSERTION C)
			coreNo.nm_id value is not equal -1
			coreNo.cluster_id value is equal -1
			coreNo={0,-1}
			coreNo={3,-1}
		*/
		coreNo[0]={0,-1};
		coreNo[1]={3,-1};
		for (i=0; i<2; i++){
			if ((res = PL_GetAccess(board, &coreNo[i], &access)) == PL_OK) {
				sprintf_s(fnd, "C) PL_GetAccess returns %s %d, coreNo={%d,%d}",
				sp_error_name[res],res, coreNo[i].nm_id, coreNo[i].cluster_id);
				sprintf_s(exp, "PL_GetAccess returns PL_ ERROR");
				err_msg(fnd, exp);
				test_res = Fail;
				PL_CloseAccess(access);
				PL_CloseBoardDesc(board);
				result_msg(TC, test_res);
				return(test_res);
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
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


