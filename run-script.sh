#!bin/bash
echo "Start judge"
cd archi24
echo "Building simulator"
cd simulator
make clean
make
cd ..
echo "Judging"
mkdir judgingTemp
cp simulator/single_cycle judgingTemp
cp testcase/iimage.bin judgingTemp
cp testcase/dimage.bin judgingTemp
./judgingTemp/single_cycle
echo "Compare"
echo "Diff error_dump.rpt"
diff testcase/error_dump.rpt judgingTemp/error_dump.rpt
echo "Diff snapshot.rpt"
diff testcase/snapshot.rpt judgingTemp/snapshot.rpt
