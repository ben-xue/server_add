#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/world_svr #worldname# --id #regionid#.#worldid#.62.1  --log-conf-file ../cfg/world_svr_log.xml --log-level #loglevel# --conf-file ../cfg/world_svr.xml --bus-key #msgq_shm_key# --pid-file=./world_svr_#regionid#_#worldid#.pid --daemon start