#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cp ../../../protocol/dr/log.dr ./protocol/

../bin/realtime_oper_logload kgame_jhz --id 1.4.33.1  --log-conf-file ../cfg/realtime_oper_log.xml --log-level 700 --conf-file ../cfg/realtime_oper_log_svr.xml --pid-file=./realtime_oper_logload_1_4.pid  --daemon start
