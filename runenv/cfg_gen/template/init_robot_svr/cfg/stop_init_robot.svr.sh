#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/init_robot_svr --id #regionid#_#worldid#.94.1 --pid-file=./robot_svr_#regionid#_#worldid#.pid stop

