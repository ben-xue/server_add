#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/zone_svr --id 1.6.61.1 --pid-file=./zone_svr_1_6.pid stop