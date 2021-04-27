#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

#在runenv 下
SERVER_DIR=`cd ../ && pwd`

REGION_ID=#regionid#
WORLD_ID=#worldid#

function CmdHelp()
{
  echo "-----------重启服务------------------"
  echo "|1. 冷启所有服务                     |"
  echo "|2. 热启所有服务                     |"
  echo "|3. 清档重启服务                     |"
  echo "|4. 退出                            |"
  echo "|------------------------------------|"
}

function ColdStartAllServeices()
{
	cd $SERVER_DIR/runenv/
	./stop_all.sh
	./clean_shm_key.sh
	./start_all.sh
}

function HotStartAllServeices()
{
	cd $SERVER_DIR/runenv/
	./start_all.sh
}

function ClearDataBase()
{
	cd $SERVER_DIR/runenv/
	./stop_all.sh
	./clean_shm_key.sh

	cd $SERVER_DIR/db/script 
	chmod +x ./*
	./drop_roledb.sh $REGION_ID $WORLD_ID
	./create_dbtab_sql.sh
	./create_roledb.sh $REGION_ID $WORLD_ID

    ./drop_global_db.sh $REGION_ID
    ./create_global_db.sh $REGION_ID

	cd $SERVER_DIR 
	./clear_rank.sh	

	echo FLUSHALL|redis-cli -p #center_redis_port#
	cd $SERVER_DIR/log

	redis-cli -p #center_redis_port# flushall

#	cd $SERVER_DIR/runenv
#	redis-cli --eval ./clear_redis_match.lua
}

function StartRobotInsertDb()
{
    cd $SERVER_DIR/runenv/init_robot_svr/cfg
    ./stop_init_robot.svr.sh
    ./start_init_robot_svr.sh
    sleep 20
}


function main()
{
	CmdHelp
	
	read -p "请输入选择:" choice
	echo -e "\n"
	
	case $choice in 
	1)
		ColdStartAllServeices
	;;
	2)
		HotStartAllServeices
	;;
	3)
		ClearDataBase
		ColdStartAllServeices
		StartRobotInsertDb
	;;
	4)
		exit;;
	*)
		echo -e "请输入正确选项\n"
	esac
}

main





