#!/bin/sh

#
# 3rdPartyInstall
# PJ O Halloran
# 03/09/2012
#
# Install all dependant 3rdParty libs using Macports.
#
# Please install and/or update to the latest version of Macports before running this script.
#

TRUE=0
FALSE=1

VERBOSE=FALSE

main()
{
    if [ ! -e /opt/local/bin/port ]; then
        echo "Please install and/or update to the latest version of macports!"
        return $FALSE
    fi
    
    #sudo port install ftgl
    return $TRUE
}

main $*
if [ $? -ne $TRUE ]; then
    echo $0 .. " failed to complete."
fi
