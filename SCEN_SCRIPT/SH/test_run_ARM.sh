#!/bin/bash

################################################################################
# The script run tests
# Options:
#	-o OUTPUTFILE - Redirect test output to a file.
#	   (Default: <BASE>/out_run.`date +%y%m%d%H%M`)
#	-d include extend print
#	   (Default: exclude).
#Arguments: paths on scen file. Run command in context directory SCEN_SCRIPT
#		For example:
#			cd ../bin/TS006 && ./tc006_01
#			cd ../bin/TS101 && ./tc101_01 30
################################################################################

BASE=`pwd`
OUTPUTFILE=""

Usage()
{
echo "usage"
	  cat <<EOF >&1
Options:
	-o OUTPUTFILE - Redirect test output to a file.
	   (Default: <BASE>/out_run.`date +%y%m%d%H%M`)
Arguments: paths on scen file. Run command in context directory bin
		For example:
			TS006/tc006_02
			TS101/tc101_01 30
EOF

	  exit 1 
}

dbg=0

while getopts hdo: arg
do
    case $arg in
      h)  usage;;
      d)  dbg=1;;
      o)  OUTPUTFILE=$OPTARG;  shift 2;;
      *)  Usage
    esac
done
if [ -z "$OUTPUTFILE" ]; then
    OUTPUTFILE=${BASE}/out_run.`date +%y'_'%m'_'%d'_'%H'_'%M`
else
    t1="${BASE}/$OUTPUTFILE"
    OUTPUTFILE="$t1"
fi
echo "==== OUTPUTFILE=$OUTPUTFILE"
echo "#Arguments:"
echo $*

echo > $OUTPUTFILE
export result

run_scen()
{
    echo "    ====  run_scen $1"
    for SC_FILE_rs in $1
    do
        need_rs=0
        while read lne_rs
        do
            if [ $dbg -eq 1 ]; then echo "line =$lne_rs"; fi

	    echo $lne_rs | grep "\-loop" >/dev/null
            if [ $? -eq 0 ]; then
                echo "ERROR "
	        return 1
            fi

            if [ $need_rs == 0 ]; then
	        echo $lne_rs | grep "#" >/dev/null
	        if [ $? -ne 0 ]; then
                    echo $lne_rs | grep "\-run" >/dev/null
                    if [ $? -eq 0 ]; then
		        need_rs=1
	            fi
	        fi
	        continue
	    else
	        need_rs=0
	        echo " ${lne_rs} >> $OUTPUTFILE"
                echo "==================================== $lne_rs"  >> $OUTPUTFILE 2>&1 3>&1
                CMD_rs="$lne_rs >> $OUTPUTFILE 2>&1 3>&1"
                if [ $dbg -eq 1 ]; then echo "CMD_rs = $CMD_rs"; fi
                (eval $CMD_rs)
	    fi
        done < $SC_FILE_rs
    done
}


for SC_FILE in $*
do
    need=0
    need_loop=0
    while read lne
    do
echo "need=$need"
	if [ $dbg -eq 1 ]; then echo "line =$lne"; fi
        if [ $need_loop -eq 1 ]; then
            need_loop=0
            run_scen $lne
	    continue
        fi
	echo $lne | grep "\-loop" >/dev/null
        if [ $? -eq 0 ]; then
            need_loop=1
	    continue
        fi

        if [ $need == 0 ]; then
echo "11111 need=$need line =$lne"
	    echo $lne | grep "#" >/dev/null
	    if [ $? -ne 0 ]; then
                echo $lne | grep "\-run" >/dev/null
                if [ $? -eq 0 ]; then
		    need=1
	        fi
	    fi
	    continue
	else
echo "22222 line =$lne"
	    need=0
	    echo " $lne >> $OUTPUTFILE"
            echo "==================================== $lne"  >> $OUTPUTFILE 2>&1 3>&1
            CMD="$lne >> $OUTPUTFILE 2>&1 3>&1"
echo "3333 CMD =$CMD"
echo "4444 "
#echo "4444 line =$lne #>> $OUTPUTFILE 2>&1 3>&1"
#echo "5555 "
#echo "CMD = $CMD"
echo "99999"
pwd
echo "RUN SCEN"
#$lne
#            if [ $dbg -eq 1 ]; then echo "CMD = $CMD"; fi
            (eval $CMD)
	fi
    done < $SC_FILE
done

pass=`grep "TC_" $OUTPUTFILE | grep " PASS"|wc -l`
fail=`grep "TC_" $OUTPUTFILE | grep " FAIL"|wc -l`
untested=`grep "TC_" $OUTPUTFILE | grep " UNTESTED"|wc -l`

echo "========================== SUMMARY RESULT =====================" >> $OUTPUTFILE
echo " PASS:      $pass" >> $OUTPUTFILE
echo " FAIL:      $fail" >> $OUTPUTFILE
echo " UNTESTED:  $untested" >> $OUTPUTFILE
echo " ALL:       $((pass + fail + untested))" >> $OUTPUTFILE

echo "SUMMARY RESULT:"
echo " PASS:      $pass"
echo " FAIL:      $fail"
echo " UNTESTED:  $untested"
echo " ALL:       $((pass + fail + untested))"

