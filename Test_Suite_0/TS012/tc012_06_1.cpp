/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_ReadMemBlock can read data from all\n"\
		"memory areas available in board.\n"\
		"Addressing with base offset in memory of another ARM.\n"\
	"    Access (-1,1)\n"\
	"    ARM CCPU (-1,-1) (0x60000000-0x6007FFFF),(0x60100000-)\n"\
	"    ARM CPU2 (-1,2)(0xС0000000-0xС003FFFF),(0xС0100000-0xС02FFFFF),(0xС0300000-)\n"\
	"     NM There are no base addresses";
    const char *TC="012_06_1";
    char fnd [256];
    char exp [256];
    int res, i,j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Access *access = (PL_Access*)0;
    PL_Board *board = (PL_Board *)0;
    #define list  2
    #define rules  4
    const char* processor_list[] = {"PC", "CCPU"};
    struct PL_CoreNo core = {-1,1};
    int avlb[] = {4, 3};  // test arrays
    PL_Word bgn[][4] = {
		{0xC0000000, 0xC0100000, 0xC0300000, 0xDFFFE000},	// ARM CPU
		{0x60000000, 0x60100000, 0x7FFFE000, 0x0}};  // ARM CCPU
 
    PL_Word buf_size = 0x200;
    PL_Word offset = 0x1000;
    PL_Word * Block = (PL_Word *)0;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault

		if ((Block = (PL_Word*)malloc(sizeof(PL_Word)*buf_size)) == NULL) {
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
					processor_list[i],sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for(i=0; i<list; i++){ // memory of another ARM

			for (j=0; j<avlb[i]; j++) { 
				printf("Processor %s. Check memory array 0x%x size 0x%x\n",
						processor_list[i],bgn[i][j],buf_size);
				PL_Word bl_sz = buf_size;
				PL_Word adr = bgn[i][j] + offset; // in word

				printf("%s step %d address=%x size=%x\n",
					processor_list[i],j, adr, bl_sz);
				if ((res=PL_ReadMemBlock(access,Block,adr,bl_sz)) != PL_OK) {
					sprintf_s(fnd, "Processor %s area address=%x size=%x "\
						"PL_ReadMemBlock returns %s %d",
						processor_list[i],adr,bl_sz,sp_error_name[res], res);
					sprintf_s(exp, "PL_ReadMemBlock returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
				if (test_res != Pass) break;
			}
			if (test_res != Pass) break;
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


