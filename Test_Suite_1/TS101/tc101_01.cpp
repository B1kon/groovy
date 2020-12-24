/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>

int main(int argc, char* argv[]) {

    const char *ASSERTION="Access to different clusters through a single Access object.\n"\
	"Complex test:\n"\
	"    arm-Access (-1,2)\n"\
	"    Load Programm arm-prog in (-1,3)\n"\
	"    Write block into (-1,1)\n"\
	"    arm-prog (-1,3) check block in (-1,1)\n"\
	"    Checking result received by PL_Sync (-1,3), but cluster_id=2";
    const char *TC="101_01";
    char fnd [256];
    char exp [256];
    int res, i;
    #define test_cnt	100000
    unsigned int count = test_cnt;
    int test_res = Pass;
    PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
    PL_Access *access_sync = (PL_Access*)0;
    const char *prog = "arm_prog.elf";
    #define Length	4
	int nm_id;
    PL_Word cluster_id, core_1_addr = 0xA0020000;
    PL_Word *Buffer = (PL_Word *)0;
    PL_Word to_fill = 0xafafafaf;
    PL_Word exp_value = 2222;
	int check_res = 0;

    struct PL_CoreNo core_home = {-1,2};
    struct PL_CoreNo core_sync = {-1,3};
    struct PL_CoreNo  armprog_core = {4,2};
    int wait_time = one_sec*3;

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
		Buffer = (PL_Word*)malloc(sizeof(PL_Word)*Length);
		for (i=0; i <Length; i++) {
			Buffer[i] = to_fill;
		}

		if ((res = PL_GetAccess(board, &core_home, &access)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_LoadProgramFile(access, prog))!= PL_OK) {
			sprintf_s(fnd, " PL_LoadProgramFile returns %s %d",
				sp_error_name[res], res);
			sprintf_s(exp, "PL_LoadProgramFile returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		} else if ((res = PL_WriteMemBlock(access, Buffer, \
					core_1_addr,Length)) != PL_OK) {
			sprintf_s(fnd," addr %x size %x but PL_WriteMemBlock returns %s %d",
					core_1_addr,Length,sp_error_name[res], res);
			sprintf_s(exp, "PL_WriteMemBlock returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if ((res = PL_GetAccess(board, &core_home,&access_sync)) != PL_OK) {
			sprintf_s(fnd, "PL_GetAccess for sync returns %s %d",
					sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if ((res = PL_SyncArray(access_sync,1,core_1_addr,to_fill,\
//				&nm_id,&((PL_Addr)check_res),&cluster_id)) != PL_OK) {
				&nm_id,NULL,&cluster_id)) != PL_OK) {
			sprintf_s(fnd, "PL_SyncArray returns %s %d",sp_error_name[res], res);
			sprintf_s(exp, "PL_SyncArray returns PL_OK");
			err_msg(fnd, exp);
			test_res = Fail;
		}else if ((res = PL_Sync(access_sync, 2, &check_res)) != PL_OK) {
			sprintf_s(fnd, "PL_Sync returns %s %d",sp_error_name[res], res);
			sprintf_s(exp, "PL_Sync return PL_OK %d", PL_OK);
			err_msg(fnd, exp);
			test_res = Fail;
		}else {
			printf("nm_id=%d  cluster_id=%d  check_res=%d\n",nm_id,cluster_id,check_res);
			if((nm_id != armprog_core.nm_id)|| \
					(cluster_id != armprog_core.cluster_id)){
				sprintf_s(fnd,"arm-programm does work in the coreNo={%d,%d}",
							nm_id, cluster_id);
				sprintf_s(exp,"arm-programm does work in the coreNo={%d,%d}",
							armprog_core.nm_id, armprog_core.cluster_id);
				err_msg(fnd, exp);
				test_res = Fail;
			}else if (exp_value != check_res) {
				sprintf_s(fnd,"Block is written out of cluster=1 core_addr=0x%x",
						core_1_addr);
				sprintf_s(exp,"Block is written in region of cluster=1");
				err_msg(fnd, exp);
				test_res = Fail;
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
    PL_ResetBoard(board);
    PL_CloseBoardDesc(board);
    result_msg(TC, test_res);
    return(test_res);
}


