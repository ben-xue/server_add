#!/bin/sh

if [ $# -lt 1 ];then
        echo "Usage: $0 dir"

fi

if [ ! -e $1 ];then
	mkdir $1
fi

