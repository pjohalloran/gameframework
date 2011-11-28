#!/bin/sh

#
# CopyDataList.sh
# PJ O Halloran
# 02/10/2011
#
# Copy the files listed in config file into the directory
#

EXCLUDE_FILE=""
DATA_DIR=""
OUTPUT_FILE=""

TRUE=0
FALSE=1

VERBOSE=FALSE

Usage()
{
    echo $0 "Help Data:"
    echo $0 "EXCLUDE_FILE OUTPUT_FILE DATA_DIR [-hv]"
}

ParseArgs()
{
    # Check input
    if [ $# -ne 3 ]; then
        echo "Invalid arguments ($#)"
        Usage
        return $FALSE
    fi

    EXCLUDE_FILE=$1
    DATA_DIR=$2
    OUTPUT_FILE=$3

    echo "EXCLUDE_FILE = " $EXCLUDE_FILE
    echo "DATA_DIR = " $DATA_DIR
    echo "OUTPUT_FILE = " $OUTPUT_FILE

    return $TRUE
}

BuildResourceFile()
{
    if [ -e $OUTPUT_FILE ]; then
        echo "Removing old data file."
        rm $OUTPUT_FILE
    fi

    cd $DATA_DIR
    echo `pwd`

    excludeList="-x "
    for line in $(cat $EXCLUDE_FILE); do
        echo $line
        excludeList="$excludeList $line"
    done

    if [ ${#excludeList} -gt 3 ]; then
        echo "Excluding some files... (zip $excludeList -r $OUTPUT_FILE $DATA_DIR)"
        zip $excludeList -r $OUTPUT_FILE *
    else
        echo "Building zip... (zip -r $OUTPUT_FILE $DATA_DIR)"
        zip -r $OUTPUT_FILE *
    fi

#    if [ $? -eq $TRUE ]; then
#        cp $OUTPUT_FILE 
#    fi

    return $?
}

main()
{
    ParseArgs $*
    if [ $? -ne $TRUE ]; then
        return $?
    fi

    BuildResourceFile
    return $?
}

main $*
if [ $? -ne $TRUE ]; then
    echo $0 .. " failed to complete."
fi
