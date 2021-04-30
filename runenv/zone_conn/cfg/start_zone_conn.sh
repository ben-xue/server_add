#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cp ../../../protocol/dr/proto_cs.dr ./proto/

../bin/zone_conn kgame_ben --id 1.6.60.1 --log-conf-file ../cfg/zone_conn_log.xml  --log-level 700 --conf-file ../cfg/zone_conn.xml  --bus-key 18930701 --pid-file=./zone_conn_1_6.pid --daemon start
