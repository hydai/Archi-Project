#!/bin/bash

for i in {1..3}
do
    make clean
    make
    cp ../../open_testcase/example${i}/*.bin ./
    ./CMP
    diff ../../open_testcase/example${i}/snapshot.rpt snapshot.rpt
    diff ../../open_testcase/example${i}/report.rpt report.rpt
done
