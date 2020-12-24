/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

const char *prog = "nm_prog_03.abs";

int main(int argc, char* argv[]) {

    const char *ASSERTION="If PL_ReadRegister returns PL_OK, then\n" \
			"the register value at address \"address\"\n"\
			"is written by pointer \"returnValue\"";
    const char *TC="019_03";
    char fnd [256];
    char exp [256];
    int res;
    PL_Word i,j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access *)0;
    #define list	2
    const char *prog = "arm_prog_03.elf";
    const char* processor_list[] = {"CCPU", "ARM"};
    struct PL_CoreNo No_list[] = {{-1,-1}, {-1,2}};

    PL_Word Length;
    PL_Word AddrStore;
    PL_Word exp_value = 0x93939393;
    int  value;
	PL_Word  value_reg;
    int wait_time = one_sec*1;

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
		for(i=0; i<list; i++){
//	Check_Value=0; Check_Addr=0; Check_Length=0;
			if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			if ((res = PL_LoadProgramFile(access, prog)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_LoadProgramFile returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			if ((res = PL_SyncArray(access, 0,0,0, \
					&value, &AddrStore, &Length)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_SyncArray returns %s %d",
					processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_SyncArray return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			value_reg = 0;
			if((res=PL_ReadRegister(access, &value_reg, AddrStore)) != PL_OK){
				sprintf_s(fnd, "Processor %s PL_ReadMemBlock returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_ReadMemBlock return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				test_res = Fail;
			}else if((value_reg && 0x0000000F) != (exp_value && 0x0000000F)) {
				sprintf_s(fnd, "Processor %s received value=%x",
						processor_list[i],value_reg);
				sprintf_s(exp, "value=%x", exp_value);
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			PL_CloseAccess(access);
			if (test_res !=Pass)  break;
			printf("value=0x%x\n",value);
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
