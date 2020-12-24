# -*- coding: cp1251 -*-

import sys, os, time, datetime, string, threading, re, tempfile
import signal, errno, thread, getopt, shutil

if len(sys.argv) <2 :
    print "Error Print_file: There is no path"
    exit (-1)

try:
    f = open(sys.argv[1])
except:
    print "Error Print_file: Can not %s" % (sys.argv[1])
    exit (-1)
f.seek(0)
for l in f:
    print l.strip()



