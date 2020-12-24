/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If PL_WriteRegister returns PL_OK, then the value\n"\
			"\"value\" is written to the register " \
			"at the address \"address\"";
    const char *TC="018_03";
    char fnd [256];
    char exp [256];
    int res;
    PL_Word i,j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access *)0;
    #define Length_fst	5
    #define Length	3
    #define list	2
    #define rules	2
    const char *prog[] = {"arm_prog_04.elf", "arm_prog_04.elf"};
    const char* processor_list[] = {"CCPU", "ARM"};
    struct PL_CoreNo No_list[] = {{-1,-1}, {-1,2}};
    PL_Word Load[] = {
	// GPIO DDR0 CCPU
			0xCC010,
	//GPIO DDR0 ARM
			0xCC010
			};

    PL_Word value = 0xAAAAAAAA;
 //   PL_Word CK_value = 0x0000000A;
    int Check_Value;
    PL_Word Check_Addr, Check_Length;
    int wait_time = one_sec*1;
    int exp_value = 2222;
	
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
		for(i=0; i<list; i++){
printf("Processor %s\n",processor_list[i]);
			Check_Value=0; Check_Addr=0; Check_Length=0;
			if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
					processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			if ((res = PL_LoadProgramFile(access, prog[i])) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_LoadProgramFile returns %s %d",
					processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			if ((res = PL_WriteRegister(access, value, Load[i])) != PL_OK) {
				sprintf_s(fnd,"Processor %s addr %x  but\n"\
					" PL_WriteRegister returns %s %d",
					processor_list[i],Load[i],
				sp_error_name[res], res);
				sprintf_s(exp, "PL_WriteRegister returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			if (test_res !=Pass)  break;
			if ((res=PL_SyncArray(access, 1, Load[i], value, \
					&Check_Value, &Check_Addr, &Check_Length)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_SyncArray returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp,"PL_SyncArray return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				test_res = Fail;
			}else if ((res = PL_Sync(access, 2, &Check_Value)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_Sync returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_Sync return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				test_res = Fail;
			}else if ((exp_value != Check_Value)) {
				sprintf_s(fnd, "%s\n %s-prog returns 0x%x",
						msg, processor_list[i], Check_Value);
				sprintf_s(exp, "%s-prog returns %d",processor_list[i],exp_value);
				err_msg(fnd, exp);
				test_res = Fail;
			}
			PL_Sync(access, 2, &Check_Value);
			printf("Check_Value=0x%x value=0x%x\n",Check_Value,value);
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
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}
