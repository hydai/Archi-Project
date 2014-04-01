#include <cstdio>
#include "simulator.h"

int main(int argc, char *argv[])
{
    printf("Start simulator\n");
    Simulator::Simulator *simulator = new Simulator();
    simulator->init();
    simulator->run();
    printf("Finish simulator\n");
    delete simulator;
    return 0;
}
