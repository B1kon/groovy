#!/bin/bash

################################################################################
# The script run tests
# Options:
#	-o OUTPUTFILE - Redirect test output to a file.
#	   (Default: <BASE>/out_run.`date +%y%m%d%H%M`)
#Arguments: paths on test suite
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
Argument: path on scenario
EOF

	  exit 1 
}

while getopts ho: arg
do
    case $arg in
      h)  usage;;
       o)  OUTPUTFILE=$OPTARG;  shift 2;;
      *)  Usage
    esac
done
if [ -z "$OUTPUTFILE" ]; then
    OUTPUTFILE=${BASE}/out_run.`date +%y'_'%m'_'%d'_'%H'_'%M`
fi
echo "==== OUTPUTFILE=$OUTPUTFILE"
echo "#Arguments:"
echo $*
#exit 1
for j in $*
do
    for i in `ls $j |grep -v abs`
    do
	echo "./$j/$i >> $OUTPUTFILE"
	echo "=============================================== ./$j/$i"  >> $OUTPUTFILE 2>&1 3>&1
	cd $j
	./$i >> ../$OUTPUTFILE 2>&1 3>&1
	cd ..
#	./$j/$i >> $OUTPUTFILE 2>&1 3>&1
    done
done
pass=`grep "TC_" $OUTPUTFILE | grep " PASS"|wc -l`
fail=`grep "TC_" $OUTPUTFILE | grep " FAIL"|wc -l`
untested=`grep "TC_" $OUTPUTFILE | grep " UnTested"|wc -l`

echo "========================== SUMMARY RESULT =====================" >> $OUTPUTFILE
echo " PASS:      $pass" >> $OUTPUTFILE
echo " FAIL:      $fail" >> $OUTPUTFILE
echo " UNTESTED:  $untested" >> $OUTPUTFILE

echo "SUMMARY RESULT:"
echo " PASS:      $pass"
echo " FAIL:      $fail"
echo " UNTESTED:  $untested"
