
.PHONY: all clean

DSF_DR = ../tools/base/drgen
DSF_TDR = ../tools/base/tdrgen
MKDIR=../tools/build/mkdir.sh

INC_DIR=./include_new
INC_DIR_OLD=./include
SRC_DIR=./src
PROTOMETA_DIR=./dr
CS_DIR=./csharp
XML_DIR=./xml


VPATH:=./:$(INC_DIR):$(PROTOMETA_DIR):$(XML_DIR)

PROTO_CS_XML=proto_macro.xml proto_result_comm.xml proto_sec.xml proto_player_comm.xml proto_cs_player.xml proto_cs.xml \

PROTO_SS_XML=proto_macro.xml proto_ss.xml \

PROTO_DB_XML=proto_macro.xml database.xml \

all: database.dr proto_cs.dr proto_ss.dr  CS_SHARP_TARGET REPLACE_CHANGE_FILE

proto_cs.dr:$(PROTO_CS_XML) $(TFRAME_XML)
	$(MKDIR) $(PROTOMETA_DIR)
	$(MKDIR) $(INC_DIR)
	$(DSF_DR) -d --gen2dr -o $(PROTOMETA_DIR)/$@ $^
	$(DSF_DR) -d --gen2h -o $(INC_DIR) $^

proto_ss.dr:$(PROTO_SS_XML)
	$(MKDIR) $(PROTOMETA_DIR)
	$(MKDIR) $(INC_DIR)
	$(DSF_DR) -d --gen2dr -o $(PROTOMETA_DIR)/$@ $^
	$(DSF_DR) -d --gen2h -o $(INC_DIR) $^

database.dr:$(PROTO_DB_XML)
	$(MKDIR) $(PROTOMETA_DIR)
	$(MKDIR) $(INC_DIR)
	$(DSF_DR) -d --gen2dr -o $(PROTOMETA_DIR)/$@ $^
	$(DSF_DR) -d --gen2h -o $(INC_DIR) $^


CS_SHARP_TARGET:$(PROTO_CS_XML)
	$(MKDIR) $(CS_DIR)
	$(DSF_DR) -d --gen2cs --enable_lua -o $(CS_DIR) $^
	rm $(CS_DIR)/proto_sec.cs

REPLACE_CHANGE_FILE:
	chmod +x ./replace_proto_change_file.sh
	./replace_proto_change_file.sh

clean:
	-rm -rf $(INC_DIR)
	-rm -rf $(INC_DIR_OLD)
	-rm -rf $(SRC_DIR)
	-rm -rf $(PROTOMETA_DIR)
	-rm -rf $(TS_DIR)
	-rm -rf $(CS_DIR)


