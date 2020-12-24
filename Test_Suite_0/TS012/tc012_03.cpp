/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If PL_ReadMemBlock returns PL_OK,\n" \
			"data from board starting with \"address\" by size \"len\"\n"\
			"are loaded into PC area \"block\"";
    const char *TC="012_03";
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
    const char *prog[] = {"nm_prog_03.abs", "arm_prog_03.elf"};
    const char* processor_list[] = {"NM", "ARM"};
    struct PL_CoreNo No_list[] = {{2,2}, {-1,2}};

    PL_Word Length = sizeof(PL_Word)*10;
    PL_Word *Buffer = (PL_Word *)0;
    PL_Word AddrStore;
    PL_Word how_fill = 0x93939393;
    int value;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if ((Buffer=(PL_Word*)malloc(sizeof(PL_Word) * Length)) == (PL_Word *)0) {
			err_msg((char*)"Cannot create buffer", (char*)"buffer is created");
			test_res = UnTested;
			result_msg(TC, test_res);
			return(test_res);
		}
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
			if ((res = PL_LoadProgramFile(access, prog[i])) != PL_OK) {
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
			bzero((char *)(Buffer), sizeof(PL_Word)*Length);
			if((res=PL_ReadMemBlock(access, Buffer, AddrStore, Length)) != PL_OK){
				sprintf_s(fnd, "Processor %s PL_ReadMemBlock returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_ReadMemBlock return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				test_res = Fail;
			}else{
				for(j = 0; j < Length; j++) {
					if(Buffer[j] != how_fill) {
						sprintf_s(fnd, "Processor %s received Buffer[%d]=%x",
							processor_list[i],j, Buffer[j]);
						sprintf_s(exp, "Buffer[%d]=%x", j, how_fill);
						err_msg(fnd, exp);
						test_res = Fail;
						break;
					}
				}
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
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    free(Buffer);
    result_msg(TC, test_res);
    return(test_res);
}
