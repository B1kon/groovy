/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If some segment of code should be loaded into\n"\
			"absent area of memory, then"\
			"PL_LoadProgramFile returns PL_BADADDRESS";
    const char *TC="010_04_1";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access_arm = (PL_Access*)0;
    PL_Access *access_nm = (PL_Access*)0;
    const char *prog_nm_absent = "nm_prog_0x0_absent.abs";
    const char *prog_arm_absent = "arm_prog_0x0_absent.elf";

    struct PL_CoreNo No = {2,2};

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
			test_res = Fail;
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
				return(test_res);
		}
		// Case nm_absent_area
		No = {2,2};
		if ((res = PL_GetAccess(board, &No, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access_nm, prog_nm_absent)) \
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case nm_absent area. PL_LoadProgramFile returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_BADADDRESS");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res != Pass){
			PL_CloseAccess(access_nm);
			PL_ResetBoard(board);
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		}else{
			PL_CloseAccess(access_nm);
		}

		// Case arm_absent area
		No = {-1,2};
		if ((res = PL_GetAccess(board, &No, &access_arm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access_arm, prog_arm_absent)) \
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case arm_absent area. PL_LoadProgramFile returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_BADADDRESS");
			err_msg(fnd, exp);
			test_res = Fail;
			PL_ResetBoard(board);
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
    PL_CloseAccess(access_arm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


