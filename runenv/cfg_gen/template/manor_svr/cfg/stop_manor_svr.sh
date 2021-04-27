#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/manor_svr --id #regionid#.#worldid#.92.1 --pid-file=./manor_svr_#regionid#_#worldid#.pid stop