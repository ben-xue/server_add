#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/realtime_oper_logload kgame_jhz --id 1.4.33.1  --pid-file=./realtime_oper_logload_1_4.pid  --daemon stop
