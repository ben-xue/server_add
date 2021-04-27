#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/logload_svr --id #regionid#.#worldid#.81.1 --pid-file=./logload_svr_#regionid#_#worldid#.pid stop
