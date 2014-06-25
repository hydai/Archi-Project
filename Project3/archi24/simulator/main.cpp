#include <cstdio>
#include "simulator.h"

int main(int argc, char *argv[])
{
    Simulator::Simulator *simulator;
    if (argc > 2) {
        printf("Start simulator - custom mode\n");
        int args[10];
        for (int i = 1; i <= 10; i++) {
            int idx = 0;
            args[i-1] = 0;
            while (argv[i][idx] != '\0') {
                args[i-1] = args[i-1]*10+(argv[i][idx]-'0');
                idx++;
            }
        }
        simulator = new Simulator::Simulator(
          args[0],
          args[1],
          args[2],
          args[3],
          args[4],
          args[5],
          args[6],
          args[7],
          args[8],
          args[9]);
        simulator->run();
        printf("Finish simulator\n");
    } else {
        printf("Start simulator - default mode\n");
        simulator = new Simulator::Simulator();
        simulator->run();
        printf("Finish simulator\n");
    }
    delete simulator;
    return 0;
}
