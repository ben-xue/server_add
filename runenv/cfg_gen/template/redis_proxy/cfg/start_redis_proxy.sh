#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh

../bin/redis_proxy #worldname# --id #regionid#.#worldid#.80.1  --log-conf-file ../cfg/redis_proxy_log.xml --log-level #loglevel# --conf-file ../cfg/redis_proxy.xml --bus-key #msgq_shm_key# --pid-file=./redis_proxy_#regionid#_#worldid#.pid  --daemon start


