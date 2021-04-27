#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

if [ ! -d #oper_log_prefix# ]
then
	mkdir -p #oper_log_prefix#
fi

if [ ! -d #stat_log_prefix# ]
then
	mkdir -p #stat_log_prefix#
fi

../bin/logd_svr #worldname# --id #regionid#.#worldid#.84.1  --log-level #loglevel#  --log-conf-file ../cfg/tlogd_log.xml --conf-file ../cfg/tlogd.xml --pid-file=./logd_svr_#regionid#_#worldid#.pid --daemon start
