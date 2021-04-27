#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/perform_tool --id #regionid#.#worldid#.90.1  --log-conf-file ../cfg/perform_tool_log.xml --log-level #loglevel# --conf-file ../cfg/perform_tool.xml --bus-key #msgq_shm_key# --pid-file=./perform_tool_#regionid#_#worldid#.pid --daemon start
