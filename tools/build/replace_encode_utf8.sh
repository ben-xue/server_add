#!/bin/sh

if [ $# -lt 1 ];then
	echo "Usage: replace_encode_utf8.sh filePath"
	exit 1
fi


sed 's/gb2312/UTF-8/g' $1 > $1_tmp
mv $1_tmp $1

sed 's/GBK/UTF-8/g' $1 > $1_tmp
mv $1_tmp $1

