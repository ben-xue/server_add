#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/zone_svr --id #regionid#.#worldid#.61.1 --pid-file=./zone_svr_#regionid#_#worldid#.pid stop