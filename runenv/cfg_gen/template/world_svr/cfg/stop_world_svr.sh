#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/world_svr --id #regionid#.#worldid#.62.1 --pid-file=./world_svr_#regionid#_#worldid#.pid stop