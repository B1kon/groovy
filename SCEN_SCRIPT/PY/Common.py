# -*- coding: utf-8 -*-

######### -*- coding: cp1251 -*-

import sys, os, time, datetime, string, threading, re, tempfile, signal, errno

PASS = 0
FAIL = 1
UNTESTED = 2
TIMEOUT = 3

RSLT = {PASS:'PASS'
        ,FAIL:'FAIL'
        ,UNTESTED:'UNTESTED'
        ,TIMEOUT:'TIMEOUT'
        }


# print a line and make exit(1)
def die(line):
    print (line)
    sys.exit(1)
# End die

def if_exe(fpath):
#{
    return ((os.path.exists(fpath)) and (os.access(fpath, os.X_OK)))
#}

# delete all blanks from start of the line
def del_start_blanks(line):
#{
    start_line = re.compile(r"^ |^\t")

    while not(start_line.findall(line) == []):
        tmp = line[1:]
        line = tmp
    return(line)
#}

# delete all blanks from end of the line
def del_end_blanks(line):
#{
    end_blanks = re.compile(r" $|\t$")
    
    while not(end_blanks.findall(line) == []):
        tmp = line[:-1]
        line = tmp
    return(line)
#}

# delete symbol "end line"
def del_end_line(line):
#{
##    end_line=re.compile(r"\n|\r")
##    
##    while not(end_line.findall(line) == []):
##        tmp = line[:-1]
##        line = tmp
    return(line.rstrip())
#}

11
