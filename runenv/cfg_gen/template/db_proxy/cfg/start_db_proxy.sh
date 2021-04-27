#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cp ../../../protocol/dr/database.dr ./db_meta/

../bin/db_proxy #worldname# --id #regionid#.#worldid#.30.1  --log-conf-file ../cfg/db_proxy_log.xml --log-level #loglevel# --conf-file ../cfg/#db_proxy_cfg_file# --bus-key #msgq_shm_key# --pid-file=./db_proxy_#regionid#_#worldid#.pid  --daemon start
