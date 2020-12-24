/*
    Реализация тестового набора для БЗО МС12705
*/

#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Module descriptors with the same index,\n"\
			"show the same firmware version";
    const char *TC="007_04";
    char fnd [256];
    char exp [256];
    int res, i, j;
    int test_res = Pass;
    unsigned int count;

#define board_cnt	3
    PL_Board *board[board_cnt];
    unsigned int version_major[board_cnt];
    unsigned int version_minor[board_cnt];

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if (( res = PL_GetBoardCount(&count)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardCount returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetBoardCount returns PL_OK %d", PL_OK);
			err_msg(fnd, exp);
			test_res = UnTested;
		} else if(count == 0) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0");
			err_msg(fnd, exp);
			test_res = UnTested;
		} 
		if(test_res != Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for (i=0; i<board_cnt; i++){
			if ((res = PL_GetBoardDesc(0, &(board[i]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetBoardDesc step %d returns %s %d",
							i, sp_error_name[res], res);
				sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
if((res = PL_GetFirmwareVersion(board[i], &(version_major[i]), &(version_minor[i]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetFirmwareVersion step %d returns %s %d",
							i,sp_error_name[res], res);
				sprintf_s(exp, "PL_GetFirmwareVersion returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}
		}
		for (j=0; j<i; j++){
			PL_CloseBoardDesc(board[j]);
		}
		for (i=0; i<board_cnt; i++){
			if ((version_major[i] != version_major[0]) || \
				(version_minor[i] != version_minor[0])){
				sprintf_s(fnd,
					"major or minor of version[%d]=%d is not equal version[0]=%d",
					i,version_major[i],version_major[0]);
				sprintf_s(exp, "All versios are equal");
				err_msg(fnd, exp);
				test_res = Fail;
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
    result_msg(TC, test_res);
    return(test_res);
}


