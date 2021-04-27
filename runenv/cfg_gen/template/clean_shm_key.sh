#!/bin/sh

./tools/busmgr --conf-file ./msgq/msgq_config.xml --clear

./zone_svr/cfg/clear.sh
./war_svr/cfg/clear.sh
./world_svr/cfg/clear.sh
./relation_svr/cfg/clear.sh
./guild_svr/cfg/clear.sh
./match_svr/cfg/clear.sh
./db_proxy/cfg/clear.sh
./chat_svr/cfg/clear.sh
./manor_svr/cfg/clear.sh
./rank_svr/cfg/clear.sh