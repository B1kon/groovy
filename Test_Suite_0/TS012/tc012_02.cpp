/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If address points into not existed memory or\n"\
			"into protected arrays\n"\
			"then PL_ReadMemBlock returns PL_BADADDRESS\n"\
			"Absent array:\n"\
			"	NM >0х3FFFFFFF\n"\
			"	ARM PC2	[0x40000,...,0x7FFFF]\n"
			"Protected array:\n"\
			"	ARM PC2&CCPU [0x80000,...,0xFFFFF]\n";
    const char *TC="012_02";
    char fnd [256];
    char exp [256];
    int res, i, j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Access *access = (PL_Access*)0;
    PL_Board *board = (PL_Board *)0;
    int array[][2] = {
			//NM_array Absent
			{0x3FFFFFFF, 0x40000010},
			//PC_array Absent
			{0x40000, 0x7FFFF},
			//PC&CCPU_array Protected
			{0x80000, 0xFFFFF}};
    #define list  4
    #define rules  2
    const char* processor_list[] = {"NM", "PC", "PC", "CCPU"};
    struct PL_CoreNo No_list[] = {{2,2}, {-1,2}, {-1,2}, {-1,-1}};

 PL_Word Load[][2][2] = {
			//NM Absent
		{ {array[0][0]-1,3},{array[0][1],2} },
			//ARM PC Absent
		{ {array[1][0]-1,2},{array[1][1],2} },
			//ARM Protected
		{ {array[2][0]-1,2},{array[2][1],2} },
			//CCPU Protected
		{ {array[2][0]-1,2},{array[2][1],2} }
		};	
    PL_Word *Buffer = (PL_Word *)0;

    tc_id(TC, ASSERTION);
    __try {// Handler Segmentation Fault
		if ((Buffer = (PL_Word*)malloc(sizeof(PL_Word)*5)) ==(PL_Word *)0) {
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
			if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			for (j=0; j<rules; j++) { 
				printf("Processor %s addr %x size %x\n",
						processor_list[i],Load[i][j][0],Load[i][j][1]);
				if ((res = PL_ReadMemBlock(access, Buffer,\
						Load[i][j][0],Load[i][j][1])) != PL_BADADDRESS) {
					sprintf_s(fnd,"Processor %s addr %x size %x but\n"\
						" PL_ReadMemBlock returns %s %d",
						processor_list[i],Load[i][j][0],Load[i][j][1],
						sp_error_name[res], res);
					sprintf_s(exp, "PL_ReadMemBlock returns PL_BADADDRESS");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
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
	
    free((char *)Buffer);
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


