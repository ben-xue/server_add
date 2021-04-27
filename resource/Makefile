
.PHONY: all clean

DSF_DR = ../tools/base/drgen
MKDIR=../tools/build/mkdir.sh

INC_DIR=./include_new
INC_DIR_OLD=./include
SRC_DIR=./src
PROTOMETA_DIR=./dr
CS_DIR=./csharp
XML_DIR=./xml
CONF_DIR=./config

VPATH:=./:$(INC_DIR):$(PROTOMETA_DIR):$(TS_DIR):$(SRC_DIR):$(XML_DIR):$(CONF_DIR):

RES_XML= ResMacro.xml BootyConfig.xml CostBootyConfig.xml ItemConfig.xml SensitiveWordConfig.xml tunnel_proxy_desc.xml

CONFIG_XML= zone_svr_conf_desc.xml zone_conn_conf_desc.xml  db_proxy_desc.xml tunnel_svr_conf_desc.xml tunnel_proxy_desc.xml cache_svr_conf_desc.xml \
			match_svr_conf_desc.xml world_svr_conf_desc.xml relation_svr_conf_desc.xml redis_proxy_conf_desc.xml guild_svr_conf_desc.xml chat_svr_conf_desc.xml \
			lbs_svr_conf_desc.xml war_svr_conf_desc.xml bi_svr_conf_desc.xml misc_svr_conf_desc.xml perform_tool_desc.xml manor_svr_conf_desc.xml \
			logload_svr_conf_desc.xml jsonlogload_svr_conf_desc.xml robot_tool_conf_desc.xml tunnel_translate_conf_desc.xml

CONFIG_C=$(patsubst %.xml,%.c, $(CONFIG_XML))

%.c: %.xml
	$(MKDIR) $(SRC_DIR)
	$(DSF_DR) -d --gen2h -o $(SRC_DIR) $^
	$(DSF_DR) -d --gen2c -o $(SRC_DIR)/$@ $^

all:ResMeta.dr ResMeta_cli.dr $(CONFIG_C) CS_SHARP_TARGET REPLACE_CHANGE_FILE

ResMeta.dr :$(RES_XML)
	$(MKDIR) $(PROTOMETA_DIR)
	$(MKDIR) $(INC_DIR)
	$(DSF_DR) -d --gen2h -o $(INC_DIR) $^
	$(DSF_DR) -d --gen2dr -o $(PROTOMETA_DIR)/$@ $^

ResMeta_cli.dr :$(RES_XML)
	$(MKDIR) $(PROTOMETA_DIR)
	$(DSF_DR) -d --gen2dr_cli -o $(PROTOMETA_DIR)/$@ $^

CS_SHARP_TARGET: $(RES_XML)
	$(MKDIR) $(CS_DIR)
	$(DSF_DR) -d --gen2cs_cli --nopack --enable_lua -o $(CS_DIR) $^

REPLACE_CHANGE_FILE:
	chmod +x ./replace_res_change_file.sh
	./replace_res_change_file.sh

clean:
	rm -rf $(INC_DIR)
	rm -rf $(SRC_DIR)
	rm -rf $(TDR_DIR)
	rm -rf $(PROTOMETA_DIR)
	rm -rf $(TS_DIR)
	rm -rf $(INC_DIR_OLD)
	rm -rf $(CS_DIR)



