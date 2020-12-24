/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Executable ELF code can work in all memory areas\n"\
			"available for writing.\n"\
			"ARM - point in bytes. NM - point in 32words. \n"\
"ARM CCPU (0x800-0x5FFFF),(0x100000-0x1FFFFFFF)\n"\
"ARM PC (0x800-0x3FFFF),(0x100000 – 0x2FFFFF),(0x300000-0x1FFFFFFF)\n"\
"NM (0x200-0x1FFFF), (0x40000-0xBFFFF), (0xC0000-0xCFFFF), (0xD0000-0x07FFFFFF)";
    const char *TC="010_06_0";
    char fnd [256];
    char exp [256];
    int res, i,j;
	#define test_cnt	100000
    unsigned int count = test_cnt;
	int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
	PL_Access *access = (PL_Access*)0;
	
int array_list[3][4][2] = {
	//NM_array[][2]
{{0x200,0x1FFFF},{0x40000,0xBFFFF},{0xC0000,0xCFFFF},{0xD0000,0x7FFFFFF}},
	//PC_array[][2]
{{0x800,0x3FFFF},{0x100000,0x2FFFFF},{0x300000,0x1FFFFFFF},{0x0,0x0}},
	//CCPU_array[][2]
{{0x800,0x5FFFF},{0x100000,0x1FFFFFFF},{0x0,0x0},{0x0,0x0}}
			};

const char *prog_list[][4] = {
	//NM_prog[]
{"nm_prog_0.abs", "nm_prog_4.abs", "nm_prog_C.abs", "nm_prog_D.abs"},
	//PC_prog[]
{"arm_prog_PC_0.elf","arm_prog_PC_10.elf","arm_prog_PC_30.elf","no_code"},
	//CCPU_prog
{"arm_prog_CCPU_0.elf","arm_prog_CCPU_10.elf","no_code","no_code"}
			};
    #define list  3
    const char* processor_list[] = {"NM", "PC", "CCPU"};
    struct PL_CoreNo No_list[] = {{2,2}, {-1,2},{-1,-1}};
    struct PL_CoreNo No2check[] = {{2,2}, {4,2},{4,4}};
    int mem_nmb_list[] = {4,3,2};
	int wait_time = one_sec; //one_sec*2;
    PL_Word  cluster_id, core_addr = 0x0;
	int nm_id;
    const char *msg = "code does not work in the memory area.";

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
		for(i=0; i<list; i++){	// LIST LOOP
			for (j=0; j<mem_nmb_list[i]; j++) {	// ARRAY MEM LIST LOOP 
				if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
					sprintf_s(fnd, "PL_GetAccess returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp, "PL_GetAccess returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}

//	for (j=0; j<mem_nmb_list[i]; j++) {	// ARRAY MEM LIST LOOP  
				printf("Processor %s segment address (%x,%x)\n",
					processor_list[i],array_list[i][j][0],array_list[i][j][0]);
				if ((res = PL_LoadProgramFile(access, prog_list[i][j])) != PL_OK) {
					sprintf_s(fnd, "Processor %s segment address (%x,%x)\n"\
						" but PL_LoadProgramFile returns %s %d",
						processor_list[i],array_list[i][j][0],array_list[i][j][0],
						sp_error_name[res], res);
					sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if ((res = PL_SyncArray(access,0,NULL,0,\
							&nm_id,&core_addr,&cluster_id)) != PL_OK) {
					sprintf_s(fnd, "Processor %s segment address (%x,%x)\n "\
									"PL_SyncArray returns %s %d",
							processor_list[i],array_list[i][j][0],array_list[i][j][0],
							sp_error_name[res],res);
					sprintf_s(exp, "PL_SyncArray returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
				} else {
					if((nm_id != (No2check[i]).nm_id) || \
						(cluster_id != No2check[i].cluster_id)){
						sprintf_s(fnd,
						"The %s-programm does work in the coreNo={%d,%d}",
						processor_list[i], nm_id, cluster_id);
						sprintf_s(exp,
						"The %s-programm does work in the coreNo={%d,%d}",
						processor_list[i],No2check[i].nm_id,
						No2check[i].cluster_id);
						err_msg(fnd, exp);
						test_res = Fail;
					}else if((core_addr < ((PL_Word) array_list[i][j][0])) || \
						(core_addr > ((PL_Word)array_list[i][j][1]))) {
						sprintf_s(fnd,
						"The %s-programm region is out of memory card={%d,%d} addr=0x%x",
						processor_list[i], No_list[i].nm_id, 
						No_list[i].cluster_id, core_addr);
						sprintf_s(exp,
						"The %s-programm is in region of memory card={%d,%d}",
						processor_list[i], No_list[i].nm_id,
						No_list[i].cluster_id);
						err_msg(fnd, exp);
						test_res = Fail;
					}
				}
				PL_CloseAccess(access);
				if (test_res !=Pass)  break;
				PL_ResetBoard(board);
			} // END ARRAY MEM LIST LOOP 
			if (test_res !=Pass)  break;

		} // END LIST LOOP
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


