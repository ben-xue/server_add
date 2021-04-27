#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi
./up_conf.sh

../bin/relation_svr  #worldname# --id #regionid#.#worldid#.63.1 --log-level #loglevel# --log-conf-file ../cfg/relation_svr_log.xml --conf-file  ../cfg/relation_svr.xml --bus-key #msgq_shm_key#  --pid-file=./relation_svr_#regionid#_#worldid#.pid --daemon  start
