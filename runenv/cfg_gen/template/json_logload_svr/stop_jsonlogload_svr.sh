#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

./json_logload_svr --id #regionid#.#worldid#.93.1 --pid-file=./json_logload_svr_#regionid#_#worldid#.pid stop







