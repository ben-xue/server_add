#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

mkdir -p ./dr/
cp ../../../protocol/dr/log.dr ./dr/
../bin/logload_svr #worldname# --id #regionid#.#worldid#.81.1 --log-conf-file ../cfg/logload_svr_log.xml  --log-level #loglevel#  --conf-file ../cfg/logload_svr.xml --pid-file=./logload_svr_#regionid#_#worldid#.pid  --daemon start
