/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_WriteMemBlock can load data in all memory\n"\
		"areas available for writing. \n"\
		"Addressing with base offset in memory of another ARM.\n"\
		"    Access (-1,1)\n"\
	"    ARM CCPU (0x60000000-0x6005FFFF),(0x60100000-0x7FFFFFFF)\n"\
	"    ARM PC (0xС0000000-0xС003FFFF),(0xС0100000 – 0xС02FFFFF),(0xС0300000-0xDFFFFFFF)\n"\
	"NM There are no  base addresses";
    const char *TC="011_07_1";
    char fnd [256];
    char exp [256];
    int res, i,j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
	PL_Access *access = (PL_Access*)0;
	PL_Board *board = (PL_Board *)0;
    #define list  2
    const char* processors[] = {"CCPU","PC"};
    struct PL_CoreNo core = {-1,1};	
    PL_Word AVLB[ ][5][2]  =	{
		// CCPU_array 
	{0x60000000,0x60100000,0x7FFFE000,0,0 },
		// PC_array core=2 (address in byte size in word)
	{0xC6000000,0xC0100000,0xC02F0000,0xC0300000,0xDFFF0000 }
				};
    int mem_nmb_list[] = {3,5};
    PL_Word buf_size = 0x200;
    PL_Word offset = 0x1000;
    PL_Word * Block = (PL_Word *)0;

    tc_id(TC, ASSERTION);

    __try {// Handler Segmentation Fault
		if ((Block = (PL_Word*)malloc(sizeof(PL_Word)*buf_size)) == (PL_Word *)0) {
			err_msg((char*)"Cannot create Block", (char*)"Block is created");
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
		}else if ((res = PL_GetAccess(board, &core, &access)) != PL_OK) {
			sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
					processors[i],sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for(i=0; i<list; i++){ //Processors loop
	
			printf("Check memory areas %s\n",processors[i]);
			for (j=0; j<mem_nmb_list[i]; j++) { //Memory loop
				PL_Word bl_sz = buf_size;// in word
				PL_Word adr = AVLB[i][j][0]+offset;

				printf("%s step %d address=%x size=%x\n",
						processors[i],j, adr, bl_sz);
				if ((res =PL_WriteMemBlock(access,Block,adr,bl_sz)) != PL_OK){
					sprintf_s(fnd, "%s address=%x size=%x "\
						"PL_WriteMemBlock returns %s %d",
						processors[i], adr, bl_sz,sp_error_name[res], res);
					sprintf_s(exp, "PL_WriteMemBlock returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
				if (test_res != Pass) break;
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
    free((char *)Block);
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


