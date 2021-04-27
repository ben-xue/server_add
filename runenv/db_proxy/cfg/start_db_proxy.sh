#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cp ../../../protocol/dr/database.dr ./db_meta/

../bin/db_proxy ben --id 1.6.30.1  --log-conf-file ../cfg/db_proxy_log.xml --log-level 700 --conf-file ../cfg/db_proxy.xml --bus-key 18030801 --pid-file=./db_proxy_1_6.pid  --daemon start
