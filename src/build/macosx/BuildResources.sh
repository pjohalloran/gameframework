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
    echo $0 "DATA_DIR OUTPUT_FILE [-hv]"
}

ParseArgs()
{
    # Check input
    if [ $# -ne 2 ]; then
        echo "Invalid arguments ($#)"
        Usage
        return $FALSE
    fi

    DATA_DIR=$1
    OUTPUT_FILE=$2

    if [ -e $DATA_DIR/ExcludeList.txt ]; then
        EXCLUDE_FILE=$DATA_DIR/ExcludeList.txt
        echo "EXCLUDE_FILE = " $EXCLUDE_FILE
    fi
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

    oldDir=`pwd`
    cd $DATA_DIR
    echo `pwd`

    if [ -e $EXCLUDE_FILE ]; then
        excludeList="-x "
        for line in $(cat $EXCLUDE_FILE); do
            echo $line
            excludeList="$excludeList $line"
        done

        zip $excludeList -r $OUTPUT_FILE *
    else
        zip -r $OUTPUT_FILE *
    fi

    #if [ $? -eq $TRUE ]; then
    #    cp $OUTPUT_FILE 
    #fi

    cd $oldDir

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
