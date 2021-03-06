#!/bin/sh
#区服端口相关配置

SHM_KEY_INCREASE_NUM=100
SERVER_PORT_INCREASE_NUM=100

worldname=kgame_jhz
regionid=1
worldid=4

#监听端口规则
#第一位大区ID 第二位分组类型ID 第三四组ID 第5位组内ID

msgq_shm_key=$[18030701+$SHM_KEY_INCREASE_NUM]
msgq_buff_size=65536

#用户
USER=$(whoami)

#项目根目录名
SERVER_NAME=kgame_server

#工作路径
proj_dir=/home/$USER/$SERVER_NAME
bin_dir=/home/$USER/$SERVER_NAME/runenv
logpathprefix=/home/$USER/$SERVER_NAME/log
loglevel=700

#日志路径配置
oper_log_prefix=/home/$USER/$SERVER_NAME/log/oper_log
stat_log_prefix=/home/$USER/$SERVER_NAME/log/stat_log
oper_json_log_prefix=/home/$USER/$SERVER_NAME/log/oper_json_log

#获取本机ip
LOCAL_IP_ADDR=$(ip addr | awk '/^[0-9]+: / {}; /inet.*global/ {print gensub(/(.*)\/(.*)/, "\\1", "g", $2)}' | head -n 1 )

#zone_conn配置
zone_outnet_conn_ip=$LOCAL_IP_ADDR
zone_conn_ip=$LOCAL_IP_ADDR
zone_conn_port=$[4702+$SERVER_PORT_INCREASE_NUM]

#全局配置
use_dsf_proto=1
use_dsf_res=1

#日志服DB配置
logdb_ip=127.0.0.1
logdb_port=3306
logdb_user=root
logdb_pswd=
splitelogtmppath=/tmp

log_svr_ip=$LOCAL_IP_ADDR
log_oper_port=24011
log_stat_port=24012
json_log_svr_ip=$LOCAL_IP_ADDR
json_log_svr_port=24013
oper_logload_svr_ip=$LOCAL_IP_ADDR
oper_logload_svr_port=24014
realtime_oper_logload_process_count=1

#zone_svr配置
zone_udp_host=$LOCAL_IP_ADDR
zone_udp_port=$[1000+$SERVER_PORT_INCREASE_NUM]
zone_svr_shm_key=$[18030702+$SHM_KEY_INCREASE_NUM]
zone_svr_shm_size=3547559020
zone_svr_player_pool_count=500
zone_svr_cache_pool_count=100
zone_svr_battle_pool_count=1000
zone_svr_max_single_battle_count=2000
zone_svr_max_client_seq_obj_count=1000
block_msg_serialize=0
block_client_gm_cmd=0
zone_svr_tunnel_ip=$LOCAL_IP_ADDR
zone_svr_tunnel_port=$[4103+$SERVER_PORT_INCREASE_NUM]
svr_status=1
zone_battle_thread_cnt=2
zone_svr_max_mail_count=3000
zone_svr_channel_id=1
zone_svr_cdkey_open=1

zone_svr_open_time=`date '+%s'`

zone_svr_max_world_node_count=15000;
zone_svr_max_world_cnt = 1000;
zone_svr_max_legion_cnt=100;
zone_svr_max_worldchat_cache_cnt=500;
zone_svr_max_aoi_key_cnt=48400
zone_svr_max_aoi_player_cnt=100

#db_proxy配置
db_proxy_shm_key=$[18030703+$SHM_KEY_INCREASE_NUM]
db_proxy_host=127.0.0.1
db_proxy_user=root
db_proxy_pwd=
db_proxy_port=3306
db_proxy_index=1
db_proxy_cfg_file=db_proxy.xml
db_proxy_thread_num=2
db_proxy_thread_queue_key_1=$[0x20191201+$SHM_KEY_INCREASE_NUM]
db_proxy_thread_queue_key_2=$[0x20191202+$SHM_KEY_INCREASE_NUM]

# db_proxy global 数据库配置
db_global_proxy_host=127.0.0.1
db_global_proxy_port=3306
db_global_proxy_user=root
db_global_proxy_pwd=

#tunnel
tunnel_cnt=1
tunnel_local_ip=$LOCAL_IP_ADDR
tunnel_local_port=$[4102+$SERVER_PORT_INCREASE_NUM]
tunnel_local_ip_2=$LOCAL_IP_ADDR
tunnel_local_port_2=$[14103+$SERVER_PORT_INCREASE_NUM]
tunnel_verfity_magic=0421
center_redis_region_id=$regionid
center_redis_host=127.0.0.1
center_redis_port=6379
center_redis_db_no=1
center_redis_authpass=

#匹配服务器
match_svr_shm_key=$[18030705+$SHM_KEY_INCREASE_NUM]
match_svr_shm_size=200000000
match_svr_tunnel_ip=$LOCAL_IP_ADDR
match_svr_tunnel_port=$[4104+$SERVER_PORT_INCREASE_NUM]
match_player_count=2000
match_SearchList_count=2000
match_SearchNode_count=1000
match_svr_war_size=5
match_redis_host=127.0.0.1
match_redis_port=6379
match_redis_db_no=2
match_redis_authpass=
max_war_svr_size=5

#world_svr配置
world_svr_shm_key=$[18030707+$SHM_KEY_INCREASE_NUM]
world_svr_tunnel_ip=$LOCAL_IP_ADDR
world_svr_tunnel_port=$[4107+$SERVER_PORT_INCREASE_NUM]
world_svr_php_api_host=$LOCAL_IP_ADDR
world_svr_php_api_port=$[24107+$SERVER_PORT_INCREASE_NUM]
world_svr_udp_php_api_host=$LOCAL_IP_ADDR
world_svr_udp_php_api_port=$[4108+$SERVER_PORT_INCREASE_NUM]
world_group_num=1
world_svr_shm_size=100000000
world_svr_online_info_pool_size=3000
world_svr_queue_len=50
world_svr_finished_queue_len=50
world_max_queue_rank_num=100
world_max_oss_mail_pool_num=100
world_svr_alloc_speed_per_sec=100
world_svr_reserved_player_num=10
world_svr_client_queue_tick_interval=10
world_svr_max_player_num_in_allworld=100
redis_db_host=127.0.0.1
redis_db_port=6379
player_info_redis_no=3
redis_db_passwd=
forbid_chat_list_count=5000

#relation_svr配置
relation_svr_shm_key=$[18070708+$SHM_KEY_INCREASE_NUM]
relation_svr_tunnel_ip=$LOCAL_IP_ADDR
relation_svr_tunnel_port=$[4142+$SERVER_PORT_INCREASE_NUM]
relation_svr_shm_size=350000000
max_relation_player_count=1000
max_relation_cache_player_count=3000

#redis_proxy
redis_proxy_svr_shm_key=$[18070709+$SHM_KEY_INCREASE_NUM]
redis_proxy_tunnel_ip=$LOCAL_IP_ADDR
redis_proxy_tunnel_port=$[8442+$SERVER_PORT_INCREASE_NUM]
redis_proxy_redis_db_no=4

#manor svr 配置
manor_svr_shm_key=$[18070712+$SHM_KEY_INCREASE_NUM]
manor_svr_shm_size=150000000
manor_player_count=3000
manor_server_id=0
manor_instance_cnt=4
manor_query_per_select=10

manor_redis_host=127.0.0.1
manor_redis_port=6379
manor_redis_db_no=11
manor_redis_authpass=
manor_svr_tunnel_ip=$LOCAL_IP_ADDR
manor_svr_tunnel_port=$[4145+$SERVER_PORT_INCREASE_NUM]

#moderation svr AI验证代理服
moderation_svr_udp_host=$LOCAL_IP_ADDR
moderation_svr_udp_port=10002

echo $proj_dir

rm -rf ./gen
cd template/ && find . -type d -exec mkdir -p ../gen/'{}' ';' && cd ..
file_list=$(cd ./template/ && find -mindepth 1 -type f)
echo $file_list
cd ./template
for file in $file_list;
do
sed "s/#worldname#/$worldname/g" $file | \
sed "s/#worldid#/$worldid/g" | \
sed "s/#regionid#/$regionid/g" | \
sed "s/#msgq_shm_key#/$msgq_shm_key/g" | \
sed "s/#msgq_buff_size#/$msgq_buff_size/g" | \
sed "s/#loglevel#/$loglevel/g" | \
sed "s:#proj_dir#:$proj_dir:g" | \
sed "s:#bin_dir#:$bin_dir:g" | \
sed "s:#logpathprefix#:$logpathprefix:g" | \
sed "s/#use_dsf_proto#/$use_dsf_proto/g" | \
sed "s/#use_dsf_res#/$use_dsf_res/g" | \
sed "s/#db_proxy_shm_key#/$db_proxy_shm_key/g" | \
sed "s/#db_proxy_host#/$db_proxy_host/g" | \
sed "s/#db_proxy_user#/$db_proxy_user/g" | \
sed "s/#db_proxy_pwd#/$db_proxy_pwd/g" | \
sed "s/#db_proxy_port#/$db_proxy_port/g" | \
sed "s/#db_global_proxy_host#/$db_global_proxy_host/g" | \
sed "s/#db_global_proxy_port#/$db_global_proxy_port/g" | \
sed "s/#db_global_proxy_user#/$db_global_proxy_user/g" | \
sed "s/#db_global_proxy_pwd#/$db_global_proxy_pwd/g" | \
sed "s/#zone_udp_host#/$zone_udp_host/g" | \
sed "s/#zone_udp_port#/$zone_udp_port/g" | \
sed "s/#zone_svr_shm_key#/$zone_svr_shm_key/g" | \
sed "s/#zone_svr_shm_size#/$zone_svr_shm_size/g" | \
sed "s/#zone_svr_player_pool_count#/$zone_svr_player_pool_count/g" | \
sed "s/#zone_svr_cache_pool_count#/$zone_svr_cache_pool_count/g" | \
sed "s/#zone_svr_battle_pool_count#/$zone_svr_battle_pool_count/g" | \
sed "s/#zone_svr_max_single_battle_count#/$zone_svr_max_single_battle_count/g" | \
sed "s/#zone_svr_max_client_seq_obj_count#/$zone_svr_max_client_seq_obj_count/g" | \
sed "s/#block_client_gm_cmd#/$block_client_gm_cmd/g" | \
sed "s/#block_msg_serialize#/$block_msg_serialize/g" | \
sed "s/#zone_svr_tunnel_ip#/$zone_svr_tunnel_ip/g" | \
sed "s/#zone_svr_tunnel_port#/$zone_svr_tunnel_port/g" | \
sed "s/#svr_status#/$svr_status/g" | \
sed "s/#zone_svr_max_friend_mail_cache_count#/$zone_svr_max_friend_mail_cache_count/g" | \
sed "s/#zone_svr_max_sys_mail_cache_count#/$zone_svr_max_sys_mail_cache_count/g" | \
sed "s/#zone_svr_max_legion_mail_cache_count#/$zone_svr_max_legion_mail_cache_count/g" | \
sed "s/#zone_svr_max_war_mail_cache_count#/$zone_svr_max_war_mail_cache_count/g" | \
sed "s/#zone_svr_channel_id#/$zone_svr_channel_id/g" | \
sed "s/#zone_svr_cdkey_open#/$zone_svr_cdkey_open/g" | \
sed "s/#zone_svr_open_time#/$zone_svr_open_time/g" | \

sed "s/#zone_svr_max_world_node_count#/$zone_svr_max_world_node_count/g" | \
sed "s/#zone_svr_max_world_cnt#/$zone_svr_max_world_cnt/g" | \
sed "s/#zone_svr_max_legion_cnt#/$zone_svr_max_legion_cnt/g" | \
sed "s/#zone_svr_max_worldchat_cache_cnt#/$zone_svr_max_worldchat_cache_cnt/g" | \

sed "s/#zone_battle_thread_cnt#/$zone_battle_thread_cnt/g" | \
sed "s/#zone_svr_max_mail_count#/$zone_svr_max_mail_count/g" | \
sed "s/#zone_svr_max_aoi_key_cnt#/$zone_svr_max_aoi_key_cnt/g" | \
sed "s/#zone_svr_max_aoi_player_cnt#/$zone_svr_max_aoi_player_cnt/g" | \

sed "s/#match_svr_shm_key#/$match_svr_shm_key/g" | \
sed "s/#match_svr_shm_size#/$match_svr_shm_size/g" | \
sed "s/#match_svr_tunnel_ip#/$match_svr_tunnel_ip/g" | \
sed "s/#match_svr_tunnel_port#/$match_svr_tunnel_port/g" | \
sed "s/#match_player_count#/$match_player_count/g" | \
sed "s/#match_SearchList_count#/$match_SearchList_count/g" | \
sed "s/#match_SearchNode_count#/$match_SearchNode_count/g" | \
sed "s/#match_svr_war_size#/$match_svr_war_size/g" | \
sed "s/#match_redis_host#/$match_redis_host/g" | \
sed "s/#match_redis_port#/$match_redis_port/g" | \
sed "s/#match_redis_db_no#/$match_redis_db_no/g" | \
sed "s/#match_redis_authpass#/$match_redis_authpass/g" | \
sed "s/#world_svr_php_api_host#/$world_svr_php_api_host/g" | \
sed "s/#world_svr_php_api_port#/$world_svr_php_api_port/g" | \
sed "s/#world_svr_udp_textmsg_host#/$world_svr_udp_php_api_host/g" | \
sed "s/#world_svr_udp_textmsg_port#/$world_svr_udp_php_api_port/g" | \
sed "s/#world_svr_client_queue_tick_interval#/$world_svr_client_queue_tick_interval/g" | \
sed "s/#world_svr_max_player_num_in_allworld#/$world_svr_max_player_num_in_allworld/g" | \
sed "s/#world_group_num#/$world_group_num/g" | \
sed "s/#world_svr_shm_key#/$world_svr_shm_key/g" | \
sed "s/#world_svr_shm_size#/$world_svr_shm_size/g" | \
sed "s/#world_svr_tunnel_ip#/$world_svr_tunnel_ip/g" | \
sed "s/#world_svr_tunnel_port#/$world_svr_tunnel_port/g" | \
sed "s/#world_svr_online_info_pool_size#/$world_svr_online_info_pool_size/g" | \
sed "s/#world_svr_queue_len#/$world_svr_queue_len/g" | \
sed "s/#world_svr_finished_queue_len#/$world_svr_finished_queue_len/g" | \
sed "s/#world_max_queue_rank_num#/$world_max_queue_rank_num/g" | \
sed "s/#world_max_oss_mail_pool_num#/$world_max_oss_mail_pool_num/g" | \
sed "s/#world_svr_alloc_speed_per_sec#/$world_svr_alloc_speed_per_sec/g" | \
sed "s/#world_svr_reserved_player_num#/$world_svr_reserved_player_num/g" | \
sed "s/#redis_db_host#/$redis_db_host/g" | \
sed "s/#redis_db_port#/$redis_db_port/g" | \
sed "s/#player_info_redis_no#/$player_info_redis_no/g" | \
sed "s/#redis_db_passwd#/$redis_db_passwd/g" | \
sed "s/#forbid_chat_list_count#/$forbid_chat_list_count/g" | \
sed "s/#relation_svr_shm_key#/$relation_svr_shm_key/g" | \
sed "s/#relation_svr_tunnel_port#/$relation_svr_tunnel_port/g" | \
sed "s/#relation_svr_tunnel_ip#/$relation_svr_tunnel_ip/g" | \
sed "s/#relation_svr_shm_size#/$relation_svr_shm_size/g" | \
sed "s/#max_relation_player_count#/$max_relation_player_count/g" | \
sed "s/#max_relation_cache_player_count#/$max_relation_cache_player_count/g" | \

sed "s/#lbs_redis_db_host#/$lbs_redis_db_host/g" |\
sed "s/#lbs_redis_db_port#/$lbs_redis_db_port/g" |\
sed "s/#lbs_svr_redis_no#/$lbs_svr_redis_no/g" |\
sed "s/#lbs_svr_tunnel_ip#/$lbs_svr_tunnel_ip/g" |\
sed "s/#lbs_redis_db_passwd#/$lbs_redis_db_passwd/g" |\
sed "s/#lbs_svr_tunnel_port#/$lbs_svr_tunnel_port/g" |\

sed "s/#war_svr_shm_key#/$war_svr_shm_key/g" | \
sed "s/#war_svr_tunnel_ip#/$war_svr_tunnel_ip/g" | \
sed "s/#war_svr_tunnel_port#/$war_svr_tunnel_port/g" | \
sed "s/#war_svr_shm_size#/$war_svr_shm_size/g" | \
sed "s/#war_svr_online_pool_size#/$war_svr_online_pool_size/g" | \
sed "s/#war_svr_war_legion_size#/$war_svr_war_legion_size/g" | \
sed "s/#war_svr_battle_pool_count#/$war_svr_battle_pool_count/g" | \
sed "s/#war_svr_war_size#/$war_svr_war_size/g" | \

sed "s/#war_battle_thread_cnt#/$war_battle_thread_cnt/g" | \

sed "s/#zone_conn_ip#/$zone_conn_ip/g" | \
sed "s/#zone_outnet_conn_ip#/$zone_outnet_conn_ip/g" | \
sed "s/#zone_conn_port#/$zone_conn_port/g" | \
sed "s/#zone_conn_tunnel_ip#/$zone_conn_tunnel_ip/g" | \
sed "s/#zone_conn_tunnel_port#/$zone_conn_tunnel_port/g" | \
sed "s/#db_global_proxy_shm_key#/$db_global_proxy_shm_key/g" | \
sed "s/#db_global_proxy_host#/$db_global_proxy_host/g" | \
sed "s/#db_global_proxy_user#/$db_global_proxy_user/g" | \
sed "s/#db_global_proxy_pwd#/$db_global_proxy_pwd/g" | \
sed "s/#db_global_proxy_port#/$db_global_proxy_port/g" | \
sed "s/#db_cache_shm_key#/$db_cache_shm_key/g" |\
sed "s/#db_cache_proxy_host#/$db_cache_proxy_host/g" |\
sed "s/#db_cache_proxy_user#/$db_cache_proxy_user/g" |\
sed "s/#db_cache_proxy_pwd#/$db_cache_proxy_pwd/g" |\
sed "s/#db_cache_proxy_port#/$db_cache_proxy_port/g" |\

sed "s/#lbs_redis_db_host#/$lbs_redis_db_host/g" |\
sed "s/#lbs_redis_db_port#/$lbs_redis_db_port/g" |\
sed "s/#lbs_svr_redis_no#/$lbs_svr_redis_no/g" |\
sed "s/#lbs_svr_tunnel_ip#/$lbs_svr_tunnel_ip/g" |\
sed "s/#lbs_redis_db_passwd#/$lbs_redis_db_passwd/g" |\
sed "s/#lbs_svr_tunnel_port#/$lbs_svr_tunnel_port/g" |\

sed "s/#guild_svr_shm_key#/$guild_svr_shm_key/g" |\
sed "s/#guild_svr_shm_size#/$guild_svr_shm_size/g" |\
sed "s/#guild_svr_max_guild_count#/$guild_svr_max_guild_count/g" |\
sed "s/#guild_svr_max_player_count#/$guild_svr_max_player_count/g" |\
sed "s/#guild_svr_tunnel_ip#/$guild_svr_tunnel_ip/g" |\
sed "s/#guild_svr_tunnel_port#/$guild_svr_tunnel_port/g" |\

sed "s/#db_proxy_cfg_file#/$db_proxy_cfg_file/g" | \
sed "s/#db_proxy_thread_num#/$db_proxy_thread_num/g" | \
sed "s/#db_proxy_thread_queue_key_1#/$db_proxy_thread_queue_key_1/g" | \
sed "s/#db_proxy_thread_queue_key_2#/$db_proxy_thread_queue_key_2/g" | \

sed "s/#misc_svr_ip#/$misc_svr_ip/g" | \
sed "s/#misc_svr_port#/$misc_svr_port/g" | \
sed "s/#udp_text_msg_host#/$udp_text_msg_host/g" | \
sed "s/#udp_text_msg_port#/$udp_text_msg_port/g" | \

sed "s/#manor_svr_shm_key#/$manor_svr_shm_key/g" | \
sed "s/#manor_svr_shm_size#/$manor_svr_shm_size/g" | \
sed "s/#manor_player_count#/$manor_player_count/g" | \
sed "s/#manor_server_id#/$manor_server_id/g" | \
sed "s/#manor_instance_cnt#/$manor_instance_cnt/g" | \
sed "s/#manor_query_per_select#/$manor_query_per_select/g" | \
sed "s/#manor_redis_host#/$manor_redis_host/g" | \
sed "s/#manor_redis_port#/$manor_redis_port/g" | \
sed "s/#manor_redis_db_no#/$manor_redis_db_no/g" | \
sed "s/#manor_redis_authpass#/$manor_redis_authpass/g" | \
sed "s/#manor_svr_tunnel_ip#/$manor_svr_tunnel_ip/g" | \
sed "s/#manor_svr_tunnel_port#/$manor_svr_tunnel_port/g" | \

sed "s/#moderation_svr_udp_host#/$moderation_svr_udp_host/g" | \
sed "s/#moderation_svr_udp_port#/$moderation_svr_udp_port/g" | \

sed "s/#tunnel_cnt#/$tunnel_cnt/g" | \
sed "s/#tunnel_local_ip#/$tunnel_local_ip/g" | \
sed "s/#tunnel_local_port#/$tunnel_local_port/g" | \
sed "s/#tunnel_local_ip_2#/$tunnel_local_ip_2/g" | \
sed "s/#tunnel_local_port_2#/$tunnel_local_port_2/g" | \
sed "s/#tunnel_verfity_magic#/$tunnel_verfity_magic/g" | \
sed "s/#center_redis_region_id#/$center_redis_region_id/g" | \
sed "s/#center_redis_host#/$center_redis_host/g" | \
sed "s/#center_redis_port#/$center_redis_port/g" | \
sed "s/#center_redis_db_no#/$center_redis_db_no/g" | \
sed "s/#center_redis_authpass#/$center_redis_authpass/g" | \
sed "s/#redis_proxy_svr_shm_key#/$redis_proxy_svr_shm_key/g" | \
sed "s/#redis_proxy_tunnel_ip#/$redis_proxy_tunnel_ip/g" | \
sed "s/#redis_proxy_tunnel_port#/$redis_proxy_tunnel_port/g" | \
sed "s/#redis_proxy_redis_db_no#/$redis_proxy_redis_db_no/g" | \

sed "s!#oper_log_prefix#!$oper_log_prefix!g" | \
sed "s!#stat_log_prefix#!$stat_log_prefix!g" | \
sed "s!#oper_json_log_prefix#!$oper_json_log_prefix!g" | \
sed "s!#logdb_ip#!$logdb_ip!g" | \
sed "s!#logdb_port#!$logdb_port!g" | \
sed "s!#logdb_user#!$logdb_user!g" | \
sed "s!#logdb_pswd#!$logdb_pswd!g" | \
sed "s!#splitelogtmppath#!$splitelogtmppath!g" | \
sed "s!#log_oper_port#!$log_oper_port!g" | \
sed "s!#log_oper_json_port#!$log_oper_json_port!g" | \
sed "s!#log_svr_ip#!$log_svr_ip!g" | \
sed "s!#json_log_svr_ip#!$json_log_svr_ip!g" | \
sed "s!#json_log_svr_port#!$json_log_svr_port!g" |
sed "s!#oper_logload_svr_ip#!$oper_logload_svr_ip!g" |
sed "s!#oper_logload_svr_port#!$oper_logload_svr_port!g" | \
sed "s!#realtime_oper_logload_process_count#!realtime_oper_logload_process_count!g" | \

sed "s!#log_stat_port#!$log_stat_port!g" > ../gen/$file

done

cd ..

cd ./gen
find . -name "*.sh" -exec chmod +x '{}' ';'

rm -rf script
file_list=$(find . -type f)

#追加需要额外生成的目录
zone_skillxml=' ./zone_svr/cfg/SkillXml/ '
zone_res=' ./zone_svr/cfg/res/ '
zone_rank_dir=' ./zone_svr/cfg/fs_rank_data/ '
file_list=$file_list$zone_skillxml$zone_res$zone_rank_dir


# 此时所在目录是 ./server/runenv/cfg_gen/gen
# 下面的 ../.. 是 指的 server/runenv

if [ ! -e $proj_dir/log ];then
    mkdir $proj_dir/log
fi

for file in $file_list;
do
	complete_dir_path=../../$file
	if [ ! -e ${complete_dir_path%/*} ];then
		echo 'create directory ----'${complete_dir_path%/*}
		mkdir -p ${complete_dir_path%/*}
	fi
	if [ -d $file ];then
		continue
	fi
	cp -p  $file ../../$file
	 chmod +x ../../$file
done

cd ..
rm -rf gen
