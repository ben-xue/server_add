#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/relation_svr  --id #regionid#.#worldid#.63.1  --pid-file=./relation_svr_#regionid#_#worldid#.pid stop
