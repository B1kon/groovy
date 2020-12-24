/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="PL_ReadMemBlock can read data from all\n"\
			"memory areas available in board\n"\
	"ARM CCPU (0x0-0x7FFFF),(0x100000-)\n"\
	"ARM CPU (0x0-0x3FFFF),(0x100000-0x2FFFFF),(0x300000-)\n"\
	"NM (0x0-0x1FFFF),(0x40000-0xBFFFF),(0xC0000-0xCFFFF),(0xD0000-)";
    const char *TC="012_05";
    char fnd [256];
    char exp [256];
    int res, i,j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Access *access = (PL_Access*)0;
    PL_Board *board = (PL_Board *)0;
    #define list  3
    #define rules  4
    const char* processor_list[] = {"NM", "PC", "CCPU"};
    struct PL_CoreNo No_list[] = {{2,2}, {-1,2},{-1,-1}};
    int avlb[] = {5, 4, 3};  // test arrays
    PL_Word bgn[][5] = {{0x0, 0x40000, 0xC0000, 0xD0000, 0x07FFF000}, // NM
		{0x0, 0x100000, 0x300000, 0x1FFFF000, 0x0},	// ARM CPU
		{0x0, 0x100000, 0x1FFFF000, 0x0, 0x0}};  // ARM CCPU
    PL_Word size[][5] ={
		{(0x1FFFF-bgn[0][0]+1), (0xBFFFF-bgn[0][1]+1), 
		 0xCFFFF-bgn[0][2]+1, 0xDFFFF-bgn[0][3]+1,
		 0x07FFFFFF-bgn[0][4]+1},			// NM
		{0x3FFFF-bgn[1][0]+1, 0x2FFFFF-bgn[1][1]+1, 
		 0x30FFFF-bgn[1][2]+1, 0x1FFFFFFF-bgn[1][3]+1, 0x0},// ARM CPU
		{0x7FFFF-bgn[2][0]+1, 0x10FFFF-bgn[2][1]+1, 
		 0x1FFFFFFF-bgn[2][2]+1, 0x0, 0x0}		// ARM CCPU
		};
    PL_Word buf_size = 0x10000;
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
		}
		if (test_res !=Pass){
			result_msg(TC, test_res);
			return(test_res);
		}
		for(i=0; i<list; i++){
			if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			for (j=0; j<avlb[i]; j++) { 
				printf("Processor %s. Check memory array 0x%x size 0x%x\n",
					processor_list[i],bgn[i][j],size[i][j]);
				PL_Word area_sz = size[i][j];
				PL_Word area_size;// in word
				PL_Word offset; // in word

				if (strcmp(processor_list[i],"NM") == 0){
					area_size = area_sz;// in word
				}else{
					area_size = (area_sz>>2);// in byte
				}
				for (offset=0; offset<area_size; offset += buf_size) {
					PL_Word bl_sz = buf_size; // in word
					PL_Word adr;  //in ??
					if (strcmp(processor_list[i],"NM") == 0){
						adr = bgn[i][j] + offset;// in word
					}else{
						adr = bgn[i][j] + (offset>>2); // in byte
					}

					if ((offset + buf_size) > area_size) {
						bl_sz = area_size - offset;
					}
printf("%s step %d address=%x size=%x\n",processor_list[i],j, adr, bl_sz);
					if ((res=PL_ReadMemBlock(access,Block,adr,bl_sz)) != PL_OK) {
						sprintf_s(fnd, "Processor %s area address=%x size=%x "\
							"PL_ReadMemBlock returns %s %d",
							processor_list[i],adr,bl_sz,sp_error_name[res], res);
						sprintf_s(exp, "PL_ReadMemBlock returns PL_OK");
						err_msg(fnd, exp);
						test_res = Fail;
						break;
					}
				}
				if (test_res != Pass) break;
			}
			PL_CloseAccess(access);
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


