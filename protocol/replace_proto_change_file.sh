#!/bin/bash

MD5_FILE="md5_list"
INCLUDE_OLD=./include
INCLUDE_NEW=./include_new

mkdir -p ${INCLUDE_OLD}

#rm ${INCLUDE_OLD}/${MD5_FILE}
#rm ${INCLUDE_NEW}/${MD5_FILE}


function GenNewIncludeMd5()
{
	for file in ${INCLUDE_NEW}
	do 
		md5sum $file >> ${MD5_FILE}
	done
}

function GenOldIncludeMd5()
{
	for file in ${INCLUDE_OLD}
	do 
		md5sum $file >> ${MD5_FILE}
	done
}

function RepalceChangeFile()
{
	for file in ${INCLUDE_NEW}/*
	do 
		real_file=$(basename $file)
	
		if [ -z "$real_file" ] 
		then 
			continue
		fi
		
		diff $file ${INCLUDE_OLD}/$real_file > /dev/null
		if [ $? != 0 ]
		then 
			echo "different" $file "  " ${INCLUDE_OLD}/$real_file
			cp $file ${INCLUDE_OLD}/$real_file
		fi
	done
}

function main()
{
	RepalceChangeFile
}

main

