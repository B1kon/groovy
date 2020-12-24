/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main() {

    const char *ASSERTION="The result PL_GetFirmwareVersion - PL_ ERROR if\n"\
	"the argument of board isn't the index on a module copy descriptor";
    const char *TC="007_01";
    char fnd [256];
    char exp [256];
    unsigned int res;
	
    char brd [256];
    PL_Board *board = (PL_Board *)brd;
    int test_res=Pass;
    unsigned int version_major;
    unsigned int version_minor;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if ((res = PL_GetFirmwareVersion(board, &version_major,\
								&version_minor)) == PL_OK) {
			sprintf_s(fnd, "PL_GetFirmwareVersion returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_GetFirmwareVersion returns PL_ ERROR");
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
    result_msg(TC, test_res);
    return(test_res);
}


