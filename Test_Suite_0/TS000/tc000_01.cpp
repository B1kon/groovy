/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>


int main(int argc, char* argv[]) {

    const char *ASSERTION="RESET";
    const char *TC="000_01";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    const char *nm_prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    const char* processor[] = {"NM", "ARM"};
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};

  

    tc_id(TC, ASSERTION);


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
    
    if ((res = PL_ResetBoard(board)) != PL_OK) {
		test_res = Fail;
    }

    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


