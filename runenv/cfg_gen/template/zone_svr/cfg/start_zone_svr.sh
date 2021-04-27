#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/zone_svr #worldname# --id #regionid#.#worldid#.61.1  --log-conf-file ../cfg/zone_svr_log.xml --log-level #loglevel# --conf-file ../cfg/zone_svr.xml --bus-key #msgq_shm_key# --pid-file=./zone_svr_#regionid#_#worldid#.pid --daemon start
