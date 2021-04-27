#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

#./up_conf.sh
#../bin/zone_svr ben --id 1.6.61.1  --log-conf-file ../cfg/zone_svr_log.xml --log-level 700 --conf-file ../cfg/zone_svr.xml --bus-key 18030801 --pid-file=./zone_svr_1_6.pid  start
../bin/zone_svr ben --id 1.6.61.1  --log-conf-file ../cfg/zone_svr_log.xml --log-level 700 --conf-file ../cfg/zone_svr.xml --bus-key 18030801 --pid-file=./zone_svr_1_6.pid --daemon start
