#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/db_proxy --id #regionid#.#worldid#.30.1  --pid-file=./db_proxy_#regionid#_#worldid#.pid stop
