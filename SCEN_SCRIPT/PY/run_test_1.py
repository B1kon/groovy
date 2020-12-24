#-*- coding: utf-8 -*-

'''
    arguments
        0 - filename to save self pid
        1 - string for os.system
'''
import sys, os, time, datetime, string, threading, re, tempfile, signal, errno

xprnt = int(sys.argv[1])
arg = sys.argv[2:]
if xprnt:print ('run_test_1 arg=',arg)
if xprnt:print ('run_test_1 arg[0]=',arg[0])
pid = os.getpid()
if xprnt:print ("run test =============== pid=",pid)
try:
    file_res = open(arg[0], 'w')
except IOError as err:
    print ("Can not prepare result file %s " % arg[0])
    print(err)
    exit(-1)    

file_res.write(str(pid)+os.linesep)
file_res.close()

cmd = ' '.join(arg[1:])
cmd1 = cmd.split(';')
cmd_A = cmd1[0]
cmd_B = cmd1[1]
if xprnt:print ('cmd1= ',cmd1)
if xprnt:print ('cmd_A= ',cmd_A)
if xprnt:print ('cmd_B= ',cmd_B)
if xprnt:print ('cmd= ',cmd)
test_res = -1
try:
    test_res = os.system(cmd_A)
#    if test_res==0: test_res = os.system(cmd_B)
    test_res = os.system(cmd_B)
except:
    print ("except os.system('arg[1:]')\n")

sys.exit(test_res)

