#!/bin/bash

for i in {1..9}
do
    if [ -d ../../valid_testcase/archi0${i} ]
    then
        sh ./clean.sh
        make
        cp ../../valid_testcase/archi0${i}/*.bin ./
        ./single_cycle
        vimdiff ../../valid_testcase/archi0${i}/error_dump.rpt ./error_dump.rpt +"TOhtml" +"x error0${i}.html" +"qa"
        vimdiff ../../valid_testcase/archi0${i}/snapshot.rpt ./snapshot.rpt +"TOhtml" +"x snap0${i}.html" +"qa"
    fi
done
for (( i = 10; i <= 50; i++ )); do
    if [ -d ../../valid_testcase/archi${i} ]
    then
        sh ./clean.sh
        make
        cp ../../valid_testcase/archi${i}/*.bin ./
        ./single_cycle
        vimdiff ../../valid_testcase/archi${i}/error_dump.rpt ./error_dump.rpt +"TOhtml" +"x error${i}.html" +"qa"
        vimdiff ../../valid_testcase/archi${i}/snapshot.rpt ./snapshot.rpt +"TOhtml" +"x snap${i}.html" +"qa"
    fi
done
