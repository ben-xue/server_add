#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/match_svr --id #regionid#.#worldid#.64.1 --pid-file=./match_svr_#regionid#_#worldid#.pid stop