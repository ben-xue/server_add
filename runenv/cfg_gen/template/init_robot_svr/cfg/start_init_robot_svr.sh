#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./up_conf.sh
../bin/init_robot_svr #worldname# --id #regionid#.#worldid#.94.1  --log-conf-file ../cfg/robot_svr_log.xml --log-level #loglevel# --conf-file ../cfg/robot_svr.xml --bus-key #msgq_shm_key# --pid-file=./robot_svr_#regionid#_#worldid#.pid --daemon start
