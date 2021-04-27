#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/manor_svr #worldname# --id #regionid#.#worldid#.92.1  --log-conf-file ../cfg/manor_svr_log.xml --log-level #loglevel# --conf-file ../cfg/manor_svr.xml --bus-key #msgq_shm_key# --pid-file=./manor_svr_#regionid#_#worldid#.pid --daemon start