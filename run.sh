#!/bin/bash
###########################################################################

echo "Executing sample"

export LD_LIBRARY_PATH="$(pwd)/bin:$LD_LIBRARY_PATH"

gnome-terminal --disable-factory -x tao_cosnaming -ORBDottedDecimalAddresses 1 -ORBListenEndpoints iiop://:2809 -ORBSvcConf svc-iso.conf &
perl -e "select(undef,undef,undef,2);"

gnome-terminal --disable-factory -x tao_rtevent -ORBDottedDecimalAddresses 1 -ORBInitRef NameService=corbaloc:iiop::2809/NameService -ORBSvcConf svc-iso.conf &
perl -e "select(undef,undef,undef,1);"

gnome-terminal --disable-factory -x ./bin/server -ORBDottedDecimalAddresses 1 -ORBInitRef NameService=corbaloc:iiop::2809/NameService -ORBSvcConf svc-cp850.conf -ORBDebugLevel 10 &
perl -e "select(undef,undef,undef,2);"

gnome-terminal --disable-factory -x ./bin/client -ORBDottedDecimalAddresses 1 -ORBInitRef NameService=corbaloc:iiop::2809/NameService -ORBSvcConf svc-iso.conf &
perl -e "select(undef,undef,undef,2);"

read -p "Press any key to terminate...
" -n1 -s
echo "Terminating"

kill 0



