#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh

../bin/tunnel_svr #worldname# --id #regionid#.#worldid#.77.1  --log-conf-file ../cfg/tunnel_svr_log.xml --log-level #loglevel# --conf-file ../cfg/tunnel_svr.xml --bus-key #msgq_shm_key# --pid-file=./tunnel_svr_#regionid#_#worldid#.pid  --daemon start
