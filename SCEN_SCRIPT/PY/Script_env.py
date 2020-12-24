
# -*- coding: utf-8 -*-

######### -*- coding: cp1251 -*-

import sys, os, time, datetime, string, threading, re, tempfile
#import signal, errno, thread, getopt
import signal, errno, getopt
from Common import *

# Global environment
    #save
global scen_file, save_dir, log_file, result_file, file_exec_pid, execute_test_res
global env_file

# scen log, test Log, test result files, enviroment file
f_env = None
f_log = None
f_resalt = None
scen_log = None
file_exec_pid = 'file_execute_test_pid'
need_Usage = False
xprnt = 0
arg_ind = 1
full_path = None

#time
time_struct = time.localtime()
tm_year = str(time_struct.tm_year)
tm_mon = str(time_struct.tm_mon)
tm_mday = str(time_struct.tm_mday)
tm_hour = str(time_struct.tm_hour)
tm_min = str(time_struct.tm_min)
tm_sec = str(time_struct.tm_sec)

def substitute(rln = ''):
#{
    global env_file

    if xprnt:print ("start substitute\n")
    s = None
    if (env_file is None):
        return(rln)
    if xprnt:print ("substitute  env_file=",env_file)
    re_env = re.compile("%.+?%")
    r = re_env.findall(rln)
    if r == []:
        return(rln)
#    print ("substitute  re_env.findall(rln) = ", r
    for sbt in r:
        e = (sbt.split("%"))[1]
#       print ("sbt=",sbt
#       print ("e=",e
        try:
            f_env = open(env_file)
        except IOError as err:
            print ("Can open env file %s " % env_file)
            print(err)
            return(None)
        for e_ln in f_env.readlines():
 #           print ("e_ln=",e_ln
            # delete all blanks from start of the line
            e_ln = del_start_blanks(e_ln)
            # delete symbol "end line"
            e_ln = del_end_line(e_ln)
            # delete all blanks from end of the line
            e_ln = del_end_blanks(e_ln)
            if e_ln == "":
                continue
 #           print ("e_ln=",e_ln
 #           if e in e_ln:
            if e == (e_ln.split()[0]):
 #               print ("e_ln=",e_ln
 #               s = ((e_ln.split())[1:]).pop()
                s = " ".join(((e_ln.split())[1:]))
        if s == [] :
            print ("substitute ERROR: Undefined env =%s in line=%s" %(e, rln) )
            f_env.close
            return(None)
        if xprnt:print ("sbt=%s  s=%s" %(sbt, s) )
        tmp1 = rln.replace(sbt, s)
        rln = tmp1
        f_env.close
    if xprnt:print ("end substitute\n")
    return(rln)
#} end substitute
'''
Function cmd_wait
  wait arg_time seconds.
   Result:
    =0 - successful result
    <0 - error result in the function
'''
def cmd_wait(arg_time = []):
#{
    if len(arg_time) < 1 :
        wait_time = 0
    else :
        wait_time = ''.join(arg_time)
        # delete all blanks from start of the line
        wait_time = del_start_blanks(wait_time)
   
    print ("wait_time=",wait_time)
    try:
        need_time = int(wait_time[0])
    except:
        print ("cmd_wait ERROR: UNSPECIFIED wait time=",wait_time)
        return(-1)

    while(need_time > 0 ):
    #{
        need_time-=1
        time.sleep(1)
    #}
    return(0)
#} end cmd_wait

'''
Function cmd_wait_answer
  wait input any symbol.
   Result:
    =0 - successful result
    <0 - error result in the function
'''
def cmd_wait_answer():
#{
    promt = 'To continue input any symbol'
    try:
        input=raw_input(promt)
        scen_log.write(time.ctime()+'  '+promt+' '+input+'\n')
        return(0)
    except IOError as err:
        print(err)
        return(-1)
#} end cmd_wait_answer

'''
Function cmd_print
  Execute printf with argument.
   Result:
    =0 - successful result
    <0 - error result in the function
'''
def cmd_print(arg_text = ''):
#{
    arg_text = substitute(arg_text)
    try:
        if arg_text is None:
            return(-1)
        print (arg_text)
        return(0)
    except:
        return(-1)
#} end cmd_print


'''
Function loop_time
  Execute scenario during 'time' seconds.
  If the scenario finished early, loop_time repeats one.
   Result:
    =0 - successful result
    <0 - error result in the function
'''

def loop_time(
    arg_time = [],
    arg_scen = ''):
#{
    exec_time = 0
    t_pth = None


    def execute_scen(
        exec_time=0,
        t_pth = ''):
    #{
        bgn_t = int(str(time.clock()).split('.')[0])
        end_t = bgn_t + exec_time
        crnt_t = -1
##        print ("bgn_t=%d end_t=%d crnt_t=%d" %(bgn_t, end_t, crnt_t)
##        print ('\\\\\ execute_scen \\\\\\\\\\\\\\\\ t_pth=', t_pth
        # ------------
        i = 0
        while(crnt_t < end_t):
            i+= 1
            if xprnt:print ('='*10+' start execute_scen ''='*10+str(i) )
            scen_exec(t_pth, False)
            crnt_t = int(str(time.clock()).split('.')[0])

        if xprnt:print ("bgn_t=%d end_t=%d crnt_t=%d" %(bgn_t, end_t, crnt_t) )
        return(0)
    #}

    if xprnt:print ('='*10+' start loop_time '+'='*10 )
    # delete symbol "end line"
    t_pth = del_end_line(arg_scen)
    t_pth = del_start_blanks(t_pth)

    t_pth = substitute(t_pth)
    if t_pth is None:
        print ("loop_time: Can not substitute %s " % t_pth)
        return(-1)
    try:
        file_res = open(result_file, 'a+', -1)
    except IOError as err:
        print ("11 Can not prepare result file %s " % result_file)
        print(err)
        return(-1)    
    msg = '='*10+' TEST RESULTS OF SCEN '+t_pth+' '+'='*10+os.linesep
    file_res.write(msg)

    if not (os.path.exists(t_pth)) :
        print ("ERROR: scen %s does not exist" % t_pth)
        file_res.write("ERROR: scen "+t_pth+" does not exist\n")
        file_res.close()
        return(-1)
    # ExecTime
    # ================================
    if len(arg_time) < 1 :
        exec_time = 0
    else :
        exec_time = ''.join(arg_time)
        # delete all blanks from start of the line
        exec_time = del_start_blanks(exec_time)
   
    try:
        need_time = int(exec_time[0])
        tout = need_time
    except:
            print ("ERROR: UNSPECIFIED LoopCount=",count)
            return(-1)

    # Execute scen
    try:    
        sc1 = threading.Thread(target= execute_scen, name="scn",
                          args=[need_time, t_pth])
    except:
        print ('Thread is not created. Test:',cmd_string)
        file_res.write('ERROR: Thread is not created'+os.linesep)
        file_res.close()
        return (-2)

    try:
        bbb_t = int(str(time.clock()).split('.')[0])
        r = sc1.start()
    except:
        print ('ERROR in start scen')
        if (sc1.isAlive()) :
            print ('exception in start scen but thread is active\n')
            file_res.write('ERROR: exception in start scen but thread is active'+os.linesep)
            file_res.close()
            return (-3)
        else :
            print ('exception in start thread with scen\n')
            file_res.write('ERROR: can not start thread with scen'+os.linesep)
            file_res.close()
            return (-4)

    try:
        # waite sc1 finish
        r = sc1.join()

    except:
        print ('JOIN ERROR')
        file_res.write('ERROR: -1 in start scen'+os.linesep)
        file_res.close()
        return (-4)
    file_res.write('='*10+' END OF SCEN '+t_pth+' '+'='*10+os.linesep)
    file_res.close()
    return(0)
#}

'''
Function loop_cntr
  repeat scenario "count" times
   Result:
    =0 - successful result
    <0 - error result in the function
'''
def loop_cntr(arg_count = [],
              arg_scen = ''):
#{
    cnt_loop = 0
    t_pth = None

    if xprnt:print ('='*10+' start loop_cntr'    )

    # delete symbol "end line"
    t_pth = del_end_line(arg_scen)
    t_pth = del_start_blanks(t_pth)

    t_pth = substitute(t_pth)
    if t_pth is None:
        print ("loop_cntr: Can not substitute %s " % t_pth )
        return(-1)    

    try:
        file_res = open(result_file, 'a+', -1)
    except IOError as err:
        print ("22 Can not prepare result file %s " % result_file)
        print(err)
        return(-1)    

    msg = '='*10+' TEST RESULTS OF SCEN '+t_pth+' '+'='*10+os.linesep
    file_res.write(msg)

    if not (os.path.exists(t_pth)) :
        print ("ERROR: scen %s does not exist" % t_pth)
        file_res.write("ERROR: scen "+t_pth+" does not exist\n")
        file_res.close()
        return(-1)

    # LoopCount
    # ================================
    if len(arg_count) < 1 :
        count = 0
    else :
        count = ''.join(arg_count)
        # delete all blanks from start of the line
        count = del_start_blanks(count)
    # =================================    

    try:
        need_count = int(count[0])
    except:
            print ("ERROR: UNSPECIFIED LoopCount=",count)
            return(-1)
    
    while ( (cnt_loop < need_count) or (need_count == 0) ):
    #{
        cnt_loop =+ 1
        if xprnt:print ("Scenario=",t_pth)
        scen_exec(t_pth, False)
    #}

    file_res.write('='*10+' END OF SCEN '+t_pth+' '+'='*10+os.linesep)
    file_res.close()        
    return (0)
#} end of loop_cntr


'''
Function cmd_run
  Call test, check timeout.
  Result:
    Result:
    =0 - successful result
    <0 - error result in the function
'''
def cmd_run (
    arg_timeout = [],
    #timeout = 0,
    arg_cmd_string = ''    
):
#{
    global scen_file, save_dir, log_file, result_file, file_exec_pid, execute_test_res
    timeout = None
    cmd_string = None
    res_run = None
    need_timeout = 0
    tout = 0
    execute_test_res = -1
    tmp_log = os.path.dirname(log_file)+os.sep+'tmp_log_file'
    is_teat_res = 0
    
    def execute_test(
        f_res,      # result file
        test='echo ooo',
        log=''      # log file
    ):
    #{
        global file_exec_pid, execute_test_res
        cmd = None
        exec_pid = os.getpid()
        try:
            if xprnt:print ('='*10+' execute_test start '+'='*10)
            cmd_rt = 'python run_test_1.py '+str(xprnt)+' '+file_exec_pid
#            cmd_tst = str(test)+' >> '+log
            cmd_tst = str(test)+' > '+tmp_log+'; '
            if 'win' in sys.platform :
                cmd_log = 'type '+tmp_log+' >> '+log
            else :
                cmd_log = 'cat '+tmp_log+' >> '+log
#            cmd = cmd_rt+' "'+cmd_tst+'"'
            cmd = cmd_rt+' "'+cmd_tst+cmd_log+'"'
            #################
#            print ('cmd_rt= ', cmd_rt
#            print ('cmd_tst= ', cmd_tst
#            print ('test= ', str(test)
#            print ('cmd_log= ', cmd_log
            #################
            if xprnt:print ('cmd= ', cmd)
            execute_test_res = os.system(cmd)
            print ("execute_test_res = ",str(execute_test_res) )
            if xprnt:print ("execute_test_res = ",str(execute_test_res))
#        except IOError, exc:
        except IOError as exc:
            print ("ERROR I/O", exc)
            f_res.write("ERROR: I/O"+os.linesep)
        except:
            print ("ERROR: UNSPECIFIED Exception\n")
            f_res.write("ERROR: UNSPECIFIED Exception"+os.linesep)
 
    #} end execute_test
    
    if xprnt:print ('='*10+' start cmd_run')
    cmd_string = arg_cmd_string
    
    if (cmd_string == ''):
        print ('There is no path to test\n')
        return(-1)

    # delete symbol "end line"
    cmd_string = del_end_line(cmd_string)
    cmd_string = del_start_blanks(cmd_string)

    cmd_string = substitute(cmd_string)
    if cmd_string is None:
        print ("cmd_run: Can not substitute %s " % cmd_string)
        return(-1)      
    # TimeOut
    if len(arg_timeout) < 1 :
        timeout = 0
    else :
        timeout = ''.join(arg_timeout)
        # delete all blanks from start of the line
        timeout = del_start_blanks(timeout)

    try:
        tout = int(timeout)
    except:
            print ("ERROR: UNSPECIFIED TimeOut=",timeout)
            return(-1)
    need_timeout = tout

    # LogFile and ResultFile    
    try:
        file_res = open(result_file, 'a+', -1)
    except IOError as err:
        print ("33 Can not prepare result file %s " % result_file)
        print(err)
        return(-1)    

    print ('TEST: ' + str(cmd_string))
    file_res.write('TEST: ' + str(cmd_string)+os.linesep)

    try:    
        p1 = threading.Thread(target= execute_test, name="tst",
                          args=[file_res, cmd_string, log_file])
    except:
        print ('Thread is not started. Test:',cmd_string)
        file_res.write('ERROR: Thread is not started'+os.linesep)
        file_res.close()
        return (-2)

    try:
        r = p1.start()
    except:
        print ('ERROR in start test\n')
        if (p1.isAlive()) :
            print ('exception in start test but thread is active\n')
            file_res.write('ERROR: exception in start test but thread is active'+os.linesep)
            file_res.close()
            return (-3)
        else :
            print ('exception in start test\n')
            file_res.write('ERROR: -1 in start test'+os.linesep)
            file_res.close()
            return (-4)

#    while (p1.isAlive()):
    while (p1.is_alive()):
    #{
        
        if xprnt:print (str(time.ctime()))
        if (need_timeout == 0):
            time.sleep(1)
            continue
        elif (tout > 0) :
            tout -= 1
            time.sleep(1)
            continue
##        elif ((tout < 0) and (tout >= -5)):
##            threading.Thread._Thread__stop(p1)
##            # ERROR Timeout
##            tout -= 1
##            if xprnt:print ('(tout < 0) and (tout >= -5)\n'
##            execute_test_res = TIMEOUT
        elif (tout < 0):
            print ('ERROR: out < -5\n')
            file_res.close()
            return (-5)
        else :
            # tout == 0
            tout -= 1
            try:
                p_file = open(file_exec_pid, 'r')
            except IOError as err:
                print ("Can not open ",file_exec_pid)
                print(err)
                return(-5)

            exec_pid = p_file.readline()
            if 'win' in sys.platform :
                kill_cmd = 'taskkill /PID '+str(int(exec_pid))+' /T /F'
            else :
                kill_cmd = 'kill '+'-9 '+str(int(exec_pid))
            
##            if os.uname()[0] == 'Linux' :
##                kill_cmd = 'kill '+'-9 '+str(int(exec_pid))
##            else :
##                kill_cmd = 'taskkill /PID '+str(int(exec_pid))+' /T /F'
            
            if xprnt:print ('kill_cmd',kill_cmd)
            try:
                os.system(kill_cmd)
            except:
                print ('ERROR: Can not kill test by timeout\n')
                return(-6)
            # wait then os.system returns result
            i=0
            while(i<5):
            #{
                if execute_test_res < 0 :
                    time.sleep(1)
                    i+=1
                else : i = 5
            #}
            if execute_test_res < 0 :
                print ("ERROR: during 5 sec. os.system doen't return result "\
                      "after killing")
                return(-7)
            execute_test_res = TIMEOUT
            # ===============
    #}
    #==============================
    if ( (execute_test_res >= len(RSLT)) or (execute_test_res <0) ):
        print ("ERROR UNSPECIFIED test result= ",str(execute_test_res))
        file_res.write("ERROR: UNSPECIFIED test result= "+str(execute_test_res)+os.linesep)
    else:
        ll = []
        #======== Check test result =============
        try:
#	    print ("tmp_log = ",tmp_log
            p_log = open(tmp_log, 'r')
        except IOError as err:
            print ("Can not open log_file",tmp_log)
            print(err)
            return(-5)
        is_teat_res = 0
        for line in p_log.readlines() :
	#{
 #         print ("XXXXX",line,)
          if  re.search("TEST RESULTS OF SCEN", line):
              continue
          if  re.search("^TC_", line):
             if xprnt:print (line,)
             a = line.split(' ')
             if (len(a) == 2):
               ll = ll + a
               is_teat_res = 1
#               print ('RESULT: '+a[1])  #+os.linesep)
#               file_res.write('RESULT: '+a[1]+os.linesep)
#             else:
#               print ("Can not test result",line)
#               return(-5)
	#}
    if (is_teat_res == 1):
        print ('RESULT: '+ll[-1])  #+os.linesep)
        file_res.write('RESULT: '+ll[-1]+os.linesep)

    else:   # if (is_teat_res == 0):
           print ('RESULT: UNTESTED'+os.linesep)
           file_res.write('RESULT: UNTESTED'+os.linesep)

    #=============================
    file_res.write('*'*10+os.linesep)
    file_res.close()
    return(0)
#} end cmd_run

def cmd_env(path_to_env = []):
#{
    global env_file
    if xprnt:print ("start cmd_env\n")
    if not(len(path_to_env) == 1) :
        print ('Wrong path to env' % path_to_env)
        return(-1)

    # prepare path
    tmp1 = path_to_env.pop()
#    print ("tmp1 = ",tmp1
    env_path = ((os.sep.join(tmp1.split(os.sep))).split()).pop()
    start_env_pth = env_path.split(os.sep)[0]
    env_file = env_path
    if start_env_pth == '..' :
        # not full path. make full
        full_env_path = os.getcwd()+os.sep+env_path
        if xprnt:print ('full_env_path = ',full_env_path)
        env_file = full_env_path
    if xprnt:print ('env_file = ',env_file)
#    print ('env_file = ',env_file

    return(0)    
#} end cmd_env
    
""" """""""""""""""""""""""""""""""""""""""""""
 Create directory where will be saved files of test log and result
 Name of log file log_hh_mm_sec
 Name of result file result_hh_mm_sec
"""""""""""""""""""""""""""""""""""""""""""""" """
def cmd_save(path_to_save = []):
#{
    global scen_file, save_dir, log_file, result_file, full_path
    
    if xprnt:print ("start cmd_save\n")
    if not(len(path_to_save) == 1) :
        print ('Wrong path to scen' % path_to_save)
        return(-1)
    # prepare path
    tmp1 = path_to_save.pop()
#    print ("tmp1 = ",tmp1

    tmp1 = substitute(tmp1)    
    if tmp1 is None:
        print ("cmd_save: Can not substitute %s " % tmp1)
        return(-1)  
    save_dir = ((os.sep.join(tmp1.split(os.sep))).split()).pop()
    start_pth = save_dir.split(os.sep)[0]
    if start_pth == '..' :
        # not full path. make full
        #full_save_dir = full_path+os.sep+save_dir
        full_save_dir = os.getcwd()+os.sep+save_dir
        if xprnt:print ('full_save_dir = ',full_save_dir)
        save_dir = full_save_dir
    if xprnt:print ('save_dir = ',save_dir)
    print ('save_dir = ',save_dir)
    
    try:
        if not (os.path.exists(save_dir)) :
            os.makedirs(save_dir)
        elif not (os.path.isdir(save_dir)) :
            print ("ERROR %s is not directory" % save_dir)
            return(-1)
    except:
        print ("Can not create directory %s " % save_dir)
        return(-1)
    
    log_file_name = 'log_'+tm_hour+'_'+tm_min+'_'+tm_sec
    log_file = save_dir+os.sep+log_file_name
    try:
        f_log = open(log_file, 'w')
    except IOError as err:
        print ("Can not prepare log file %s " % log_file)
        print(err)
        return(-1)
    try:
        msg = '========== LOG OF SCEN '+scen_file+' =========='+os.linesep
        f_log.write(msg)
        f_log.close()
    except IOError as err:
        print ("Can not write into log file %s " % log_file)
        print(err)
        return(-1)
    result_file_name = 'result_'+tm_hour+'_'+tm_min+'_'+tm_sec
    result_file = save_dir+os.sep+result_file_name
    try:
      f_resalt = open(result_file, 'w', -1)
      msg = '='*10+' TEST RESULTS OF SCEN '+scen_file+' '+'='*10+os.linesep
      f_resalt.write(msg)
      f_resalt.close()
    except IOError as err:
        print ("44 Can not prepare result file %s " % result_file)
        print(err)
        return(-1)
    # success result
    return(0)
#} end cmd_save

##################################
# Analyse scen file
# Execute commands of scen
#####################################
def scen_exec (
    path_to_scen,
    need_exec_save = True
):
    scen_cmd = []
    if xprnt:print ('Start scen_exec! = ',path_to_scen+'!\n')

    path_to_scen = substitute(path_to_scen)
    if path_to_scen is None:
        print ("scen_exec: Can not substitute %s " % path_to_scen)
        return(-1)
    
    short_scen_cmd = re.compile(r"^-save|^-wait_answer|^-wait|^-env")
    long_scen_cmd = re.compile(r"^-loop_cnt |^-loop_time|^-run|^-print")
    end_scen_cmd = re.compile(r"^-end|^-end_loop")

    f_scn=None

    try:
        f_scn = open(path_to_scen, 'r')
    except IOError as err:
        print ("Can not open scen %s error()" % (path_to_scen))
        print(err)
        return (-1)

    ln_a = len(f_scn.readlines())
    if xprnt:print ("len=", ln_a)
    f_scn.seek(0)
    i = -1
    while i<(ln_a) :
    #{
        i = i + 1
        if xprnt:print ("i=",i)
        # find scen command
        line=f_scn.readline()
 
        if len(line) < 2 :
            continue
        
        # delete all blanks from start of the line
        line = del_start_blanks(line)
        
        # delete symbol "end line"
        line = del_end_line(line)
        
        # delete all blanks from end of the line
        line = del_end_blanks(line)
        
        scen_cmd = ((line.replace('\t',' ')).split(" "))
                
        # detach one command
        if (not(short_scen_cmd.findall(line) == [])):
        #{   ------------- Short Command ---------
            if xprnt:print ("short_scen_cmd", scen_cmd)

            if ("-save" in scen_cmd):
                if xprnt:print ("Command SAVE\n")
                if need_exec_save:
                    scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                    r = cmd_save(scen_cmd[1:])
                    if xprnt:print ("Result cmd_save =", r)
                    scen_log.write(time.ctime()+'  '+'Result cmd_save='+str(r)+'\n')
                    if (r != 0): return(-1)
                else :
                    continue
            # -----------------------------------------
            elif ("-wait_answer" in scen_cmd):
                if xprnt:print ("Command WAIT answer\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                r = cmd_wait_answer()
                if xprnt:print ("Result cmd_wait_answer =", r)
                scen_log.write(time.ctime()+'  '+'Result cmd_wait_answer='+str(r)+'\n')
            # -----------------------------------------
            elif ("-wait" in scen_cmd):
                if xprnt:print ('Command WAIT\n')
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                r = cmd_wait(scen_cmd[1:])
                if xprnt:print ("Result cmd_wait =", r)
                scen_log.write(time.ctime()+'  '+'Result cmd_wait='+str(r)+'\n')
            # -----------------------------------------
            elif ("-env" in scen_cmd):
                if xprnt:print ("Command ENV\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                r = cmd_env(scen_cmd[1:])
                if xprnt:print ("Result cmd_env =", r)
                scen_log.write(time.ctime()+'  '+'Result cmd_env='+str(r)+'\n')
            else :
                print (scen_cmd, " is not short scen command\n")
                return(-1)
        #}
 
        if (not(long_scen_cmd.findall(line) == [])):
        #{   ------------- Long Command ---------
            if xprnt:print ("long_scen_cmd=",scen_cmd)
            
            # choose what to do
            t_pth = ''
            old_i = i
            while i < ln_a :
            #{
                i = i + 1
                t_ln=f_scn.readline()
                
                if (end_scen_cmd.findall(t_ln) == []):
                    t_pth = t_pth + t_ln
                    continue
                break
            #} end while loop
            if xprnt:print ('Command lines =',t_pth+'@')

            # Exec command            
            if ("-run" in scen_cmd):
                if xprnt:print ("Command RUN\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                scen_log.write('    TEST: '+t_pth+'\n')
                r = cmd_run(scen_cmd[1:], t_pth)
                if xprnt:print ("Result cmd_run =", r)
                scen_log.write(time.ctime()+'  '+'Result cmd_run='+str(r)+'\n')
            # -----------------------------------------
            elif ("-loop_cnt" in scen_cmd):
                if xprnt:print ("Command LOOP_CNT\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                scen_log.write('    PATH: '+t_pth+'\n')
                r = loop_cntr(scen_cmd[1:], t_pth)
                if xprnt:print ("Result loop_cntr =", r)
                scen_log.write(time.ctime()+'  '+'Result loop_cntr='+str(r)+'\n')
            # -----------------------------------------
            elif ("-loop_time" in scen_cmd):
                if xprnt:print ("Command LOOP_TIME\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                scen_log.write('    PATH: '+t_pth+'\n')
                r = loop_time(scen_cmd[1:], t_pth)
                if xprnt:print ("Result loop_time =", r)
                scen_log.write(time.ctime()+'  '+'Result loop_time='+str(r)+'\n')
            # -----------------------------------------
            elif ("-print" in scen_cmd):
                if xprnt:print ("Command PRINT\n")
                scen_log.write(time.ctime()+'  '+' '.join(scen_cmd)+'\n')
                scen_log.write('    TEXT: '+t_pth+'\n')
                r = cmd_print(t_pth)
                if xprnt:print ("Result cmd_print (=", r)
                scen_log.write(time.ctime()+'  '+'Result loop_time='+str(r)+'\n')
            else :
                print (scen_cmd, " is not long scen command\n")
                return(-1)

        #}            
        if (not(end_scen_cmd.findall(line) == [])):
            print ("end_scen_cmd\n")
            print (end_scen_cmd.findall(line))
            print (scen_cmd, " end scen command can not be found here\n")
            return(-1)
    #} end while loop

    return 0
#} end scen_exec

def Usage():
    usg = """
Call:
    python -u Script1.py [-d] scen[,scen1, ...]
        -u  meens stdout without buffer
Options:
	-d include extend print
	   (Default: exclude).
    """
    print (usg)
    sys.exit(1)
# End Usage
##############################
# main
##############################
#===============================
# option

try:
    opts, tmp_args = getopt.getopt(sys.argv[1:],"d")
    if xprnt:print ('opts= ',opts)
    if xprnt:print ('tmp_args= ',tmp_args)
except getopt.GetoptError:
    need_Usage = True
try:
    for o, v in opts:
        if o == "-d":
            xprnt = 1
            arg_ind += 1
except :
    need_Usage = True
if need_Usage:
    Usage()
#===================================
arg = sys.argv[arg_ind:]
argc = len(arg)
if xprnt:print ('sys.argv', sys.argv[0])
full_path = os.sep.join((sys.argv[0]).split(os.sep)[:-1])
#print ('full_path=',full_path
scen_log_dir = 'Scen_Log'
if (argc == 0 ):
    print ("There is no scen\n")
    exit(-1)

if not (os.path.exists(scen_log_dir)) :
    try:
        os.makedirs(scen_log_dir)
    except:
        print ("Can not create directory %s " % scen_log_dir)
        exit(-1)
elif not (os.path.isdir(scen_log_dir)) :
    print ("ERROR %s is not directory" % scen_log_dir)
    exit(-1)

scen_log_file_name = 'scen_log_'+tm_hour+'_'+tm_min+'_'+tm_sec
scen_log_file = scen_log_dir+os.sep+scen_log_file_name
try:
    scen_log = open(scen_log_file, 'w')
except IOError as err:
    print ("Can not prepare scen log file %s " % scen_log_file)
    print(err)
    exit(-1)


env_file = None


for path_to_scen in arg :
#{
    if xprnt:print ("New Scen", path_to_scen)
    # prepare path
    tmp1 = ' '.join(path_to_scen.split())
    scen_file = os.sep.join(tmp1.split(os.sep))

    if xprnt:print ("New Scen", scen_file)
    
    scen_exec(scen_file)
#}

rs = {RSLT[PASS]:0
    ,RSLT[FAIL]:0
    ,RSLT[UNTESTED]:0
    ,RSLT[TIMEOUT]:0
    }

try:
    file_res = open(result_file, 'r')
except IOError as err:
    print ("Can not open result file %s " % result_file)
    print(err)
    exit(-1)

for line in file_res.readlines() :
    if 'RESULT: ' in line :
        rs[line.split(' ')[1].rstrip()] +=1

file_res.close()

try:
    file_res = open(result_file, 'a+')
except IOError as err:
    print ("Can not open result file %s " % result_file)
    print(err)
    exit(-1)

print ('SUMMARY RESULT:'+os.linesep)
file_res.write('SUMMARY RESULT:'+os.linesep)
for rslt in range(PASS,TIMEOUT+1):
    print (RSLT[rslt]+':\t'+str(rs[RSLT[rslt]]) )
    file_res.write(RSLT[rslt]+':\t'+str(rs[RSLT[rslt]])+os.linesep)

exit(0)
