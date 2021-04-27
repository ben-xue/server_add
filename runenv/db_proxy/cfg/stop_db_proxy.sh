#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/db_proxy --id 1.6.30.1  --pid-file=./db_proxy_1_6.pid stop
