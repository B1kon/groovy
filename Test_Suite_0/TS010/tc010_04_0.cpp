/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If some segment of code should be loaded into one\n"\
		"of protected memory areas (REG_protect or CCPU_protec), then\n"\
		"PL_LoadProgramFile returns PL_BADADDRESS";
    const char *TC="010_04_0";
    char fnd [256];
    char exp [256];
    int res;
	#define test_cnt	100000
    unsigned int count = test_cnt;

    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access_arm = (PL_Access*)0;
    PL_Access *access_nm = (PL_Access*)0;
    const char *prog_nm_REG_protect = "nm_prog_0x0_REG_protect.abs";
    const char *prog_arm_REG_protect = "arm_prog_0x0_REG_protect.elf";
    const char *prog_arm_CCPU_0x3_protect = "arm_prog_0x0_CCPU_0x3_protect.elf";
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
		// Case nm_REG_protect
		No = {2,2};
printf("Case nm_REG_protect No ={2,2}\n");
		if ((res = PL_GetAccess(board, &No, &access_nm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access_nm, prog_nm_REG_protect)) \
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case nm_REG_protect. PL_LoadProgramFile returns %s %d",
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

		// Case arm_REG_protect Claster
		No = {-1,2};
printf("Case arm_REG_protect Claster No ={-1,2}\n");
		if ((res = PL_GetAccess(board, &No, &access_arm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access_arm, prog_arm_REG_protect)) \
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case arm_REG_protect. PL_LoadProgramFile returns %s %d",
						sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_BADADDRESS");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res != Pass){
			PL_CloseAccess(access_arm);
			PL_ResetBoard(board);
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		}else{
			PL_CloseAccess(access_arm);
		}
    // Case arm_REG_protect CCPU
		No = {-1,-1};
printf("Case arm_REG_protect CCPU No ={-1,-1}\n");
		if ((res = PL_GetAccess(board, &No, &access_arm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access_arm, prog_arm_REG_protect)) \
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case arm_REG_protect. PL_LoadProgramFile returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_BADADDRESS");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res != Pass){
			PL_CloseAccess(access_arm);
			PL_ResetBoard(board);
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		}else{
			PL_CloseAccess(access_arm);
		}
    // Case arm_protect
		No = {-1,-1};
printf("Case arm_protect No ={-1,-1}\n");
		if ((res = PL_GetAccess(board, &No, &access_arm)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
				sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if((res = PL_LoadProgramFile(access_arm,prog_arm_CCPU_0x3_protect))\
						!= PL_BADADDRESS) {
			sprintf_s(fnd, "Case arm_protect. PL_LoadProgramFile returns %s %d",
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
//    PL_ResetBoard(board);
    PL_CloseAccess(access_arm);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


