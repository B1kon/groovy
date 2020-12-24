/*
    Реализация тестового набора для БЗО МС12705
*/
#include <common_mc12705_tc.h>
 

//int main( int core_number, int work) // Функция процесса
int main( int argc, char* argv[]) // Функция процесса
{
	const char *ASSERTION="Process for tc102_01";
    const char *TC="102_01_Process";
    int res;
    int test_res = Pass;
    char fnd [256];
    char exp [256];
	#define test_cnt	100000
    unsigned int count = test_cnt;
	PL_Board *board = (PL_Board *)0;
    PL_Access *access = (PL_Access*)0;
	const char *prog[] = {"nm_prog.abs","arm_prog.elf"};
    struct PL_CoreNo core = {-1,-1};
	PL_Word load[][2]={ 
 /*ARM*/  {0x300000,0x1FFFFFFF},  // in byte
  /*NM*/  {0x41000,0x5FFFF},{0x61000,0x7FFFF},  // in word
	  {0x81000,0x9FFFF},{0xA1000,0xBFFFF} };
    PL_Word size_b[]={ 0x7C000, //in byte = 0.5mb NM
		     0x200000}; //in byte   2mb   ARM
    PL_Word size_w[]={ 0x1F000,   //in byte = 0.5mb NM
		       0x80000};  //in byte   2mb   ARM
    int chsz = 0x4000; //16*4kb
    
    int nm, arm, ARM_core;
    int i;
    
    PL_Word *R_Buf = (PL_Word *)0;
    PL_Word * W_Buf = (PL_Word *)0;
//    PL_Word *Buf[2];
    PL_Word to_fill = 0xafafafaf;
   PL_Word ck_fill = 0xafafafaf;  //0xa;
    int Check_Value;
    PL_Word Check_Addr, Check_Length;
    int exp_value = 2222;
 //  out thread
    FILE *fd_out;
    char file_res[128];
    char file_log[128];
    char result[256];
  // control loop
    int cnt_loop=0;
    time_t start_time, end_time, ml=0;
    int sec=0;
	int wait_time = one_sec*3;
	int work=0, core_number=0;

	tc_id(TC, ASSERTION);
	if(argc != 3){
		printf("Argument count != 3. argc=%d\n",argc);
		system("echo \"Fail_child - Argument count != 3. \" >> _res_102_01");
		return(-1);
	}
	core_number = atoi(argv[1]);
	work = atoi(argv[2]);	
	printf("Start 102_01_Process core_number=%d, work=%d \n",core_number,work);
	__try {// Handler Segmentation Fault
		arm = core_number/5;
		nm = core_number%5;
		if (nm == 0) {arm--; ARM_core=1;} else {ARM_core=0;}
		core = {nm-1,arm};

		//create file to out
//    sprintf_s(file_res,"coreout_res_%d,%d",nm-1,arm);
		sprintf_s(file_log,"coreout_log_%d,%d",nm-1,arm);
		sprintf_s(file_res,"coreout_%d,%d",nm-1,arm);
#pragma warning( push )
#pragma warning( disable : 4996 )
		if((fd_out=fopen(file_res, "wb")) == NULL){
			sprintf_s(fnd, "can not open %s",file_res);
			sprintf_s(exp, "open %s",file_res);
			err_msg(fnd, exp);
			test_res = UnTested;
			system("echo \"Fail_child - can not open file_res. \" >> _res_102_01");
			return(-1);
		}
#pragma warning( pop )
		sprintf_s(result,"core_number=%d work=%d This is file %s\n",
				core_number,work,file_res);
		fputs(result,fd_out);
		fflush(fd_out);
		//Buffers
		W_Buf = (PL_Word*)malloc(size_b[ARM_core]);
		R_Buf = (PL_Word*)malloc(size_b[ARM_core]);
		if ((W_Buf == (PL_Word *)0) || (R_Buf == (PL_Word *)0)){
			sprintf_s(fnd,"Cannot create buffer");
			sprintf_s(result,"Fail_child 11\n%s\n",fnd);
			fputs(result,fd_out);
			fflush(fd_out);
			return(-1);
		} 
		for (i=0; i <((size_b[ARM_core])>>2); i++) {
			W_Buf[i] = to_fill;
		}
		//prepare loop
		PL_SetTimeout(wait_time);
		PL_GetBoardCount(&count);
		if((count == 0) || (count == test_cnt)) {
			sprintf_s(fnd, "count = %d",count);
			sprintf_s(exp, "count >0 and count != test_cnt (%d)",test_cnt);
			err_msg(fnd, exp);
			sprintf_s(result,"%s\n",fnd);
			fputs(result,fd_out);
			fflush(fd_out);
			test_res = UnTested;
		} else if ((res = PL_GetBoardDesc(0, &board)) != PL_OK) {
			sprintf_s(fnd, "PL_GetBoardDesc returns %s %d",
					sp_error_name[res], res);
			sprintf_s (exp, "PL_GetBoardDes returns PL_OK");
			err_msg(fnd, exp);
			sprintf_s(result,"%s\n",fnd);
			fputs(result,fd_out);
			fflush(fd_out);
			test_res = Fail;
		} else if ((res = PL_GetAccess(board, &core, &access)) != PL_OK) {
			sprintf_s(fnd, "core_number=%d PL_GetAccess returns %s %d",
				core_number,sp_error_name[res], res);
			sprintf_s(exp, "PL_GetAccess returns PL_OK");
			err_msg(fnd, exp);
			sprintf_s(result,"%s\n",fnd);
			fputs(result,fd_out);
			fflush(fd_out);
			test_res = Fail;
		}else if ((res = PL_LoadProgramFile(access, prog[ARM_core])) != PL_OK) {
			sprintf_s(fnd, "core_number=%d PL_LoadProgramFile returns %s %d",
				core_number,sp_error_name[res], res);
			sprintf_s(exp,"PL_LoadProgramFile returns PL_OK");
			err_msg(fnd, exp);
			sprintf_s(result,"%s\n",fnd);
			fputs(result,fd_out);
			fflush(fd_out);
			test_res = Fail;
		}
		if (test_res !=Pass) {
			sprintf_s(result,"Fail_child 22\n%s\n",fnd);
			fputs(result,fd_out);
			PL_CloseAccess(access);
			return(-1);
		}
		sprintf_s(result,"Start loop\n");
		fputs(result,fd_out);
		fflush(fd_out);
		// loop
		start_time = GetTickCount();
		while (work>sec){
//printf("In loop work=%d pid_chld=%d core(%d,%d)\n",work,pid,nm-1,arm);
			if(cnt_loop == 0){
				printf("In loop core(%d,%d) working=%d cnt_loop=%d\n",
					nm-1,arm,sec,cnt_loop);
				sprintf_s(result,"In loop cnt_loop=%d\n",cnt_loop);
				fputs(result,fd_out);
				fflush(fd_out);
			}else if((sec%10) == 0){
				printf("In loop core(%d,%d) working=%dms cnt_loop=%d\n",
					nm-1,arm,sec,cnt_loop);
				sprintf_s(result,"Pass_child In loop cnt_loop=%d msec=%d\n",
				cnt_loop, sec);
				fputs(result,fd_out);
				fflush(fd_out);
			}

			if ((res = PL_WriteMemBlock(access,W_Buf,load[nm][0],\
						size_w[ARM_core])) != PL_OK) {
				sprintf_s(fnd,"core_number=%d "\
					"Processor (%d,%d) addr %x size %xb but\n"\
					" PL_WriteMemBlock returns %s %d",
					core_number,nm-1,arm,load[nm][0],size_b[ARM_core],
					sp_error_name[res], res);
				sprintf_s(exp, "PL_WriteMemBlock returns PL_OK");
				err_msg(fnd, exp);
				sprintf_s(result,"%s\n",fnd);
				fputs(result,fd_out);
				fflush(fd_out);
				test_res = Fail;
			}else if ((res=PL_SyncArray(access, chsz, load[nm][0], ck_fill, \
				&Check_Value, &Check_Addr, &Check_Length)) != PL_OK) {
				sprintf_s(fnd, "core_number=%d "\
					"Processor (%d,%d) PL_SyncArray returns %s %d",
					core_number,nm-1,arm,sp_error_name[res], res);
				sprintf_s(exp,"PL_SyncArray return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				sprintf_s(result,"%s\n",fnd);
				fputs(result,fd_out);
				fflush(fd_out);
				test_res = Fail;
			}else if ((res = PL_Sync(access, 2, &Check_Value)) != PL_OK) {
				sprintf_s(fnd, "Processor (%d,%d) PL_Sync returns %s %d",
						nm-1,arm,sp_error_name[res], res);
				sprintf_s(exp, "PL_Sync return PL_OK %d", PL_OK);
				err_msg(fnd, exp);
				sprintf_s(result,"%s\n",fnd);
				fputs(result,fd_out);
				fflush(fd_out);
				test_res = Fail;
			}else if (exp_value != Check_Value) {
				sprintf_s(fnd, "(%d,%d)-prog returns %d", nm-1,arm, Check_Value);
				sprintf_s(exp, "(%d,%d)-prog returns %d",nm-1,arm,exp_value);
				err_msg(fnd, exp);
				sprintf_s(result,"%s\n",fnd);
				fputs(result,fd_out);
				fflush(fd_out);
				test_res = Fail;
			}else if ((res=PL_ReadMemBlock(access,R_Buf,load[nm][0],\
					size_w[ARM_core])) != PL_OK){
				sprintf_s(fnd,"Processor (%d,%d) addr %x size %xb but\n"\
					"PL_ReadMemBlock returns %s %d",
					nm-1,arm,load[nm][0],size_b[ARM_core],
					sp_error_name[res], res);
				sprintf_s(exp, "PL_ReadMemBlock returns PL_OK");
				err_msg(fnd, exp);
				sprintf_s(result,"%s\n",fnd);
				fputs(result,fd_out);
				fflush(fd_out);
				test_res = Fail;
			}else{
//printf("In loop CHECK work=%d pid_chld=%d core(%d,%d)\n",work,pid,nm-1,arm);
				for(i=0; i<chsz; i++){
					if (R_Buf[i] != W_Buf[i]){
						sprintf_s(fnd,
						"Buffers are not compared R_Buf[%d]=%d W_Buf[%d]=%d",
							i,R_Buf[i],i,W_Buf[i]);
						sprintf_s(exp, "Buffers are compared");
						err_msg(fnd, exp);
						sprintf_s(result,"%s\n",fnd);
						fputs(result,fd_out);
						fflush(fd_out);
						test_res = Fail;
					}
				}
				if(cnt_loop/2) ck_fill++; else ck_fill--;
				for (i=0; i <chsz; i++) {
					W_Buf[i] = ck_fill;
				}
			}
			if (test_res !=Pass) break;
			cnt_loop++;
			end_time = GetTickCount();
			sec = end_time - start_time;
		}
	} // End __try
    __except((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? \
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
    {
        printf("ERROR: Found SIGSEGV\n");
        sprintf_s(fnd, "ERROR: Found SIGSEGV");
        sprintf_s (exp, "the test without any exceptions");
        err_msg(fnd, exp);
		sprintf_s(result,"%s\n",fnd);
		fputs(result,fd_out);
		fflush(fd_out);
        test_res = UnTested;
    }
	//core test result out
	if (test_res !=Pass) {
		printf("Fail_child cnt_loop=%d Finish core(%d,%d)\n",
				cnt_loop,nm-1,arm);
		sprintf_s(result,"Fail_child cnt_loop=%d\n",cnt_loop);
		fputs(result,fd_out);
	}else{
		printf("Pass_child cnt_loop=%d Finish core(%d,%d)\n",
				cnt_loop,nm-1,arm);
		sprintf_s(result,"Pass_child cnt_loop=%d\n",cnt_loop);
		fputs(result,fd_out);
	}
	sprintf_s(result,"work time %ds count loop %d\n",sec,cnt_loop);
	fputs(result,fd_out);
	fflush(fd_out);
    return(0);
}
