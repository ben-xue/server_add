#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/zone_conn --id 1.6.60.1 --pid-file=./zone_conn_1_6.pid  stop