#!/bin/bash

read i
if [ "${i}" -lt "10" ]
    then
        sh ./clean.sh
        make
        cp ../../valid_testcase/archi0${i}/*.bin ./
        ./single_cycle
        vimdiff ../../valid_testcase/archi0${i}/error_dump.rpt ./error_dump.rpt
        vimdiff ../../valid_testcase/archi0${i}/snapshot.rpt ./snapshot.rpt
fi
if [ "${i}" -gt "9" ]
    then
        sh ./clean.sh
        make
        cp ../../valid_testcase/archi${i}/*.bin ./
        ./single_cycle
        vimdiff ../../valid_testcase/archi${i}/error_dump.rpt ./error_dump.rpt
        vimdiff ../../valid_testcase/archi${i}/snapshot.rpt ./snapshot.rpt
fi
