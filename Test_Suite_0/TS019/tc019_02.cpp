/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If address points into protected memory\n"\
			"then PL_ReadRegister returns PL_BADADDRESS\n"\
			"Protected arrays:\n"\
			"For ARM cluster_id=[-1,...,3]\n"\
			"       protected [0x0,..., 0x7FFFC] [0xFFFFС-].";
    const char *TC="019_02";
    char fnd [256];
    char exp [256];
    int res, i, j;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Access *access = (PL_Access*)0;
    PL_Board *board = (PL_Board *)0;
    #define list  2
    const char* processor_list[] = {"PC", "CCPU"};
    struct PL_CoreNo No_list[] = {{-1,2},{-1,-1}};
    PL_Word protect_array[][2][2] = {
			//PC_array
			{{0x0, 0x7FFFC},{0100000,0x1FFFFFFF} },
			//CCPU_array
			{{0x0, 0x7FFFC},{0100000,0x1FFFFFFF}} };
	
    PL_Word Load[][2][2] = {
			//ARM PC
		{ {protect_array[1][1][1],1},{protect_array[1][1][0],1} },
			// APM CCPU
		{ {protect_array[2][1][1],1},{protect_array[2][1][0],1} }
			};
    int Load_loop[] = {2,2};
    PL_Word value;
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
			printf("Core %s \n", processor_list[i]);
			if ((res = PL_GetAccess(board, &(No_list[i]), &access)) != PL_OK) {
				sprintf_s(fnd, "Processor %s PL_GetAccess returns %s %d",
						processor_list[i],sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
			for (j=0; j<Load_loop[i]; j++) { 
				if ((res=PL_ReadRegister(access,&value,Load[i][j][0])) != \
						PL_BADADDRESS) {
					sprintf_s(fnd,"Processor %s addr %x  but\n"\
						" PL_ReadRegister returns %s %d",
						processor_list[i],Load[i][j][0],
						sp_error_name[res], res);
					sprintf_s(exp, "PL_ReadRegister returns PL_BADADDRESS %d",
								PL_BADADDRESS);
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
    PL_CloseAccess(access);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


