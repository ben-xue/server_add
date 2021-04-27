#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/tunnel_svr --id #regionid#.#worldid#.77.1 --pid-file=./tunnel_svr_#regionid#_#worldid#.pid stop
