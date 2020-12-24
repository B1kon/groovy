/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="If PL_SyncArray returns PL_OK, then\n"\
			"synchronization and change values have done.\n"\
			"For NM (value, outAddress, outLen PC -> NM;\n"\
			"       returnValue, inAddress, inLen NM -> PC)\n"\
			"For ARM (value, outAddress, outLen PC -> ARM;\n"\
			"       returnValue, inAddress, inLen ARM -> PC).";
    const char *TC="014_03";
    char fnd [256];
    char exp [256];
    int res, i;
    int test_res = Pass;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int j = -1;

    PL_Access *access_nm = (PL_Access*)0;
    PL_Board *board = (PL_Board *)0;

    int Value[] = {30, 31, 32};
    PL_Word Length[] = {40, 41, 42};
    PL_Word Addr[] = {50, 51, 52};
    int Value_nm[3];
    PL_Word Length_nm[3];
    PL_Word Addr_nm[3];
    int exp_Value_nm[] = {10, 11, 12};
    PL_Word exp_Length_nm[] = {13, 14, 15};
    PL_Word exp_Addr_nm[] = {16, 17, 18};
    int err_nm = 111;
    const char *msg1 = "synchronization has not done with NM processor";
    const char *msg2 = "Error in synchronization";
    char *msg;

    int wait_time = one_sec*3;
    struct PL_CoreNo No[] = {{2,2}, {-1,2}};
    const char* processor[] = {"NM", "ARM"};
    const char *prog[] = {"nm_prog_03.abs","arm_prog_03.elf"};

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
		for(i=0; i<2; i++){
printf("STEP %d\n",i);
			if ((res = PL_GetAccess(board, &(No[i]), &access_nm)) != PL_OK) {
				sprintf_s(fnd, "PL_GetAccess returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp, "PL_GetAccess returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			} else if ((res = PL_LoadProgramFile(access_nm, prog[i])) != PL_OK) {
				sprintf_s(fnd, "PL_LoadProgramFile returns %s %d",
						sp_error_name[res], res);
				sprintf_s(exp,"PL_LoadProgramFile returns PL_OK");
				err_msg(fnd, exp);
				test_res = Fail;
			}
			if (test_res != Pass){
				break;
			}

//====================================
			for (j=0; j<3; j++) {
				if ((res = PL_SyncArray(access_nm, \
						Value[j], Addr[j],Length[j],\
						&(Value_nm[j]), &(Addr_nm[j]), \
						&(Length_nm[j]))) != PL_OK) {
					sprintf_s(fnd, "Sync loop=%d PL_SyncArray returns %s %d",
							j, sp_error_name[res], res);
					sprintf_s(exp, "PL_SyncArray return PL_OK %d", PL_OK);
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
			}
//			if (test_res != Pass) {
			for (j=0; j<3; j++) {
				if (Value_nm[j] != exp_Value_nm[j]) {
					if (Value_nm[j] != err_nm) {msg = (char*)msg1;}
					else {msg = (char*)msg2;}
					sprintf_s(fnd, "%s loop %d received Value=%d",
							msg, j, Value_nm[j]);
					sprintf_s(exp, "received Value=%d", exp_Value_nm[j]);
					err_msg(fnd, exp);
					test_res = Fail;
					break;
				}
			}
 //   }
//=================================================

			PL_CloseAccess(access_nm);
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
    PL_CloseAccess(access_nm);
    PL_CloseBoardDesc(board);

    result_msg(TC, test_res);
    return(test_res);}
