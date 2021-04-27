#!/bin/sh

if [ $# -lt 1 ];then
	echo "Usage: conv_gbk_utf8.sh filePath"
	exit 1
fi


iconv -f GB2312 -t UTF-8 $1 > $1_tmp
mv $1_tmp $1


