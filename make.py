#-*- coding: utf-8 -*-

#
#    Provide compiltion all test cases
#

import sys, os, time, datetime, string, threading, re, tempfile
import signal, errno, getopt
import glob
import shutil
#import thread

xprnt = 0
clean_only = 0 
arg_start = 1
need_Usage = 0


def Usage():
    usg = """
Call:
    make.py [-c] [-d] [test_suite, ...]
 
Options:
    -c only clean
	-d include extend print
	   (Default: exclude).
test_suite - By default call make files in directories Test_Suite*
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
    opts, tmp_args = getopt.getopt(sys.argv[1:],"dc")
##    if xprnt:print ('opts= ',opts)
##    if xprnt:print ('tmp_args= ',tmp_args)
except getopt.GetoptError:
    need_Usage = True
try:
    for o, v in opts:
        if o == "-d":
            xprnt = 1
            arg_start += 1
        if o == "-c":
            clean_only = 1
            arg_start += 1
except:
    need_Usage = True
if need_Usage:
    Usage()
#===================================

arg = sys.argv[arg_start:]
argc = len(arg)
print ('arg=', arg)
print ('sys.argv=', sys.argv)
print ('clean_only=', clean_only)
print ('arg_start=', arg_start)
pwd = os.getcwd()
full_path = os.path.dirname(sys.argv[0])
bin_path = os.path.join(full_path, 'bin')
print ('sys.argv[0]', full_path)

try:
    if not os.path.exists(bin_path) :
        os.mkdir(bin_path)
    if not os.path.isdir(bin_path) :
        print ('ERROR: (%s)  is not dir' %bin_path)
        exit(-1)
except:
    print ('ERROR: can not prepare directory bin')
    exit(-1)
print ('argc %d' %argc)
glob.glob('Test_Suite*\*')
if argc == 0 :
#{

    for d in glob.glob('Test_Suite*/*[0-9]'):
    #{
        if not os.path.isdir(str(d)): continue
        arg.append(d)
    #}
#}
print ('Compile list')
print (arg)
for ts in arg :
    try:
        os.chdir(ts)
    except:
        print ('ERROR: os.chdir(%s)' %ts)
        exit(-1)

    if 'win' in sys.platform :
        cmd_cl = 'cleanup.bat'
        cmd_mk = 'make.bat >makeout'
    else : # linux
        cmd_cl = 'make clean'
        cmd_mk = 'make >makeout'

#    print ('compile tests in:\n %s' % ts)
    print ('cleanup test %s' % ts)
#	res = os.system('pwd')
    res = os.system(cmd_cl)
    if res != 0 :
        print ('ERROR %s' % cmd_cl)

    if clean_only == 0 :    #need make
        print ('compile test %s' % ts)
        res = os.system(cmd_mk)
        if res != 0 :
            print ('ERROR %s' % cmd_mk)
    
    try:
        os.chdir(pwd)
    except:
        print ('ERROR: os.chdir(%s)' %pwd)
        exit(-1)
#}
        

exit(0)
