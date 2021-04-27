#!/bin/sh

mkdir -p proto
mkdir -p global_proto

cp ../../../protocol/dr/*.dr proto
cp ../../../resource/dr/*.dr proto
cp ../../../protocol/dr/proto_ss.dr global_proto/proto_ss.dr
