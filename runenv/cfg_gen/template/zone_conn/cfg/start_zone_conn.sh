#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cp ../../../protocol/dr/proto_cs.dr ./proto/

../bin/zone_conn #worldname# --id #regionid#.#worldid#.60.1 --log-conf-file ../cfg/zone_conn_log.xml  --log-level #loglevel# --conf-file ../cfg/zone_conn.xml  --bus-key #msgq_shm_key# --pid-file=./zone_conn_#regionid#_#worldid#.pid --daemon start
