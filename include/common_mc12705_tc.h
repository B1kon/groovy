#ifndef __USR_TEST_common__
#define __USR_TEST_common__

#ifdef _WIN32
// ------------------------------
#include <tchar.h>
#include <Windows.h>

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <io.h>
//#include <Psapi.h>

#include <locale.h>
#include <mc12705load.h> // mc12705

#else	// linux
// ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <locale.h>

#include <mc12705load.h> // mc12705

#define sprintf_s	sprintf

#endif	// _WIN32
// ------------------------------
#define	Pass		0
#define	Fail		1
#define	UnTested	2

int one_sec = 1000; // mlsec

const char* sp_error_name[] = {
		"PL_OK",		//0
		"PL_ERROR",		//1
		"PL_TIMEOUT",		//2
		"PL_FILE",		//3
		"PL_BADADDRESS",	//4
};

void tc_id(const char* TC_NO, const char* TC_ASSERTION)
{
    setlocale(LC_ALL, "rus");
    printf("Start TC_%s\n", TC_NO);
    printf("ASSERTION: %s\n", TC_ASSERTION);
}

void err_msg( char* found, char* expected)
{
    printf("    ERROR MESSAGE\n"); 
    printf("FOUND\n%s\n", found);
    printf("EXPECTED\n%s\n", expected);
}

void result_msg(const char* TC_NO, int test_res)
{
    char msg [100];

    sprintf_s(msg,"%s", "No test result");
    if(test_res == Pass) 
	sprintf_s(msg,"%s", "PASS");

    else if(test_res == Fail)
	sprintf_s(msg,"%s", "FAIL");

    else if(test_res == UnTested)
	sprintf_s(msg,"%s", "UNTESTED");
    
    printf("\nTC_%s %s\n", TC_NO, msg);
}

void wait_mlsec(int mls)
{
#ifdef _WIN32
	Sleep(mls);
#else	// linux
    struct timeval start;
    struct timeval end;
    time_t s, ms, ml=0;
    char mmm [256];

    gettimeofday(&start, NULL);
	
    while (ml < mls)
    {
	sprintf_s(mmm, "wait_mlsec %d",mls); //spend time
	gettimeofday(&end, NULL);
	s = end.tv_sec - start.tv_sec;
	ms = end.tv_usec - start.tv_usec;
	if(ms<0){ s--; ms+=1000000; }
	ml = (s*1000)+(ms/1000);
    }
#endif // _WIN32
}

#ifdef _WIN32
#define bzero(a, sz) memset(a,0,sz)

#endif	// _WIN32

///////////////////// WINDOWS ////////////////////////////////

#ifdef _WIN32
//
//	wait, when status will be PROGRAM_FINISHED
//
int WaitEndProgram(PL_Access *access, int time) {
	char fnd [256];
	char exp [256];
	int res, w, wait_programm = PL_TIMEOUT;
	PL_Word status;
	for(w=0; w<3; w++) {
		wait_mlsec(time/3);
		if ((res = PL_GetStatus(access, &status)) != PL_OK) {
		    sprintf_s(fnd, 256, "PL_GetStatus returns %s %d",
    				sp_error_name[res], res);
    		    sprintf_s(exp, 256, "PL_GetStatus returns PL_OK %d", PL_OK);
		    err_msg(fnd, exp);
		    wait_programm = PL_ERROR;
		    break;
		}
		if (status == PROGRAM_FINISHED) { 
			wait_programm = PL_OK; //OK
			break;
		}
	}
	return(wait_programm);
}

#else	// linux
//
//	wait, when status will be PROGRAM_FINISHED
//
int WaitEndProgram(PL_Access *access, int time) {
    char fnd [256];
    char exp [256];
    int res, w, wait_programm = PL_TIMEOUT;
    PL_Word status;
    for(w=0; w<3; w++) {
	usleep(time/3);
	if ((res = PL_GetStatus(access, &status)) != PL_OK) {
	    sprintf_s(fnd, "PL_GetStatus returns %s %d",
    				sp_error_name[res], res);
    	    sprintf_s(exp, "PL_GetStatus returns PL_OK %d", PL_OK);
	    err_msg(fnd, exp);
	    wait_programm = PL_ERROR;
	    break;
	}
	if (status == PROGRAM_FINISHED) {
	    wait_programm = PL_OK; //OK
	    break;
	}
    }
    return(wait_programm);
}

#endif	// _WIN32

#ifdef _WIN32
//
// Defines the process with result Pass
// The process save his result in file
//
int check_proc_res(char* file_res) {
    char fnd [256];
    char exp [256];
    FILE *fd;
    char *fres;
    int find_res = 0;
    char ln[256];
	int pass_cnt = 0;
	int test_res = Pass;
	
#pragma warning( push )
#pragma warning( disable : 4996 )

	if((fd=fopen(file_res, "rb")) == NULL){
		sprintf_s(fnd, "can not open %s",file_res);
		sprintf_s(exp, "open %s",file_res);
		err_msg(fnd, exp);
		test_res = Fail;
		return(0-test_res);
	}
#pragma warning( pop )
    while(!feof(fd)){
//		printf("feof=%d\n",feof(fd));
		if (((fres = fgets(ln,256,fd)) == NULL) && (feof(fd) == 0)){
			sprintf_s(fnd, "reading error of %s",file_res);
			sprintf_s(exp, "no error");
			err_msg(fnd, exp);
			test_res = Fail;
			break;
		}
//printf("check_proc_res ln=%s\n",ln);
		if(strstr(ln,"Fail") != NULL) {
			sprintf_s(fnd, "%s contains Fail",file_res);
			sprintf_s(exp, "only Pass");
			err_msg(fnd, exp);
			find_res = 0;
			break;
		}
		if(strstr(ln,"Pass") != NULL) find_res++;
//printf("check_proc_res find_res=%d\n",find_res);
	}
	fclose(fd);
printf("check_proc_res test_res=%d find_res=%d\n",test_res,find_res);
	if(test_res != Pass) return(0-test_res);
	if (find_res != 0) pass_cnt++;
    return(pass_cnt);
}

#endif	// _WIN32

#endif	//__USR_TEST_common__
