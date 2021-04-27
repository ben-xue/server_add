#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

../bin/perform_tool --id #regionid#.#worldid#.90.1 --pid-file=./perform_tool_#regionid#_#worldid#.pid stop