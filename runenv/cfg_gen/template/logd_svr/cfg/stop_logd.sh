#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/logd_svr --id #regionid#.#worldid#.84.1  --pid-file=./logd_svr_#regionid#_#worldid#.pid stop
