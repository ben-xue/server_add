#!/bin/sh

curuser=`whoami`

if [ $curuser = "root" ];then
 echo "root Operation is not allowed!!"
 exit -1
fi

cd $(dirname $0)
cd ./msgq/
./init.sh 
cd -

#### 服务的启动顺序有一定的依赖

cd ./json_logload_svr/
./start_jsonlogload_svr.sh
cd -

cd ./logd_svr/cfg/
./start_logd.sh
cd -

cd ./logload_svr/cfg/
./start_logload_svr.sh
cd -

cd ./db_proxy/cfg/
./start_db_proxy.sh
cd -

cd ./tunnel_svr/cfg/
./start_tunnel_svr.sh
cd -

cd ./zone_conn/cfg/
./start_zone_conn.sh
cd -

cd ./redis_proxy/cfg/
./start_redis_proxy.sh
cd -

cd ./world_svr/cfg/
./start_world_svr.sh
cd -

cd ./match_svr/cfg/
./start_match_svr.sh
cd -

cd ./relation_svr/cfg/
./start_relation_svr.sh
cd -

cd ./chat_svr/cfg/
./start_chat_svr.sh
cd -

cd ./manor_svr/cfg/
./start_manor_svr.sh
cd -

cd ./zone_svr/cfg/
./start_zone_svr.sh
cd -