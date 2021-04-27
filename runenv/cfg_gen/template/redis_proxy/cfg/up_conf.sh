#!/bin/sh

mkdir -p proto
mkdir -p global_proto

cp ../../../protocol/dr/*.dr proto
cp ../../../resource/dr/*.dr proto
cp ../../../protocol/dr/proto_ss.dr global_proto/proto_ss.dr


mkdir -p ./db_meta
cp ../../../protocol/dr/database.dr ./db_meta/

mkdir -p ./res/
cp ../../../runenv/cfg/res/*  ./res/