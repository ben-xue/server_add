#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/match_svr #worldname# --id #regionid#.#worldid#.64.1  --log-conf-file ../cfg/match_svr_log.xml --log-level #loglevel# --conf-file ../cfg/match_svr.xml --bus-key #msgq_shm_key# --pid-file=./match_svr_#regionid#_#worldid#.pid --daemon start