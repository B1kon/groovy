/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result is PL_ReadRegister = PL_ERROR\n"\
			"if the access argument references NM.";
    const char *TC="019_04";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access *)0;

    const char* processor = "NM";
    struct PL_CoreNo No_list = {2,2};
    PL_Word Load = 0xCC010;
    PL_Word value = 0xAAAAAAAA;
    int wait_time = one_sec*1;
	
    const char *msg = "data are not loaded according arguments.";

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

printf("Processor %s\n",processor);

		if ((res = PL_GetAccess(board, &No_list, &access)) != PL_OK) {
			sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
				processor,sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if ((res = PL_ReadRegister(access, &value, Load)) != PL_ERROR) {
			sprintf_s(fnd,"Processor %s addr %x  but\n"\
				" PL_ReadRegister returns %s %d",
				processor,Load,	sp_error_name[res], res);
			sprintf_s(exp, "PL_ReadRegister returns PL_ERROR");
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
