#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cd $(dirname $0)

cd ./zone_conn/cfg/
./stop_zone_conn.sh

cd ../../zone_svr/cfg/
./stop_zone_svr.sh

cd ../../match_svr/cfg/
./stop_match_svr.sh

cd ../../world_svr/cfg/
./stop_world_svr.sh

cd ../../relation_svr/cfg/
./stop_relation_svr.sh

cd ../../chat_svr/cfg/
./stop_chat_svr.sh

cd ../../redis_proxy/cfg/
./stop_redis_proxy.sh

cd ../../manor_svr/cfg/
./stop_manor_svr.sh
cd -

sleep 20

cd ../../db_proxy/cfg/
./stop_db_proxy.sh

cd ../../tunnel_svr/cfg/
./stop_tunnel_svr.sh

cd ../../logd_svr/cfg/
./stop_logd.sh

cd ../../logload_svr/cfg/
./stop_logload_svr.sh

cd ../../json_logload_svr
./stop_jsonlogload_svr.sh