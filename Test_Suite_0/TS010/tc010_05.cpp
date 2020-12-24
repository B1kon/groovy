/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If result of PL_Load_Program_File is PL_OK, then\n"\
			"executable ELF code has been loaded for execution\n"\
			"in the space defined by the program memory card and\n"\
			"the running processor corresponds to the value coreNo";
    const char *TC="010_05";
    char fnd [256];
    char exp [256];
    int res, i;
    #define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    const char *prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    const char *processor[] = {"NeoroMatrix", "ARM"};
    PL_Word  cluster_id, core_addr = 0x40000;
	int nm_id;
    int left_addr[] = {0x200, 0x800};	//ARM:ORIGIN = 0x800, LENGTH = 0x30000
    int right_addr[] = {0x8140, 0x30800};//NM :ORIGIN = 0x200, LENGTH = 0x8140
    struct PL_CoreNo No, No2check;
    int wait_time = one_sec*3;

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
    // Case nm
		No = {2,2};
		for(i=0; i<2; i++){
			if(i == 0){
				No = {2,2}; No2check = {2,2};
			} else {
				No = {-1,2}; No2check = {4,2};
			}
			if ((res = PL_GetAccess(board, &No, &access)) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_LoadProgramFile(access, prog[i]))!= PL_OK) {
				sprintf_s(fnd, "Case %d. PL_LoadProgramFile returns %s %d",
							i, sp_error_name[res], res);
				sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_SyncArray(access,0,NULL,0,\
								&nm_id,&core_addr,&cluster_id)) != PL_OK) {
				sprintf_s(fnd, "PL_SyncArray returns %s %d",sp_error_name[res], res);
				sprintf_s(exp, "PL_SyncArray returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}else if((nm_id != No2check.nm_id) || (cluster_id != No2check.cluster_id)){
				sprintf_s(fnd,
					"The %s-programm does work in the coreNo={%d,%d}",
					processor[i], nm_id, cluster_id);
				sprintf_s(exp,
					"The %s-programm does work in the coreNo={%d,%d}",
					processor[i],No2check.nm_id, No2check.cluster_id);
				err_msg(fnd, exp);
				test_res = Fail;
			}else if ((core_addr < ((PL_Word)left_addr[i])) ||\
					   (core_addr > ((PL_Word)right_addr[i]))) {
				sprintf_s(fnd,
				"The %s-programm region is out of memory card={%d,%d} core_addr=0x%x",
				processor[i], No.nm_id, No.cluster_id,core_addr);
				sprintf_s(exp,
				"The %s-programm is in region of memory card={%d,%d}",
				processor[i], No.nm_id, No.cluster_id);
				err_msg(fnd, exp);
				test_res = Fail;
			}
			PL_CloseAccess(access);
			if (test_res !=Pass)  break;
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
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


