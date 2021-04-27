#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/zone_conn --id #regionid#.#worldid#.60.1 --pid-file=./zone_conn_#regionid#_#worldid#.pid  stop