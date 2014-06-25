#!/bin/bash

for i in {1..3}
do
    make clean
    make
    cp ../../open_testcase/example${i}/*.bin ./
    ./CMP
    vimdiff ../../open_testcase/example${i}/snapshot.rpt snapshot.rpt +"TOhtml" +"x snapshot${i}.html" +"qa"
    vimdiff ../../open_testcase/example${i}/report.rpt report.rpt +"TOhtml" +"x report${i}.html" +"qa"
done
