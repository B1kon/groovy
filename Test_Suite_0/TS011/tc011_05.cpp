/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If argument len = 0, "\
			"then any dates are not loaded into board and\n"\
			"PL_WriteMemBlock returns PL_OK.";
    const char *TC="011_05";
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
    #define Length_snd	0
    #define list	2
    #define rules	2
    const char *prog[] = {"nm_prog_05.abs", "arm_prog_05.elf"};
    const char* processor_list[] = {"NM", "ARM"};
    struct PL_CoreNo No_list[] = {{2,2}, {-1,2}};
    PL_Word Load[][2][2] = {
	// NMMB2_array   for Buffer_fst for Buffer
			{{0x80000,Length_fst},  {0x80001,Length_snd}},
	//CMB2_array
			{{0x20000,Length_fst},  {0x20004,Length_snd}}
			};
    PL_Word *Buffer_snd = (PL_Word *)0;
    PL_Word * Buffer_fst = (PL_Word *)0;
    PL_Word *Buf[2];
    PL_Word to_fill = 0xafafafaf;
    PL_Word Check_Addr, Check_Length;
	int Check_Value;
    int exp_value = 2222;
    const char *msg = "data are loaded but argument len = 0.";

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault
		// create buffers
		Buffer_snd = (PL_Word*)malloc(sizeof(PL_Word)*Length_fst);
		Buffer_fst = (PL_Word*)malloc(sizeof(PL_Word)*Length_fst);
		if ((Buffer_snd == (PL_Word *)0) || (Buffer_fst == (PL_Word *)0)){
			err_msg((char*)"Cannot create buffer", (char*)"buffer is created");
			test_res = UnTested;
			result_msg(TC, test_res);
			return(test_res);
		}
		for (i=0; i <Length_fst; i++) {
			Buffer_fst[i] = to_fill;
		}
		bzero((char *)(Buffer_snd), sizeof(PL_Word)*Length_fst);
		Buf[0] = Buffer_fst;
		Buf[1] = Buffer_snd;

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
			for (j=0; j<rules; j++) { 
				printf("Write addr %x size %x\n",Load[i][j][0],Load[i][j][1]);
				printf("Buffer[0]=%x\n", Buf[j][0]);
				if ((res = PL_WriteMemBlock(access, Buf[j], \
						Load[i][j][0],Load[i][j][1])) != PL_OK) {
					sprintf_s(fnd,"Processor %s addr %x size %x but\n"\
						" PL_WriteMemBlock returns %s %d",
						processor_list[i],Load[i][j][0],Load[i][j][1],
						sp_error_name[res], res);
					sprintf_s(exp, "PL_WriteMemBlock returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
			}
			if (test_res !=Pass)  break;

			if ((res=PL_SyncArray(access, 1, Load[i][1][0], to_fill, \
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
				sprintf_s(fnd, "%s\n %s-prog returns %d",
						msg, processor_list[i], Check_Value);
				sprintf_s(exp, "%s-prog returns %d",processor_list[i],exp_value);
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
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    free(Buffer_fst);
    free(Buffer_snd);
    result_msg(TC, test_res);
    return(test_res);
}
