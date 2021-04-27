#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi


../bin/redis_proxy --id #regionid#.#worldid#.80.1 --pid-file=./redis_proxy_#regionid#_#worldid#.pid stop

