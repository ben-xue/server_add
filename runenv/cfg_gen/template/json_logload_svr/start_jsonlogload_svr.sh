#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
./json_logload_svr #worldname# --id #regionid#.#worldid#.93.1 --log-conf-file ./jsonlogload_svr_log.xml  --log-level #loglevel#  --conf-file ./jsonlogload_svr.xml --pid-file=./json_logload_svr_#regionid#_#worldid#.pid  --daemon start
