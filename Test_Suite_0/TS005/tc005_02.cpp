/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="The result PL_ResetBoard - PL_OK,\n"\
			"reset of the module is executed.\n"
			"All cores are ready to work";
    const char *TC="005_02";
    char fnd [256];
    char exp [256];
    int res;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int i, j, a, b;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access_PC[5];
    PL_Access *access_NM_PC[4][4];
    PL_Word status = 9999;
    struct PL_CoreNo PC;
    struct PL_CoreNo NM_PC;

    const char *nm_prog = "nm_prog.abs";
    const char *arm_prog ="arm_prog.elf";


    tc_id(TC, ASSERTION);
	__try {// Handler Segmentation Fault

		PL_SetTimeout(one_sec);
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
		if(test_res != Pass){
			result_msg(TC, test_res);
			return(test_res);
		} 
		// run all cores 
		// CCPU, PC0,...,PC3
		for(i=-1; i<4; i++) {
			PC = {-1,i};
			printf("Step %d Check ARM\n",i);
			if ((res = PL_GetAccess(board, &PC, &(access_PC[i+1]))) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}else if((res=PL_GetStatus(access_PC[i+1], &status)) != PL_OK) {
				sprintf_s(fnd,"PL_GetStatus returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp,"PL_GetStatus returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}else {
				printf("status=%d PROGRAM_FINISHED=%d\n",status, PROGRAM_FINISHED);

				if((res=PL_LoadProgramFile(access_PC[i+1],arm_prog)) != PL_OK) {
					sprintf_s(fnd, "Check ARM PL_LoadProgramFile returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if ((res = PL_Sync(access_PC[i+1],0,NULL)) != PL_OK) {
					sprintf_s(fnd, "Check ARM PL_Sync returns %s %d",
						sp_error_name[res], res);
					sprintf_s(exp, "Befor ResetBoard PL_Sync returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
			}
		}
		if(test_res != Pass){
			PL_ResetBoard(board);
		for(a=0; a<i+1; a++) { PL_CloseAccess(access_PC[a]); }
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		} 
		// all nm for all arm
		for(i=0; i<4; i++) { // arm loop
			for(j=0; j<4; j++) { // nm loop
				NM_PC = {j,i};
				printf("Step NM_PC = {%d,%d} Check NM\n",j,i);
				if((res=PL_GetAccess(board,&NM_PC,&(access_NM_PC[j][i]))) != PL_OK) {
					sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
					sprintf_s(exp, "PL_GetAccess returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if((res=PL_LoadProgramFile(access_NM_PC[j][i],nm_prog)) != \
								 PL_OK) {
					sprintf_s(fnd, "Check NM PL_LoadProgramFile returns %s %d",
								sp_error_name[res], res);
					sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if ((res = PL_Sync(access_NM_PC[j][i],0,NULL)) != PL_OK) {
					sprintf_s(fnd, "Check NM PL_Sync returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp, "Befor ResetBoard PL_Sync returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} 
			}
			if(test_res != Pass) break;
		}
		if(test_res != Pass){
			PL_ResetBoard(board);
			for(a=0; a<i; a++) {
				for(b=0; b<j; b++) {
					PL_CloseAccess(access_NM_PC[b][a]);
				}
			}
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		} 

		if ((res = PL_ResetBoard(board)) != PL_OK) {
			sprintf_s(fnd, "1 PL_ResetBoard returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_ResetBoard returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
			for(a=0; a<5; a++) { PL_CloseAccess(access_PC[a]); }
			for(a=0; a<4; a++) {
				for(b=0; b<4; b++) {
					PL_CloseAccess(access_NM_PC[b][a]);
				}
			}
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		} 
		// run all cores 
		// CCPU, PC0,...,PC3
		for(i=-1; i<4; i++) {
			PC = {-1,i};
			printf("Step %d ARM Rst1\n",i);
			if ((res = PL_LoadProgramFile(access_PC[i+1],arm_prog)) != PL_OK) {
				sprintf_s(fnd, "ARM Rst1 PL_LoadProgramFile returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, 
					"After ResetBoard PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			} else if ((res = PL_ResetBoard(board)) != PL_OK) {
				sprintf_s(fnd, "2 PL_ResetBoard returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, "PL_ResetBoard returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			} else if ((res = PL_Sync(access_PC[i+1],0,NULL)) != PL_TIMEOUT) {
				sprintf_s(fnd, "ARM Rst2 PL_Sync returns %s %d",
							sp_error_name[res], res);
				sprintf_s(exp, "After ResetBoard PL_Sync returns PL_TIMEOUT");
				err_msg(fnd, exp);
				test_res = Fail;
				break;
			}
		}

		for(a=0; a<5; a++) { PL_CloseAccess(access_PC[a]); }

		if(test_res != Pass){
			for(a=0; a<4; a++) {
				for(b=0; b<4; b++) {
					PL_CloseAccess(access_NM_PC[b][a]);
				}
			}
			PL_CloseBoardDesc(board);
			result_msg(TC, test_res);
			return(test_res);
		} 
		// all nm for all arm
		for(i=0; i<4; i++) { // arm loop
			for(j=0; j<4; j++) { // nm loop
				NM_PC = {j,i};
				printf("Step NM_PC ={%d,%d} NM Rst1\n",j,i);
				if((res=PL_LoadProgramFile(access_NM_PC[j][i], nm_prog)) != PL_OK){
					sprintf_s(fnd, "NM Rst2 PL_LoadProgramFile returns %s %d",
							sp_error_name[res], res);
					sprintf_s(exp, 
						"After ResetBoard PL_LoadProgramFile returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if ((res = PL_ResetBoard(board)) != PL_OK) {
					sprintf_s(fnd, "3 PL_ResetBoard returns %s %d",
								sp_error_name[res], res);
					sprintf_s(exp, "PL_ResetBoard returns PL_OK");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				} else if((res=PL_Sync(access_NM_PC[j][i],0,NULL)) != PL_TIMEOUT) {
					sprintf_s(fnd, "NM Rst3 PL_Sync returns %s %d",
								sp_error_name[res], res);
					sprintf_s(exp, "After ResetBoard PL_Sync returns PL_TIMEOUT");
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
			}
			if(test_res != Pass) break;
		}

		for(a=0; a<4; a++) {
			for(b=0; b<4; b++) {
				PL_CloseAccess(access_NM_PC[b][a]);
			}
		}
	} // End __try
	__except((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? \
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
	{
		printf("ERROR: Found SIGSEGV\n");
		sprintf_s(fnd, 256, "ERROR: Found SIGSEGV");
		sprintf_s (exp, 256, "the test without any exceptions");
		err_msg(fnd, exp);
		test_res = UnTested;
	}
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


